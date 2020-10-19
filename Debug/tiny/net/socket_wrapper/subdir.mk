################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tiny/net/socket_wrapper/tos_at_socket.c \
../tiny/net/socket_wrapper/tos_at_socket_lib.c \
../tiny/net/socket_wrapper/tos_at_socket_prv.c 

OBJS += \
./tiny/net/socket_wrapper/tos_at_socket.o \
./tiny/net/socket_wrapper/tos_at_socket_lib.o \
./tiny/net/socket_wrapper/tos_at_socket_prv.o 

C_DEPS += \
./tiny/net/socket_wrapper/tos_at_socket.d \
./tiny/net/socket_wrapper/tos_at_socket_lib.d \
./tiny/net/socket_wrapper/tos_at_socket_prv.d 


# Each subdirectory must supply rules for building sources it contributes
tiny/net/socket_wrapper/tos_at_socket.o: ../tiny/net/socket_wrapper/tos_at_socket.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"tiny/net/socket_wrapper/tos_at_socket.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
tiny/net/socket_wrapper/tos_at_socket_lib.o: ../tiny/net/socket_wrapper/tos_at_socket_lib.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"tiny/net/socket_wrapper/tos_at_socket_lib.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
tiny/net/socket_wrapper/tos_at_socket_prv.o: ../tiny/net/socket_wrapper/tos_at_socket_prv.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"tiny/net/socket_wrapper/tos_at_socket_prv.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

