################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HalInterfaces/src/iCharArrayBuffer.c \
../HalInterfaces/src/iCom.c \
../HalInterfaces/src/iDebug.c \
../HalInterfaces/src/osAbstract.c \
../HalInterfaces/src/tPool.c \
../HalInterfaces/src/tinyprintf.c 

OBJS += \
./HalInterfaces/src/iCharArrayBuffer.o \
./HalInterfaces/src/iCom.o \
./HalInterfaces/src/iDebug.o \
./HalInterfaces/src/osAbstract.o \
./HalInterfaces/src/tPool.o \
./HalInterfaces/src/tinyprintf.o 

C_DEPS += \
./HalInterfaces/src/iCharArrayBuffer.d \
./HalInterfaces/src/iCom.d \
./HalInterfaces/src/iDebug.d \
./HalInterfaces/src/osAbstract.d \
./HalInterfaces/src/tPool.d \
./HalInterfaces/src/tinyprintf.d 


# Each subdirectory must supply rules for building sources it contributes
HalInterfaces/src/iCharArrayBuffer.o: ../HalInterfaces/src/iCharArrayBuffer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/iCharArrayBuffer.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
HalInterfaces/src/iCom.o: ../HalInterfaces/src/iCom.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/iCom.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
HalInterfaces/src/iDebug.o: ../HalInterfaces/src/iDebug.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/iDebug.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
HalInterfaces/src/osAbstract.o: ../HalInterfaces/src/osAbstract.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/osAbstract.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
HalInterfaces/src/tPool.o: ../HalInterfaces/src/tPool.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/tPool.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
HalInterfaces/src/tinyprintf.o: ../HalInterfaces/src/tinyprintf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx '-DLOG_LEVEL=DBG_LEV_VERBOSE' -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../App/inc -I../HalInterfaces/inc -I../Test/inc -I../tiny/arch/arm/arm-v7m/common/include -I../tiny/arch/arm/arm-v7m/cortex-m4/gcc -I../tiny/devices/esp8266 -I../tiny/net/at/include -I../tiny/net/sal_module_wrapper -I../tiny/net/socket_wrapper/include -I../tiny/osal/cmsis_os -I../tiny/kernel/core/include -I../tiny/kernel/evtdrv/include -I../tiny/kernel/hal/include -I../tiny/kernel/pm/include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"HalInterfaces/src/tinyprintf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

