/*
 * task.c
 *
 *  Created on: 2017Äê5ÔÂ29ÈÕ
 *      Author: boblane
 */
#include "Mytask.h"
#include "Debug.h"

#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "lwip/lwip/sockets.h"

#include "../include/gpio.h"

#define SERVER_IP   "192.168.1.124"
#define SERVER_PORT  1001

LOCAL void gpio_task(void *pvParameters);
LOCAL void TCP_client_task(void *pvParameters);

void start_task(void)
{
	xTaskCreate(gpio_task, "gpio_task", 256, NULL, 4, NULL);
	xTaskCreate(TCP_client_task, "TCP_client_task", 512, NULL, 5, NULL);
	vTaskStartScheduler();
}

LOCAL int PushCnt = 0;
void gpio_task(void *pvParameters)
{
	INFO("gpio_task start...\r\n");
	for(;;)
	{
		printf("GPIO_Pin_4: %d\r\n",gpio_input_get()&GPIO_Pin_4);
		if( (gpio_input_get()&GPIO_Pin_4) == 0 )
		{
			Sleep_ms(100);
			if( (gpio_input_get()&GPIO_Pin_4) == 0 )
			{
				++PushCnt;
				printf("PushCnt: %d\r\n",PushCnt);
			}
			else
				printf("Hello World!\r\n");

			if(PushCnt == 5)
			{
				break;
			}
		}
		Sleep_ms(500);
	}
	vTaskDelete(NULL);
	printf("delete the gpio_task\r\n");
}

void TCP_client_task(void *pvParameters)
{
	int 				sockfd;
	struct sockaddr_in	remote_ip;
	INFO("TCP_client_task start...\r\n");
	for(;;)
	{
		sockfd = socket(PF_INET, SOCK_STREAM, 0);
		if (-1 == sockfd)
		{
			close(sockfd);
			Sleep_ms(1000);
			INFO("TCP_client_task > socket fail!\n");
			continue;
		}
		printf("TCP_client_task > socket ok!\n");

		bzero(&remote_ip, sizeof(struct sockaddr_in));
		remote_ip.sin_family = AF_INET;
		remote_ip.sin_addr.s_addr = inet_addr(SERVER_IP);
		remote_ip.sin_port = htons(SERVER_PORT);

		if (0 != connect(sockfd, (struct sockaddr *)(&remote_ip), sizeof(struct sockaddr)))
		{
			close(sockfd);
			Sleep_ms(1000);
			printf("TCP_client_task > connect fail!\n");
			continue;
		}
		printf("TCP_client_task > connect ok!\n");

		Sleep_ms(500);
	}
}
