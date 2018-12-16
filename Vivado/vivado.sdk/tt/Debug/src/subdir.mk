################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/xgpio.c \
../src/xgpio_example.c \
../src/xgpio_extra.c \
../src/xgpio_g.c \
../src/xgpio_intr.c \
../src/xgpio_selftest.c \
../src/xgpio_sinit.c 

OBJS += \
./src/xgpio.o \
./src/xgpio_example.o \
./src/xgpio_extra.o \
./src/xgpio_g.o \
./src/xgpio_intr.o \
./src/xgpio_selftest.o \
./src/xgpio_sinit.o 

C_DEPS += \
./src/xgpio.d \
./src/xgpio_example.d \
./src/xgpio_extra.d \
./src/xgpio_g.d \
./src/xgpio_intr.d \
./src/xgpio_selftest.d \
./src/xgpio_sinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -I../../standalone_bsp_0/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


