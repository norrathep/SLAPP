################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Crypto/sphincs/endian.c \
../Core/Src/Crypto/sphincs/fips202.c \
../Core/Src/Crypto/sphincs/get_space.c \
../Core/Src/Crypto/sphincs/get_space2.c \
../Core/Src/Crypto/sphincs/key_gen.c \
../Core/Src/Crypto/sphincs/sha256.c \
../Core/Src/Crypto/sphincs/sha256_L1_hash.c \
../Core/Src/Crypto/sphincs/sha256_L1_hash_simple.c \
../Core/Src/Crypto/sphincs/sha256_hash.c \
../Core/Src/Crypto/sphincs/sha2_128f_simple.c \
../Core/Src/Crypto/sphincs/sha2_128s_simple.c \
../Core/Src/Crypto/sphincs/sha2_192f_simple.c \
../Core/Src/Crypto/sphincs/sha2_192s_simple.c \
../Core/Src/Crypto/sphincs/sha2_256f_simple.c \
../Core/Src/Crypto/sphincs/sha2_256s_simple.c \
../Core/Src/Crypto/sphincs/sha2_simple.c \
../Core/Src/Crypto/sphincs/sha512.c \
../Core/Src/Crypto/sphincs/sha512_L35_hash_simple.c \
../Core/Src/Crypto/sphincs/sha512_hash.c \
../Core/Src/Crypto/sphincs/shake256_128f_simple.c \
../Core/Src/Crypto/sphincs/shake256_128s_simple.c \
../Core/Src/Crypto/sphincs/shake256_192f_simple.c \
../Core/Src/Crypto/sphincs/shake256_192s_simple.c \
../Core/Src/Crypto/sphincs/shake256_256f_simple.c \
../Core/Src/Crypto/sphincs/shake256_256s_simple.c \
../Core/Src/Crypto/sphincs/shake256_hash.c \
../Core/Src/Crypto/sphincs/shake256_simple.c \
../Core/Src/Crypto/sphincs/size.c \
../Core/Src/Crypto/sphincs/stack.c \
../Core/Src/Crypto/sphincs/tiny_sphincs.c \
../Core/Src/Crypto/sphincs/verify.c 

OBJS += \
./Core/Src/Crypto/sphincs/endian.o \
./Core/Src/Crypto/sphincs/fips202.o \
./Core/Src/Crypto/sphincs/get_space.o \
./Core/Src/Crypto/sphincs/get_space2.o \
./Core/Src/Crypto/sphincs/key_gen.o \
./Core/Src/Crypto/sphincs/sha256.o \
./Core/Src/Crypto/sphincs/sha256_L1_hash.o \
./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.o \
./Core/Src/Crypto/sphincs/sha256_hash.o \
./Core/Src/Crypto/sphincs/sha2_128f_simple.o \
./Core/Src/Crypto/sphincs/sha2_128s_simple.o \
./Core/Src/Crypto/sphincs/sha2_192f_simple.o \
./Core/Src/Crypto/sphincs/sha2_192s_simple.o \
./Core/Src/Crypto/sphincs/sha2_256f_simple.o \
./Core/Src/Crypto/sphincs/sha2_256s_simple.o \
./Core/Src/Crypto/sphincs/sha2_simple.o \
./Core/Src/Crypto/sphincs/sha512.o \
./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.o \
./Core/Src/Crypto/sphincs/sha512_hash.o \
./Core/Src/Crypto/sphincs/shake256_128f_simple.o \
./Core/Src/Crypto/sphincs/shake256_128s_simple.o \
./Core/Src/Crypto/sphincs/shake256_192f_simple.o \
./Core/Src/Crypto/sphincs/shake256_192s_simple.o \
./Core/Src/Crypto/sphincs/shake256_256f_simple.o \
./Core/Src/Crypto/sphincs/shake256_256s_simple.o \
./Core/Src/Crypto/sphincs/shake256_hash.o \
./Core/Src/Crypto/sphincs/shake256_simple.o \
./Core/Src/Crypto/sphincs/size.o \
./Core/Src/Crypto/sphincs/stack.o \
./Core/Src/Crypto/sphincs/tiny_sphincs.o \
./Core/Src/Crypto/sphincs/verify.o 

