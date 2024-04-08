################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Device_Startup/ \
../Secure_Functions/


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Device_Startup/startup_saml11e16a.c \
../Device_Startup/system_saml11e16a.c \
../main.c \
../Secure_Functions/secure.c \
../trustzone_manager.c \
../veneer.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Device_Startup/startup_saml11e16a.o \
Device_Startup/system_saml11e16a.o \
main.o \
Secure_Functions/secure.o \
trustzone_manager.o \
veneer.o

OBJS_AS_ARGS +=  \
Device_Startup/startup_saml11e16a.o \
Device_Startup/system_saml11e16a.o \
main.o \
Secure_Functions/secure.o \
trustzone_manager.o \
veneer.o

C_DEPS +=  \
Device_Startup/startup_saml11e16a.d \
Device_Startup/system_saml11e16a.d \
main.d \
Secure_Functions/secure.d \
trustzone_manager.d \
veneer.d

C_DEPS_AS_ARGS +=  \
Device_Startup/startup_saml11e16a.d \
Device_Startup/system_saml11e16a.d \
main.d \
Secure_Functions/secure.d \
trustzone_manager.d \
veneer.d

OUTPUT_FILE_PATH +=SecureApp.elf

OUTPUT_FILE_PATH_AS_ARGS +=SecureApp.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+=  \
../Device_Startup/saml11e16a_flash.ld \
../Device_Startup/saml11e16a_sram.ld


# AVR32/GNU C Compiler
Device_Startup/startup_saml11e16a.o: ../Device_Startup/startup_saml11e16a.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Device_Startup/system_saml11e16a.o: ../Device_Startup/system_saml11e16a.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Secure_Functions/secure.o: ../Secure_Functions/secure.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./trustzone_manager.o: .././trustzone_manager.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./veneer.o: .././veneer.c
	@echo Building file: $<
	@echo Invoking: ARM/GNU C Compiler : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE)  -x c -mthumb -D__SAML11E16A__ -DDEBUG  -I"../Secure_Functions" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\arm\CMSIS\5.4.0\CMSIS\Core\Include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\SAML11_DFP\1.0.109\include\component"  -Og -ffunction-sections -mlong-calls -g3 -Wall -mcpu=cortex-m23 -c -std=gnu99 -mcmse -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker : 6.3.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -mthumb -Wl,-Map="SecureApp.map" --specs=nano.specs --specs=nosys.specs -Wl,--start-group -lm  -Wl,--end-group -L"..\\Device_Startup"  -Wl,--gc-sections -mcpu=cortex-m23 -Tsaml11e16a_flash.ld -Wl,--out-implib=secureapp-cmse-implib.lib -Wl,--cmse-implib  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "SecureApp.elf" "SecureApp.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "SecureApp.elf" "SecureApp.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objdump.exe" -h -S "SecureApp.elf" > "SecureApp.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "SecureApp.elf" "SecureApp.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\arm\arm-gnu-toolchain\bin\arm-none-eabi-size.exe" "SecureApp.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "SecureApp.elf" "SecureApp.a" "SecureApp.hex" "SecureApp.bin" "SecureApp.lss" "SecureApp.eep" "SecureApp.map" "SecureApp.srec"
	