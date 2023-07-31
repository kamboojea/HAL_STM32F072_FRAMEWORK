################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Amscreen/Framework/Libs/Src/subdir.mk
-include Amscreen/Framework/Drivers/Src/subdir.mk
-include Amscreen/Aplication/Tasks/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := hal_stm32f072cbt6_template
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
hal_stm32f072cbt6_template.elf \

MAP_FILES += \
hal_stm32f072cbt6_template.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
hal_stm32f072cbt6_template.list \

OBJCOPY_HEX += \
hal_stm32f072cbt6_template.hex \

OBJCOPY_BIN += \
hal_stm32f072cbt6_template.bin \


# All Target
all: main-build

# Main-build Target
main-build: hal_stm32f072cbt6_template.elf secondary-outputs

# Tool invocations
hal_stm32f072cbt6_template.elf hal_stm32f072cbt6_template.map: $(OBJS) $(USER_OBJS) C:\Projects\Firmwares\Signs\wafer\V2\HAL_template\HAL_STM32F072_FRAMEWORK\hal_stm32f072cbt6_template\STM32F072CBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "hal_stm32f072cbt6_template.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Projects\Firmwares\Signs\wafer\V2\HAL_template\HAL_STM32F072_FRAMEWORK\hal_stm32f072cbt6_template\STM32F072CBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="hal_stm32f072cbt6_template.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

hal_stm32f072cbt6_template.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "hal_stm32f072cbt6_template.list"
	@echo 'Finished building: $@'
	@echo ' '

hal_stm32f072cbt6_template.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "hal_stm32f072cbt6_template.hex"
	@echo 'Finished building: $@'
	@echo ' '

hal_stm32f072cbt6_template.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "hal_stm32f072cbt6_template.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout hal_stm32f072cbt6_template.bin hal_stm32f072cbt6_template.elf hal_stm32f072cbt6_template.hex hal_stm32f072cbt6_template.list hal_stm32f072cbt6_template.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
