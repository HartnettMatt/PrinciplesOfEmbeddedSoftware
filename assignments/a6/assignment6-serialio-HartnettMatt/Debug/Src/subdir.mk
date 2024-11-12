################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/cbfifo.c \
../Src/hexdump.c \
../Src/main.c \
../Src/process_cmd.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/test_cbfifo.c \
../Src/uled.c \
../Src/usart.c 

OBJS += \
./Src/cbfifo.o \
./Src/hexdump.o \
./Src/main.o \
./Src/process_cmd.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/test_cbfifo.o \
./Src/uled.o \
./Src/usart.o 

C_DEPS += \
./Src/cbfifo.d \
./Src/hexdump.d \
./Src/main.d \
./Src/process_cmd.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/test_cbfifo.d \
./Src/uled.d \
./Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DNUCLEO_F091RC -DSTM32F091RCTx -DSTM32F091xC -c -I../Inc -I../CMSIS -O0 -ffunction-sections -fdata-sections -Wall -Werror -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/cbfifo.cyclo ./Src/cbfifo.d ./Src/cbfifo.o ./Src/cbfifo.su ./Src/hexdump.cyclo ./Src/hexdump.d ./Src/hexdump.o ./Src/hexdump.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/process_cmd.cyclo ./Src/process_cmd.d ./Src/process_cmd.o ./Src/process_cmd.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/test_cbfifo.cyclo ./Src/test_cbfifo.d ./Src/test_cbfifo.o ./Src/test_cbfifo.su ./Src/uled.cyclo ./Src/uled.d ./Src/uled.o ./Src/uled.su ./Src/usart.cyclo ./Src/usart.d ./Src/usart.o ./Src/usart.su

.PHONY: clean-Src

