################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/analog_out.c \
../Src/button.c \
../Src/cbfifo.c \
../Src/dig_in.c \
../Src/main.c \
../Src/process_time.c \
../Src/sine.c \
../Src/sleep.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/systick.c \
../Src/usart.c 

OBJS += \
./Src/analog_out.o \
./Src/button.o \
./Src/cbfifo.o \
./Src/dig_in.o \
./Src/main.o \
./Src/process_time.o \
./Src/sine.o \
./Src/sleep.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/systick.o \
./Src/usart.o 

C_DEPS += \
./Src/analog_out.d \
./Src/button.d \
./Src/cbfifo.d \
./Src/dig_in.d \
./Src/main.d \
./Src/process_time.d \
./Src/sine.d \
./Src/sleep.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/systick.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DNUCLEO_F091RC -DSTM32F091RCTx -DSTM32F091xC -c -I../Inc -I../CMSIS -O0 -ffunction-sections -fdata-sections -Wall -Werror -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/analog_out.cyclo ./Src/analog_out.d ./Src/analog_out.o ./Src/analog_out.su ./Src/button.cyclo ./Src/button.d ./Src/button.o ./Src/button.su ./Src/cbfifo.cyclo ./Src/cbfifo.d ./Src/cbfifo.o ./Src/cbfifo.su ./Src/dig_in.cyclo ./Src/dig_in.d ./Src/dig_in.o ./Src/dig_in.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/process_time.cyclo ./Src/process_time.d ./Src/process_time.o ./Src/process_time.su ./Src/sine.cyclo ./Src/sine.d ./Src/sine.o ./Src/sine.su ./Src/sleep.cyclo ./Src/sleep.d ./Src/sleep.o ./Src/sleep.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/systick.cyclo ./Src/systick.d ./Src/systick.o ./Src/systick.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

