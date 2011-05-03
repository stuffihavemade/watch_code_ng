//The only changes from the stock sports watch is this file (main.c)
//and ports.c. You need to delete the default PORT2_ISR(void) in it.
//Also if you want to change WATCH_NUMBER to the watch go
//to  Project->Properties->C\C++ Build->MSP430 Compiler->Predefined Symbols
//and add WATCH_NUMBER=1, for example.


#include "project.h"
#include <string.h>
#include "clock.h"
#include "display.h"
#include "vti_as.h"
#include "vti_ps.h"
#include "radio.h"
#include "buzzer.h"
#include "ports.h"
#include "timer.h"
#include "pmm.h"
#include "rf1a.h"
#include "acceleration.h"

void init_application(void);
void send_packet(void);
int main(void)
{
	init_application();
	WDTCTL = WDTPW + WDTHOLD;
	LPM4;
}
#include "ports.h"
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	u8 int_flag, int_enable;
	// Remember interrupt enable bits
	int_enable = BUTTONS_IE;

	// Store valid button interrupt flag
	int_flag = BUTTONS_IFG & int_enable;
	if ((int_flag & ALL_BUTTONS) != 0)			
  	{
  		//the stuff with buttons I don't really understand,
  		//we got it from ports.c. It has to do with clearning
  		//out iterrupts.
  		BUTTONS_IE = 0;
  		send_packet();
		BUTTONS_IFG = 0x00; 	
		BUTTONS_IE  = int_enable; 	
  	}
}

#include "mrfi.h"
#include "bsp.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "radio.h"
extern uint8_t sInit_done;
void send_packet() {
	linkID_t lid;
	uint8_t msg[1];
    msg[0] = 0xFE;
    msg[1] = 0xCA;
    msg[2] = 0xCE;
    msg[3] = 0xFA;
	open_radio();
	BSP_InitBoard();
 	SMPL_Init(0);
	SMPL_Link(&lid);
	SMPL_SendOpt(lid, msg, 4, SMPL_TXOPTION_NONE);
	sInit_done = 0;
	close_radio();
}

//This is unchanged from stock.
void init_application(void)
{
	volatile unsigned char *ptr;
	  
	// ---------------------------------------------------------------------
	// Enable watchdog
	
	// Watchdog triggers after 16 seconds when not cleared
#ifdef USE_WATCHDOG		
	WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK;
#else
	WDTCTL = WDTPW + WDTHOLD;
#endif
	
	// ---------------------------------------------------------------------
	// Configure PMM
	SetVCore(3);
	
	// Set global high power request enable
	PMMCTL0_H  = 0xA5;
	PMMCTL0_L |= PMMHPMRE;
	PMMCTL0_H  = 0x00;	

	// ---------------------------------------------------------------------
	// Enable 32kHz ACLK	
	P5SEL |= 0x03;                            // Select XIN, XOUT on P5.0 and P5.1
	UCSCTL6 &= ~XT1OFF;        				  // XT1 On, Highest drive strength
	UCSCTL6 |= XCAP_3;                        // Internal load cap

	UCSCTL3 = SELA__XT1CLK;                   // Select XT1 as FLL reference
	UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;      
	
	// ---------------------------------------------------------------------
	// Configure CPU clock for 12MHz
	_BIS_SR(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;          // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;       // Select suitable range
	UCSCTL2 = FLLD_1 + 0x16E;  // Set DCO Multiplier
	_BIC_SR(SCG0);                  // Enable the FLL control loop

    // Worst-case settling time for the DCO when the DCO range bits have been
    // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
    // UG for optimization.
    // 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
    __delay_cycles(250000);
  
	// Loop until XT1 & DCO stabilizes, use do-while to insure that 
	// body is executed at least once
	do
	{
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	} while ((SFRIFG1 & OFIFG));	

	
	// ---------------------------------------------------------------------
	// Configure port mapping
	
	// Disable all interrupts
	__disable_interrupt();
	// Get write-access to port mapping registers:
	PMAPPWD = 0x02D52;
	// Allow reconfiguration during runtime:
	PMAPCTL = PMAPRECFG;

	// P2.7 = TA0CCR1A or TA1CCR0A output (buzzer output)
	ptr  = &P2MAP0;
	*(ptr+7) = PM_TA1CCR0A;
	P2OUT &= ~BIT7;
	P2DIR |= BIT7;

	// P1.5 = SPI MISO input
	ptr  = &P1MAP0;
	*(ptr+5) = PM_UCA0SOMI;
	// P1.6 = SPI MOSI output
	*(ptr+6) = PM_UCA0SIMO;
	// P1.7 = SPI CLK output
	*(ptr+7) = PM_UCA0CLK;

	// Disable write-access to port mapping registers:
	PMAPPWD = 0;
	// Re-enable all interrupts
	__enable_interrupt();
	
	// ---------------------------------------------------------------------
	// Configure ports

	// ---------------------------------------------------------------------
	// Reset radio core
	radio_reset();
	radio_powerdown();	
	
	// ---------------------------------------------------------------------
	// Init acceleration sensor
	as_init();
	
	// ---------------------------------------------------------------------
	// Init LCD
	lcd_init();
  
	// ---------------------------------------------------------------------
	// Init buttons
	init_buttons();

	// ---------------------------------------------------------------------
	// Configure Timer0 for use by the clock and delay functions
	Timer0_Init();
	
	// ---------------------------------------------------------------------
	// Init pressure sensor
	ps_init();
}

//Everything after this line is just stuff needed to compile
//because other files are dependent on it (nice coupling TI!).
//It is not actually used by us and can be safely ignored.

void to_lpm(void)
{
	// Go to LPM3
	_BIS_SR(LPM3_bits + GIE); 
	__no_operation();
}

void idle_loop(void)
{
	// To low power mode
	to_lpm();

#ifdef USE_WATCHDOG		
	// Service watchdog
	WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK + WDTCNTCL;
#endif
}

volatile s_system_flags sys;
volatile s_request_flags request;
volatile s_message_flags message;
u8 rf_frequoffset;
void (*fptr_lcd_function_line1)(u8 line, u8 update);
void (*fptr_lcd_function_line2)(u8 line, u8 update);
