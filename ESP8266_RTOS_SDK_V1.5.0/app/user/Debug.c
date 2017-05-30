/*
 * Debug.c
 *
 *  Created on: 2017��5��30��
 *      Author: boblane
 */

#include "Debug.h"

#include "esp_common.h"
#include "freertos/FreeRTOS.h"

void Sleep_ms(unsigned long _ms)
{
	vTaskDelay( _ms / portTICK_RATE_MS );
}
