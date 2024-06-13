################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/kann/kann.c \
../Core/Src/kann/kautodiff.c 

OBJS += \
./Core/Src/kann/kann.o \
./Core/Src/kann/kautodiff.o 

C_DEPS += \
./Core/Src/kann/kann.d \
./Core/Src/kann/kautodiff.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/kann/%.o Core/Src/kann/%.su Core/Src/kann/%.cyclo: ../Core/Src/kann/%.c Core/Src/kann/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-kann

clean-Core-2f-Src-2f-kann:
	-$(RM) ./Core/Src/kann/kann.cyclo ./Core/Src/kann/kann.d ./Core/Src/kann/kann.o ./Core/Src/kann/kann.su ./Core/Src/kann/kautodiff.cyclo ./Core/Src/kann/kautodiff.d ./Core/Src/kann/kautodiff.o ./Core/Src/kann/kautodiff.su

.PHONY: clean-Core-2f-Src-2f-kann

