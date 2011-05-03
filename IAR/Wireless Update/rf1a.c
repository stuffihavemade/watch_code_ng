// *************************************************************************************************
//
// Actual revision: $Revision: $
// Revision label:  $Name: $
// Revision state:  $State: $
//
// *************************************************************************************************
// Radio core access functions. Taken from TI reference code for CC430.
// *************************************************************************************************


// *************************************************************************************************
// Include section

// system
#include "project_defs.h"

// driver
#include "rf1a.h"
#include "wbsl.h"
// *************************************************************************************************
// Global section


// *************************************************************************************************
// @fn          Strobe
// @brief       Send command to radio.
// @param       none
// @return      none
// *************************************************************************************************
u8 Strobe(u8 strobe)
{
   u8 statusByte = 0;
   u16 int_state, gdo_state;
  
   // Check for valid strobe command 
   if((strobe == 0xBD) || ((strobe > RF_SRES) && (strobe < RF_SNOP)))
   {      
      ENTER_CRITICAL_SECTION(int_state);
      
      // Clear the Status read flag 
      RF1AIFCTL1 &= ~(RFSTATIFG);
      
      // Wait for radio to be ready for next instruction
      while( !(RF1AIFCTL1 & RFINSTRIFG));

      // Write the strobe instruction
      if ((strobe > RF_SRES) && (strobe < RF_SNOP))
      {
        
         gdo_state = ReadSingleReg(IOCFG2); // buffer IOCFG2 state
         WriteSingleReg(IOCFG2, 0x29); // c-ready to GDO2
        
         RF1AINSTRB = strobe; 
         if ( (RF1AIN&0x04)== 0x04 )             // chip at sleep mode
         {
            if ( (strobe == RF_SXOFF) || (strobe == RF_SPWD) || (strobe == RF_SWOR) ) { }
            else    
            {
               while ((RF1AIN&0x04)== 0x04);                   // c-ready ?
               __delay_cycles(9800);   // Delay for ~810usec at 12MHz CPU clock
            }
         }
         WriteSingleReg(IOCFG2, gdo_state); // restore IOCFG2 setting
      }
      else      // chip active mode
      { 
         RF1AINSTRB = strobe;     
      }
      statusByte = RF1ASTATB;

      EXIT_CRITICAL_SECTION(int_state);
   }
   return statusByte;
}

// *************************************************************************************************
// @fn          ResetRadioCore
// @brief       Software reset radio core.
// @param       none
// @return      none
// *************************************************************************************************
/*
void ResetRadioCore(void)
{
  Strobe(RF_SRES);                             // Reset the Radio Core
  Strobe(RF_SNOP);                             // Reset Radio Pointer
}
*/

// *************************************************************************************************
// @fn          ReadSingleReg
// @brief       Read byte from register.
// @param       none
// @return      none
// *************************************************************************************************
u8 ReadSingleReg(u8 addr)
{
   u8 x;
   u16 int_state;

   ENTER_CRITICAL_SECTION(int_state);
 
   while (!(RF1AIFCTL1 & RFINSTRIFG)); 
 
   if( (addr <= 0x2E) || (addr == 0x3E) )
   {
      RF1AINSTR1B = (addr | RF_SNGLREGRD); 
   }
   else
   {
      RF1AINSTR1B = (addr | RF_REGRD); 
   } 
   
   x = RF1ADOUTB;
   //RF1ADINB = 0; //dummy write
   EXIT_CRITICAL_SECTION(int_state);  

   return x;
}

// *************************************************************************************************
// @fn          WriteSingleReg
// @brief       Write byte to register.
// @param       none
// @return      none
// *************************************************************************************************
void WriteSingleReg(u8 addr, u8 value)
{ 
   volatile u16 i;
   u16 int_state;

   ENTER_CRITICAL_SECTION(int_state);
	
   while (!(RF1AIFCTL1 & RFINSTRIFG));     // Wait for the Radio to be ready for the next instruction    
    
   RF1AINSTRW = ((addr | RF_REGWR)<<8 ) + value; // Send address + Instruction
   while (!(RFDINIFG & RF1AIFCTL1));

   i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte

   EXIT_CRITICAL_SECTION(int_state);
}

// *************************************************************************************************
// @fn          WriteBurstReg
// @brief       Write sequence of bytes to register.
// @param       none
// @return      none
// *************************************************************************************************
void WriteBurstReg(u8 addr, u8 *buffer, u8 count)
{  
   // Write Burst works wordwise not bytewise - bug known already
   u8 i;                             
   u16 int_state;

   ENTER_CRITICAL_SECTION(int_state);

   while (!(RF1AIFCTL1 & RFINSTRIFG));       // Wait for the Radio to be ready for next instruction
   RF1AINSTRW = ((addr | RF_REGWR)<<8 ) + buffer[0]; // Send address + Instruction

   for (i = 1; i < count; i++)
   {
      while (!(RFDINIFG & RF1AIFCTL1));       // Wait for TX to finish
      RF1ADINB = buffer[i];                   // Send data
   }  
   i = RF1ADOUTB;                            // Reset RFDOUTIFG flag which contains status byte
   
   EXIT_CRITICAL_SECTION(int_state);
}


// *************************************************************************************************
// @fn          WritePATable
// @brief       Write data to power table
// @param       u8 value		Value to write
// @return      none
// *************************************************************************************************
void WritePATable(u8 value)
{
   u8 readbackPATableValue = 0;
   u16 int_state;

   ENTER_CRITICAL_SECTION(int_state);

   while(readbackPATableValue != value)
   {
      while( !(RF1AIFCTL1 & RFINSTRIFG));
      RF1AINSTRW = 0x7E00 + value;               // PA Table write (burst)
  
      while( !(RF1AIFCTL1 & RFINSTRIFG));
      RF1AINSTRB = RF_SNOP;                      // reset pointer
  

      while( !(RF1AIFCTL1 & RFINSTRIFG));
      RF1AINSTRB = 0xFE;                      // PA Table read (burst)

      while( !(RF1AIFCTL1 & RFDINIFG));
      RF1ADINB = 0x00;                       //dummy write

      while( !(RF1AIFCTL1 & RFDOUTIFG));
      readbackPATableValue = RF1ADOUT0B;

      while( !(RF1AIFCTL1 & RFINSTRIFG));
      RF1AINSTRB = RF_SNOP;
   }

   EXIT_CRITICAL_SECTION(int_state); 
}


