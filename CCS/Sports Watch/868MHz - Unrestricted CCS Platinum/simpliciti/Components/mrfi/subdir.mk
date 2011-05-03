################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../simpliciti/Components/mrfi/mrfi.c 

OBJS += \
./simpliciti/Components/mrfi/mrfi.obj 

C_DEPS += \
./simpliciti/Components/mrfi/mrfi.pp 

OBJS__QTD += \
".\simpliciti\Components\mrfi\mrfi.obj" 

C_DEPS__QTD += \
".\simpliciti\Components\mrfi\mrfi.pp" 

C_SRCS_QUOTED += \
"../simpliciti/Components/mrfi/mrfi.c" 


# Each subdirectory must supply rules for building sources it contributes
simpliciti/Components/mrfi/mrfi.obj: ../simpliciti/Components/mrfi/mrfi.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	$(shell echo --cmd_file="C:\Users\user\watch_code\CCS\Sports Watch\simpliciti\Applications\configuration\smpl_nwk_config.dat" --cmd_file="C:\Users\user\watch_code\CCS\Sports Watch\simpliciti\Applications\configuration\End Device\smpl_config.dat" > "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --silicon_version=mspx >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo -g >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo -O3 >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --define=__CCE__ --define=ISM_EU --define=__CC430F6137__ --define=MRFI_CC430 >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --include_path="C:/Program Files/Texas Instruments/ccsv4/msp430/include" --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/include" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/include" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/driver" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/logic" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/bluerobin" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Applications/application/End Device" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/boards" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/boards/CC430EM" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/boards/CC430EM/bsp_external" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/drivers" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/drivers/code" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/bsp/mcus" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/mrfi" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/mrfi/radios" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/mrfi/radios/family5" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/mrfi/smartrf" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/nwk" --include_path="C:/Users/user/watch_code/CCS/Sports Watch/simpliciti/Components/nwk_applications" >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --diag_warning=225 >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --call_assumptions=0 >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --gen_opt_info=2 >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --printf_support=minimal >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --preproc_with_compile >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --preproc_dependency="simpliciti/Components/mrfi/mrfi.pp" >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(shell echo --obj_directory="simpliciti/Components/mrfi" >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt")
	$(if $(strip $(GEN_OPTS_QUOTED)), $(shell echo $(GEN_OPTS_QUOTED) >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt"))
	$(if $(strip $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")), $(shell echo $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#") >> "simpliciti/Components/mrfi/mrfi_ccsCompiler.opt"))
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/msp430/bin/cl430" -@"simpliciti/Components/mrfi/mrfi_ccsCompiler.opt"
	@echo 'Finished building: $<'
	@echo ' '


