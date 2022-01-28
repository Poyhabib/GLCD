################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
NEW.lss \

FLASH_IMAGE += \
NEW.hex \

EEPROM_IMAGE += \
NEW.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: NEW.elf secondary-outputs

# Tool invocations
NEW.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,NEW.map -mmcu=atmega16 -o "NEW.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

NEW.lss: NEW.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S NEW.elf  >"NEW.lss"
	@echo 'Finished building: $@'
	@echo ' '

NEW.hex: NEW.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex NEW.elf  "NEW.hex"
	@echo 'Finished building: $@'
	@echo ' '

NEW.eep: NEW.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex NEW.elf  "NEW.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: NEW.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega16 NEW.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) NEW.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
