#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

xTaskHandle xMyHandle;
xSemaphoreHandle xSemaphore;

void PulseTrigger( void *pvParameters ){
	while(1) {
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000);
	}
}

void Pulse_LED0 (void *pvParameters) {
	while(1) {
		if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
			Led_Set(0);
			vTaskDelay(100);
			Led_Clr(0);
		}
	}
}

void PulseTrigger_delay (void *pvParameters) {
	vTaskDelay(1000/3);
	while(1) {
		xSemaphoreGive(xSemaphore);
		vTaskDelay(1000/3);
	}
}

int main( void )
{
	Led_Init();
	vSemaphoreCreateBinary(xSemaphore);
	
	xTaskCreate(PulseTrigger, NULL , 100 , NULL, 2, NULL);
	xTaskCreate(Pulse_LED0, NULL , 100 , NULL, 2, NULL);
	xTaskCreate(PulseTrigger_delay, NULL , 100 , NULL, 2, NULL);
	vTaskStartScheduler();
	while(1);
}