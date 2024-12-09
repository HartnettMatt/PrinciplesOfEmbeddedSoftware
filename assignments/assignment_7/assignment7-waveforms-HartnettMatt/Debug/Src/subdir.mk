################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/analog_in.c \
../Src/analog_out.c \
../Src/analysis.c \
../Src/autocorrelate.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/test_sine.c 

OBJS += \
./Src/analog_in.o \
./Src/analog_out.o \
./Src/analysis.o \
./Src/autocorrelate.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/test_sine.o 

C_DEPS += \
./Src/analog_in.d \
./Src/analog_out.d \
./Src/analysis.d \
./Src/autocorrelate.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/test_sine.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DNUCLEO_F091RC -DSTM32F091RCTx -DSTM32F091xC -c -I../Inc -I../CMSIS -O0 -ffunction-sections -fdata-sections -Wall -Werror -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/analog_in.cyclo ./Src/analog_in.d ./Src/analog_in.o ./Src/analog_in.su ./Src/analog_out.cyclo ./Src/analog_out.d ./Src/analog_out.o ./Src/analog_out.su ./Src/analysis.cyclo ./Src/analysis.d ./Src/analysis.o ./Src/analysis.su ./Src/autocorrelate.cyclo ./Src/autocorrelate.d ./Src/autocorrelate.o ./Src/autocorrelate.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/test_sine.cyclo ./Src/test_sine.d ./Src/test_sine.o ./Src/test_sine.su

.PHONY: clean-Src

