################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.c 

S_UPPER_SRCS += \
../tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_s.S 

OBJS += \
./tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.o \
./tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_s.o 

C_DEPS += \
./tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.d 


# Each subdirectory must supply rules for building sources it contributes
tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.o: ../tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"tiny/arch/arm/arm-v7m/cortex-m4/gcc/port_c.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
tiny/arch/arm/arm-v7m/cortex-m4/gcc/%.o: ../tiny/arch/arm/arm-v7m/cortex-m4/gcc/%.S
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

