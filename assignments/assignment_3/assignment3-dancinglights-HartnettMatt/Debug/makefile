################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := assignment3-dancinglights-HartnettMatt
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
assignment3-dancinglights-HartnettMatt.elf \

MAP_FILES += \
assignment3-dancinglights-HartnettMatt.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
assignment3-dancinglights-HartnettMatt.list \


# All Target
all: main-build

# Main-build Target
main-build: assignment3-dancinglights-HartnettMatt.elf secondary-outputs

# Tool invocations
assignment3-dancinglights-HartnettMatt.elf assignment3-dancinglights-HartnettMatt.map: $(OBJS) $(USER_OBJS) /home/matt/Documents/Education/Graduate/PrinciplesOfEmbeddedSoftware/assignment3-dancinglights-HartnettMatt/STM32F091RCTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "assignment3-dancinglights-HartnettMatt.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"/home/matt/Documents/Education/Graduate/PrinciplesOfEmbeddedSoftware/assignment3-dancinglights-HartnettMatt/STM32F091RCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="assignment3-dancinglights-HartnettMatt.map" -Wl,--gc-sections -static -L../Lib -Wl,--whole-archive -lstm_startup -Wl,--no-whole-archive --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

assignment3-dancinglights-HartnettMatt.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "assignment3-dancinglights-HartnettMatt.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) assignment3-dancinglights-HartnettMatt.elf assignment3-dancinglights-HartnettMatt.list assignment3-dancinglights-HartnettMatt.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
