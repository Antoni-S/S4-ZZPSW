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

void Pulse_LED (void *pvParameters) {
	while(1) {
		if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
			Led_Set(*(unsigned char*)pvParameters);
			vTaskDelay(100);
			Led_Clr(*(unsigned char*)pvParameters);
		}
	}
}

int main( void )
{
	unsigned char ucLed0 = 0;
	unsigned char ucLed1 = 1;
	
	Led_Init();
	vSemaphoreCreateBinary(xSemaphore);
	
	xTaskCreate(PulseTrigger, NULL , 100 , NULL, 2, NULL);
	xTaskCreate(Pulse_LED, NULL , 100 , &ucLed0, 2, NULL);
	xTaskCreate(Pulse_LED, NULL , 100 , &ucLed1, 2, NULL);
	vTaskStartScheduler();
	while(1);
}
