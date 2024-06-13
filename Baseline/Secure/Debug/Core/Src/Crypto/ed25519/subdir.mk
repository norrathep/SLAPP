################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Crypto/ed25519/c25519.c \
../Core/Src/Crypto/ed25519/ed25519.c \
../Core/Src/Crypto/ed25519/edsign.c \
../Core/Src/Crypto/ed25519/f25519.c \
../Core/Src/Crypto/ed25519/fprime.c \
../Core/Src/Crypto/ed25519/morph25519.c \
../Core/Src/Crypto/ed25519/sha512.c 

OBJS += \
./Core/Src/Crypto/ed25519/c25519.o \
./Core/Src/Crypto/ed25519/ed25519.o \
./Core/Src/Crypto/ed25519/edsign.o \
./Core/Src/Crypto/ed25519/f25519.o \
./Core/Src/Crypto/ed25519/fprime.o \
./Core/Src/Crypto/ed25519/morph25519.o \
./Core/Src/Crypto/ed25519/sha512.o 

C_DEPS += \
./Core/Src/Crypto/ed25519/c25519.d \
./Core/Src/Crypto/ed25519/ed25519.d \
./Core/Src/Crypto/ed25519/edsign.d \
./Core/Src/Crypto/ed25519/f25519.d \
./Core/Src/Crypto/ed25519/fprime.d \
./Core/Src/Crypto/ed25519/morph25519.d \
./Core/Src/Crypto/ed25519/sha512.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Crypto/ed25519/%.o Core/Src/Crypto/ed25519/%.su Core/Src/Crypto/ed25519/%.cyclo: ../Core/Src/Crypto/ed25519/%.c Core/Src/Crypto/ed25519/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Crypto-2f-ed25519

clean-Core-2f-Src-2f-Crypto-2f-ed25519:
	-$(RM) ./Core/Src/Crypto/ed25519/c25519.cyclo ./Core/Src/Crypto/ed25519/c25519.d ./Core/Src/Crypto/ed25519/c25519.o ./Core/Src/Crypto/ed25519/c25519.su ./Core/Src/Crypto/ed25519/ed25519.cyclo ./Core/Src/Crypto/ed25519/ed25519.d ./Core/Src/Crypto/ed25519/ed25519.o ./Core/Src/Crypto/ed25519/ed25519.su ./Core/Src/Crypto/ed25519/edsign.cyclo ./Core/Src/Crypto/ed25519/edsign.d ./Core/Src/Crypto/ed25519/edsign.o ./Core/Src/Crypto/ed25519/edsign.su ./Core/Src/Crypto/ed25519/f25519.cyclo ./Core/Src/Crypto/ed25519/f25519.d ./Core/Src/Crypto/ed25519/f25519.o ./Core/Src/Crypto/ed25519/f25519.su ./Core/Src/Crypto/ed25519/fprime.cyclo ./Core/Src/Crypto/ed25519/fprime.d ./Core/Src/Crypto/ed25519/fprime.o ./Core/Src/Crypto/ed25519/fprime.su ./Core/Src/Crypto/ed25519/morph25519.cyclo ./Core/Src/Crypto/ed25519/morph25519.d ./Core/Src/Crypto/ed25519/morph25519.o ./Core/Src/Crypto/ed25519/morph25519.su ./Core/Src/Crypto/ed25519/sha512.cyclo ./Core/Src/Crypto/ed25519/sha512.d ./Core/Src/Crypto/ed25519/sha512.o ./Core/Src/Crypto/ed25519/sha512.su

.PHONY: clean-Core-2f-Src-2f-Crypto-2f-ed25519

