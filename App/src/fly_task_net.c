#include "osAbstract.h"
#include "tos_k.h"
#include "cmsis_os.h"
#include "main.h"
#include "iDebug.h"
#include "stdio.h"

#include "sal_module_wrapper.h"
#include "esp8266.h"

#include "fly_task_board.h"
#include "fly_task_net.h"

//这个size需要注意，如果太小，会导致执行频繁，而其它任务被饿死
#define TEST_TASK_STK_SIZE				4095
static void net_main_task(void const *pdata);
osThreadDef(net_main_task, osPriorityNormal, 1, TEST_TASK_STK_SIZE);

#define TCP_TEST_TASK0_STK_SIZE         1024
void tcp_test0(void);
osThreadDef(tcp_test0, osPriorityNormal, 1, TCP_TEST_TASK0_STK_SIZE);

//信号量
static k_sem_t sem_rec;

int socket_id_0 = -1;
static int net_connected = -1;

#define RECV_LEN            1024
uint8_t recv_data_0[RECV_LEN];

/*
 * 消息接收回调
 */
static void espcom_rec_callback(void) {
	if (net_connected != -1) {
		tos_sem_post(&sem_rec);
	}
}

/*
 * 资源初始化
 */
static void res_init(void) {
	if (K_ERR_NONE != tos_sem_create(&sem_rec, 0)) {
		iDebugPrint(DBG_LEV_ERROR, "sem_rec create error\r\n");
	}
}

void tcp_test0(void)
{
	int recv_len = -1;
	k_err_t err;

	while (1) {
		err = tos_sem_pend(&sem_rec, tos_millisec2tick(1*1000));
		if (err != K_ERR_NONE) {
			iDebugPrint(DBG_LEV_WARN, "net_rec may be error :%d\r\n", err);
			//continue;
		}

		recv_len = tos_sal_module_recv_timeout(socket_id_0, recv_data_0, sizeof(recv_data_0), 8000);
		if (recv_len < 0) {
			printf("task0 receive error\n");
		} else if (recv_len == 0) {
			printf("task0 receive none\n");
		} else {
			recv_data_0[recv_len] = 0;
			printf("task0: receive len: %d\nmsg from remote: %s\n", recv_len, recv_data_0);

			parseServerCmd(recv_data_0, recv_len);
		}

	}
}

uint8_t send_buff[] = "GET http://oldsix.top/myServer/server?temperature=234\r\n";
static void net_main_task(void const *arg) {
	int err_count = 0;

	esp8266_sal_init(HAL_UART_PORT_2);
	esp8266_rec_callback_regist(espcom_rec_callback);
    esp8266_join_ap("HW", "00000000");
	
	socket_id_0 = tos_sal_module_connect("111.67.205.50", "80", TOS_SAL_PROTO_TCP);
    if (socket_id_0 == -1) {
        printf("TCP0 connect failed\r\n");
        net_connected = -1;
    } else {
        printf("TCP0 connect success! fd: %d\n", socket_id_0);
        net_connected = 0;
    }

	while(1){
		tos_sleep_ms(2000);
		if(net_connected != -1){
			//建立连接成功，开始发送数据
			net_connected = tos_sal_module_send(socket_id_0, send_buff, strlen((const char *)send_buff));

			if(net_connected == -1) {
				//连接失败，断开重连
				tos_sal_module_close(socket_id_0);
				iDebugPrint(DBG_LEV_ERROR, "net try to force restart\r\n");

				//重新初始化
				//...
			}
		}

		tos_sleep_ms(5000);
	}
}


void fly_task_net_create(void)
{
	iDebugPrint(DBG_LEV_INFO, "net task create\r\n");
	res_init();
	osThreadCreate(osThread(net_main_task), NULL);
	osThreadCreate(osThread(tcp_test0), NULL);
}