C_DEPS += \
./Core/Src/Crypto/sphincs/endian.d \
./Core/Src/Crypto/sphincs/fips202.d \
./Core/Src/Crypto/sphincs/get_space.d \
./Core/Src/Crypto/sphincs/get_space2.d \
./Core/Src/Crypto/sphincs/key_gen.d \
./Core/Src/Crypto/sphincs/sha256.d \
./Core/Src/Crypto/sphincs/sha256_L1_hash.d \
./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.d \
./Core/Src/Crypto/sphincs/sha256_hash.d \
./Core/Src/Crypto/sphincs/sha2_128f_simple.d \
./Core/Src/Crypto/sphincs/sha2_128s_simple.d \
./Core/Src/Crypto/sphincs/sha2_192f_simple.d \
./Core/Src/Crypto/sphincs/sha2_192s_simple.d \
./Core/Src/Crypto/sphincs/sha2_256f_simple.d \
./Core/Src/Crypto/sphincs/sha2_256s_simple.d \
./Core/Src/Crypto/sphincs/sha2_simple.d \
./Core/Src/Crypto/sphincs/sha512.d \
./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.d \
./Core/Src/Crypto/sphincs/sha512_hash.d \
./Core/Src/Crypto/sphincs/shake256_128f_simple.d \
./Core/Src/Crypto/sphincs/shake256_128s_simple.d \
./Core/Src/Crypto/sphincs/shake256_192f_simple.d \
./Core/Src/Crypto/sphincs/shake256_192s_simple.d \
./Core/Src/Crypto/sphincs/shake256_256f_simple.d \
./Core/Src/Crypto/sphincs/shake256_256s_simple.d \
./Core/Src/Crypto/sphincs/shake256_hash.d \
./Core/Src/Crypto/sphincs/shake256_simple.d \
./Core/Src/Crypto/sphincs/size.d \
./Core/Src/Crypto/sphincs/stack.d \
./Core/Src/Crypto/sphincs/tiny_sphincs.d \
./Core/Src/Crypto/sphincs/verify.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Crypto/sphincs/%.o Core/Src/Crypto/sphincs/%.su Core/Src/Crypto/sphincs/%.cyclo: ../Core/Src/Crypto/sphincs/%.c Core/Src/Crypto/sphincs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../../Secure_nsclib -I../../Drivers/STM32L5xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Include -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -mcmse -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Crypto-2f-sphincs

