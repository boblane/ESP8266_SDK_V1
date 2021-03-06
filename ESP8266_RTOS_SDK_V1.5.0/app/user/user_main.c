/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_common.h"
#include "../include/gpio.h"
#include "Debug.h"
#include "Mytask.h"

#define DEMO_AP_SSID 		"MYWIFI"
#define DEMO_AP_PASSWORD 	"1234567890"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
 *******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
	flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
	case FLASH_SIZE_4M_MAP_256_256:
		rf_cal_sec = 128 - 5;
		break;

	case FLASH_SIZE_8M_MAP_512_512:
		rf_cal_sec = 256 - 5;
		break;

	case FLASH_SIZE_16M_MAP_512_512:
	case FLASH_SIZE_16M_MAP_1024_1024:
		rf_cal_sec = 512 - 5;
		break;

	case FLASH_SIZE_32M_MAP_512_512:
	case FLASH_SIZE_32M_MAP_1024_1024:
		rf_cal_sec = 1024 - 5;
		break;

	default:
		rf_cal_sec = 0;
		break;
	}

	return rf_cal_sec;
}


void wifi_init(void)
{
	wifi_set_opmode(SOFTAP_MODE);
	struct softap_config *config = (struct softap_config *)zalloc(sizeof(struct softap_config));   // initialization
	if(config == NULL)
	{
		INFO("alloc softap_config failed!\r\n");
		return;
	}
	if(wifi_softap_get_config_default(config)) // Get soft-AP config first.
	{
		if((strcmp(config->ssid,DEMO_AP_SSID) != 0)||
				(strcmp(config->password, DEMO_AP_PASSWORD)!=0))
		{
			sprintf(config->ssid, DEMO_AP_SSID);
			sprintf(config->password, DEMO_AP_PASSWORD);
			config->authmode = AUTH_WPA_WPA2_PSK;
			config->ssid_len = 0; // or its actual SSID length
			config->max_connection = 4;
			wifi_softap_set_config(config); // Set ESP8266 soft-AP config
		}
		else
			INFO("wifi_softap already set!\r\n");
	}
	else
		INFO("wifi_softap_get_config_default failed!\r\n");

	free(config);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/
void user_init(void)
{
	LOCAL int PushCnt = 0;
	uart_init_new();
	os_printf("Ai-Thinker Technology Co. Ltd.\r\n%s %s\r\n", __DATE__, __TIME__);
	os_printf("Hello,World!\r\n");
	os_printf("SDK version:%s\n", system_get_sdk_version());
	os_printf("init...\r\n");
	gpio_init();
	wifi_init();
	os_printf("init done\r\n");

	start_task();
}

