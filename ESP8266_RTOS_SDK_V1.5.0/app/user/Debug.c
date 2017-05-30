/*
 * Debug.c
 *
 *  Created on: 2017Äê5ÔÂ30ÈÕ
 *      Author: boblane
 */

#include "Debug.h"

#include "esp_common.h"
#include "freertos/FreeRTOS.h"

void Sleep_ms(unsigned long _ms)
{
	vTaskDelay( _ms / portTICK_RATE_MS );
}
