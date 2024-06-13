################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/kann/examples/textgen.c 

OBJS += \
./Core/Src/kann/examples/textgen.o 

C_DEPS += \
./Core/Src/kann/examples/textgen.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/kann/examples/%.o Core/Src/kann/examples/%.su Core/Src/kann/examples/%.cyclo: ../Core/Src/kann/examples/%.c Core/Src/kann/examples/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-kann-2f-examples

clean-Core-2f-Src-2f-kann-2f-examples:
	-$(RM) ./Core/Src/kann/examples/textgen.cyclo ./Core/Src/kann/examples/textgen.d ./Core/Src/kann/examples/textgen.o ./Core/Src/kann/examples/textgen.su

.PHONY: clean-Core-2f-Src-2f-kann-2f-examples