clean-Core-2f-Src-2f-Crypto-2f-sphincs:
	-$(RM) ./Core/Src/Crypto/sphincs/endian.cyclo ./Core/Src/Crypto/sphincs/endian.d ./Core/Src/Crypto/sphincs/endian.o ./Core/Src/Crypto/sphincs/endian.su ./Core/Src/Crypto/sphincs/fips202.cyclo ./Core/Src/Crypto/sphincs/fips202.d ./Core/Src/Crypto/sphincs/fips202.o ./Core/Src/Crypto/sphincs/fips202.su ./Core/Src/Crypto/sphincs/get_space.cyclo ./Core/Src/Crypto/sphincs/get_space.d ./Core/Src/Crypto/sphincs/get_space.o ./Core/Src/Crypto/sphincs/get_space.su ./Core/Src/Crypto/sphincs/get_space2.cyclo ./Core/Src/Crypto/sphincs/get_space2.d ./Core/Src/Crypto/sphincs/get_space2.o ./Core/Src/Crypto/sphincs/get_space2.su ./Core/Src/Crypto/sphincs/key_gen.cyclo ./Core/Src/Crypto/sphincs/key_gen.d ./Core/Src/Crypto/sphincs/key_gen.o ./Core/Src/Crypto/sphincs/key_gen.su ./Core/Src/Crypto/sphincs/sha256.cyclo ./Core/Src/Crypto/sphincs/sha256.d ./Core/Src/Crypto/sphincs/sha256.o ./Core/Src/Crypto/sphincs/sha256.su ./Core/Src/Crypto/sphincs/sha256_L1_hash.cyclo ./Core/Src/Crypto/sphincs/sha256_L1_hash.d ./Core/Src/Crypto/sphincs/sha256_L1_hash.o ./Core/Src/Crypto/sphincs/sha256_L1_hash.su ./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.cyclo ./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.d ./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.o ./Core/Src/Crypto/sphincs/sha256_L1_hash_simple.su ./Core/Src/Crypto/sphincs/sha256_hash.cyclo ./Core/Src/Crypto/sphincs/sha256_hash.d ./Core/Src/Crypto/sphincs/sha256_hash.o ./Core/Src/Crypto/sphincs/sha256_hash.su ./Core/Src/Crypto/sphincs/sha2_128f_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_128f_simple.d ./Core/Src/Crypto/sphincs/sha2_128f_simple.o ./Core/Src/Crypto/sphincs/sha2_128f_simple.su ./Core/Src/Crypto/sphincs/sha2_128s_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_128s_simple.d ./Core/Src/Crypto/sphincs/sha2_128s_simple.o ./Core/Src/Crypto/sphincs/sha2_128s_simple.su ./Core/Src/Crypto/sphincs/sha2_192f_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_192f_simple.d ./Core/Src/Crypto/sphincs/sha2_192f_simple.o ./Core/Src/Crypto/sphincs/sha2_192f_simple.su ./Core/Src/Crypto/sphincs/sha2_192s_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_192s_simple.d ./Core/Src/Crypto/sphincs/sha2_192s_simple.o ./Core/Src/Crypto/sphincs/sha2_192s_simple.su ./Core/Src/Crypto/sphincs/sha2_256f_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_256f_simple.d ./Core/Src/Crypto/sphincs/sha2_256f_simple.o ./Core/Src/Crypto/sphincs/sha2_256f_simple.su ./Core/Src/Crypto/sphincs/sha2_256s_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_256s_simple.d ./Core/Src/Crypto/sphincs/sha2_256s_simple.o ./Core/Src/Crypto/sphincs/sha2_256s_simple.su ./Core/Src/Crypto/sphincs/sha2_simple.cyclo ./Core/Src/Crypto/sphincs/sha2_simple.d ./Core/Src/Crypto/sphincs/sha2_simple.o ./Core/Src/Crypto/sphincs/sha2_simple.su ./Core/Src/Crypto/sphincs/sha512.cyclo ./Core/Src/Crypto/sphincs/sha512.d ./Core/Src/Crypto/sphincs/sha512.o ./Core/Src/Crypto/sphincs/sha512.su ./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.cyclo ./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.d ./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.o ./Core/Src/Crypto/sphincs/sha512_L35_hash_simple.su ./Core/Src/Crypto/sphincs/sha512_hash.cyclo ./Core/Src/Crypto/sphincs/sha512_hash.d ./Core/Src/Crypto/sphincs/sha512_hash.o ./Core/Src/Crypto/sphincs/sha512_hash.su ./Core/Src/Crypto/sphincs/shake256_128f_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_128f_simple.d ./Core/Src/Crypto/sphincs/shake256_128f_simple.o ./Core/Src/Crypto/sphincs/shake256_128f_simple.su ./Core/Src/Crypto/sphincs/shake256_128s_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_128s_simple.d ./Core/Src/Crypto/sphincs/shake256_128s_simple.o ./Core/Src/Crypto/sphincs/shake256_128s_simple.su ./Core/Src/Crypto/sphincs/shake256_192f_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_192f_simple.d ./Core/Src/Crypto/sphincs/shake256_192f_simple.o ./Core/Src/Crypto/sphincs/shake256_192f_simple.su ./Core/Src/Crypto/sphincs/shake256_192s_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_192s_simple.d ./Core/Src/Crypto/sphincs/shake256_192s_simple.o ./Core/Src/Crypto/sphincs/shake256_192s_simple.su ./Core/Src/Crypto/sphincs/shake256_256f_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_256f_simple.d ./Core/Src/Crypto/sphincs/shake256_256f_simple.o ./Core/Src/Crypto/sphincs/shake256_256f_simple.su ./Core/Src/Crypto/sphincs/shake256_256s_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_256s_simple.d ./Core/Src/Crypto/sphincs/shake256_256s_simple.o ./Core/Src/Crypto/sphincs/shake256_256s_simple.su ./Core/Src/Crypto/sphincs/shake256_hash.cyclo ./Core/Src/Crypto/sphincs/shake256_hash.d ./Core/Src/Crypto/sphincs/shake256_hash.o ./Core/Src/Crypto/sphincs/shake256_hash.su ./Core/Src/Crypto/sphincs/shake256_simple.cyclo ./Core/Src/Crypto/sphincs/shake256_simple.d ./Core/Src/Crypto/sphincs/shake256_simple.o ./Core/Src/Crypto/sphincs/shake256_simple.su ./Core/Src/Crypto/sphincs/size.cyclo ./Core/Src/Crypto/sphincs/size.d ./Core/Src/Crypto/sphincs/size.o ./Core/Src/Crypto/sphincs/size.su ./Core/Src/Crypto/sphincs/stack.cyclo ./Core/Src/Crypto/sphincs/stack.d ./Core/Src/Crypto/sphincs/stack.o ./Core/Src/Crypto/sphincs/stack.su ./Core/Src/Crypto/sphincs/tiny_sphincs.cyclo ./Core/Src/Crypto/sphincs/tiny_sphincs.d ./Core/Src/Crypto/sphincs/tiny_sphincs.o ./Core/Src/Crypto/sphincs/tiny_sphincs.su ./Core/Src/Crypto/sphincs/verify.cyclo ./Core/Src/Crypto/sphincs/verify.d ./Core/Src/Crypto/sphincs/verify.o ./Core/Src/Crypto/sphincs/verify.su

.PHONY: clean-Core-2f-Src-2f-Crypto-2f-sphincs

