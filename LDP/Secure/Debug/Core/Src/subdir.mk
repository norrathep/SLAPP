################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Hacl_HMAC_SHA2_256.c \
../Core/Src/Hacl_SHA2_256.c \
../Core/Src/auc.c \
../Core/Src/cfa_engine.c \
../Core/Src/gpio.c \
../Core/Src/gtzc.c \
../Core/Src/hash.c \
../Core/Src/kremlib.c \
../Core/Src/main.c \
../Core/Src/secure_nsc.c \
../Core/Src/serial_messages.c \
../Core/Src/stm32l5xx_hal_msp.c \
../Core/Src/stm32l5xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l5xx_s.c \
../Core/Src/tim.c \
../Core/Src/uECC.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/Hacl_HMAC_SHA2_256.o \
./Core/Src/Hacl_SHA2_256.o \
./Core/Src/auc.o \
./Core/Src/cfa_engine.o \
./Core/Src/gpio.o \
./Core/Src/gtzc.o \
./Core/Src/hash.o \
./Core/Src/kremlib.o \
./Core/Src/main.o \
./Core/Src/secure_nsc.o \
./Core/Src/serial_messages.o \
./Core/Src/stm32l5xx_hal_msp.o \
./Core/Src/stm32l5xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l5xx_s.o \
./Core/Src/tim.o \
./Core/Src/uECC.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/Hacl_HMAC_SHA2_256.d \
./Core/Src/Hacl_SHA2_256.d \
./Core/Src/auc.d \
./Core/Src/cfa_engine.d \
./Core/Src/gpio.d \
./Core/Src/gtzc.d \
./Core/Src/hash.d \
./Core/Src/kremlib.d \
./Core/Src/main.d \
./Core/Src/secure_nsc.d \
./Core/Src/serial_messages.d \
./Core/Src/stm32l5xx_hal_msp.d \
./Core/Src/stm32l5xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l5xx_s.d \
./Core/Src/tim.d \
./Core/Src/uECC.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Hacl_HMAC_SHA2_256.cyclo ./Core/Src/Hacl_HMAC_SHA2_256.d ./Core/Src/Hacl_HMAC_SHA2_256.o ./Core/Src/Hacl_HMAC_SHA2_256.su ./Core/Src/Hacl_SHA2_256.cyclo ./Core/Src/Hacl_SHA2_256.d ./Core/Src/Hacl_SHA2_256.o ./Core/Src/Hacl_SHA2_256.su ./Core/Src/auc.cyclo ./Core/Src/auc.d ./Core/Src/auc.o ./Core/Src/auc.su ./Core/Src/cfa_engine.cyclo ./Core/Src/cfa_engine.d ./Core/Src/cfa_engine.o ./Core/Src/cfa_engine.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/gtzc.cyclo ./Core/Src/gtzc.d ./Core/Src/gtzc.o ./Core/Src/gtzc.su ./Core/Src/hash.cyclo ./Core/Src/hash.d ./Core/Src/hash.o ./Core/Src/hash.su ./Core/Src/kremlib.cyclo ./Core/Src/kremlib.d ./Core/Src/kremlib.o ./Core/Src/kremlib.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/secure_nsc.cyclo ./Core/Src/secure_nsc.d ./Core/Src/secure_nsc.o ./Core/Src/secure_nsc.su ./Core/Src/serial_messages.cyclo ./Core/Src/serial_messages.d ./Core/Src/serial_messages.o ./Core/Src/serial_messages.su ./Core/Src/stm32l5xx_hal_msp.cyclo ./Core/Src/stm32l5xx_hal_msp.d ./Core/Src/stm32l5xx_hal_msp.o ./Core/Src/stm32l5xx_hal_msp.su ./Core/Src/stm32l5xx_it.cyclo ./Core/Src/stm32l5xx_it.d ./Core/Src/stm32l5xx_it.o ./Core/Src/stm32l5xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l5xx_s.cyclo ./Core/Src/system_stm32l5xx_s.d ./Core/Src/system_stm32l5xx_s.o ./Core/Src/system_stm32l5xx_s.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/uECC.cyclo ./Core/Src/uECC.d ./Core/Src/uECC.o ./Core/Src/uECC.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

