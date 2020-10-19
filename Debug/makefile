################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include tiny/osal/cmsis_os/subdir.mk
-include tiny/net/socket_wrapper/subdir.mk
-include tiny/net/sal_module_wrapper/subdir.mk
-include tiny/net/at/src/subdir.mk
-include tiny/kernel/pm/subdir.mk
-include tiny/kernel/hal/subdir.mk
-include tiny/kernel/evtdrv/subdir.mk
-include tiny/kernel/core/subdir.mk
-include tiny/devices/esp8266/subdir.mk
-include tiny/arch/arm/arm-v7m/cortex-m4/gcc/subdir.mk
-include tiny/arch/arm/arm-v7m/common/subdir.mk
-include Test/src/subdir.mk
-include HalInterfaces/src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include App/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
myOsProject.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
myOsProject.list \

OBJCOPY_BIN += \
myOsProject.bin \


# All Target
all: myOsProject.elf secondary-outputs

# Tool invocations
myOsProject.elf: $(OBJS) $(USER_OBJS) D:\WAREHOUSE\LocalRep\STM32F4_ESP8266_GCC\STM32F407ZGTX_FLASH.ld
	arm-none-eabi-gcc -o "myOsProject.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\WAREHOUSE\LocalRep\STM32F4_ESP8266_GCC\STM32F407ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="myOsProject.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

myOsProject.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "myOsProject.list"
	@echo 'Finished building: $@'
	@echo ' '

myOsProject.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "myOsProject.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
