#include "tos_k.h"
#include "tos_hal.h"
#include "stm32f4xx_hal.h"
#include "usart.h"

#if 0
__API__ int tos_hal_uart_init(hal_uart_t *uart, hal_uart_port_t port)
{
    if (!uart) {
        return -1;
    }

    if (port == HAL_UART_PORT_1) {
        uart->private_uart = &huart1;
        MX_USART1_UART_Init();
    } else if (port == HAL_UART_PORT_2) {
        uart->private_uart = &huart2;
        MX_USART2_UART_Init();
    } else if (port == HAL_UART_PORT_3) {
        uart->private_uart = &huart3;
        MX_USART3_UART_Init();
    }
    return 0;
}

__API__ int tos_hal_uart_write(hal_uart_t *uart, const uint8_t *buf, size_t size, uint32_t timeout)
{
    UART_HandleTypeDef *uart_handle;

    if (!uart || !buf) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;
    (void)HAL_UART_Transmit(uart_handle, (uint8_t *)buf, size, timeout);
    return 0;
}

__API__ int tos_hal_uart_read(hal_uart_t *uart, const uint8_t *buf, size_t size, uint32_t timeout)
{
    HAL_StatusTypeDef hal_status;
    UART_HandleTypeDef *uart_handle;

    if (!uart || !buf) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;

    hal_status = HAL_UART_Receive(uart_handle, (uint8_t *)buf, size, timeout);
    if (hal_status != HAL_OK) {
        return -1;
    }
    return 0;
}

__API__ int tos_hal_uart_deinit(hal_uart_t *uart)
{
    HAL_StatusTypeDef hal_status;
    UART_HandleTypeDef *uart_handle;

    if (!uart) {
        return -1;
    }

    if (!uart->private_uart) {
        return -1;
    }

    uart_handle = (UART_HandleTypeDef *)uart->private_uart;

    hal_status = HAL_UART_DeInit(uart_handle);
    HAL_UART_MspDeInit(uart_handle);

    if (hal_status != HAL_OK) {
        return -1;
    }

    return 0;
}
#else
#include "iCom.h"
#include "tos_at.h"

iComObjectDef comSim;

static void comSimGotData(void);

__API__ int tos_hal_uart_init(hal_uart_t *uart, hal_uart_port_t port) {
	if (uart == NULL)
		return -1;
	if (port == HAL_UART_PORT_2) {
		comSim.bitrate = COM_BITRATE_115200;
		comSim.bufferLength = 2048;
		comSim.com = COM_1;
		comSim.priority = 0;
//		comSim.xferMode = COM_XFER_STANDARD;
//		comSim.xferMode = COM_XFER_INTERRUPT;
		comSim.xferMode = COM_XFER_DMA;
		comSim.GotFrameFromIrq = comSimGotData;
//		comSim.GotFrameFromIrq = NULL;
		if (ICOM_SUCCESS != IComConstruct(&comSim)) {
			return -1;
		}
		uart->private_uart = &comSim;
		IComStart(&comSim);
		return 0;
	}

	return -1;
}

__API__ int tos_hal_uart_write(hal_uart_t *uart, const uint8_t *buf,
		size_t size, uint32_t timeout) {
	int res = -1;
	if (uart == NULL)
		return res;
	if (uart->port == HAL_UART_PORT_0) {
		if (ICOM_SUCCESS != IComTransmit(&comSim, buf, size)) {
			res = -1;
		}
		res = size;
		return size;
	}

	return 0;
}

__API__ int tos_hal_uart_read(hal_uart_t *uart, const uint8_t *buf, size_t size,
		uint32_t timeout) {

	return 0;
}

__API__ int tos_hal_uart_deinit(hal_uart_t *uart) {
	if (uart == NULL)
		return -1;
	if (ICOM_SUCCESS != IComDestory(uart->private_uart)) {
		return -1;
	}

	return 0;
}

/**
 * 从sim模块收到数据
 */
static void comSimGotData(void) {
	uint8 data;

	while(!IComIsEmpty(&comSim))
	{
		if (IComGetData(&comSim, &data, 1) > 0) {
			tos_at_uart_input_byte(data);
		}
	}
}

void comSimReStart(void){
	IComStart(&comSim);
}

#endif
