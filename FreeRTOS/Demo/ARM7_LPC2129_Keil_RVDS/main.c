#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

xTaskHandle xMyHandle;

void LedBlink( void *pvParameters ){
	unsigned char ucFreq = *((unsigned char*)pvParameters);
	while(1){
		Led_Toggle(0);
		vTaskDelay((1000/ucFreq)/2);
	}
}

void LedCtrl( void *pvParameters ) {
	unsigned char ucTaskStatus = 0;
	while(1) {
		if(ucTaskStatus) {
			vTaskSuspend(xMyHandle);
			ucTaskStatus = 0;
		} else {
			vTaskResume(xMyHandle);
			ucTaskStatus = 1;
		}
		vTaskDelay(1000);
	}
}

int main( void )
{
	unsigned char ucBlinkingFreq = 10;
	
	Led_Init();
	
	xTaskCreate(LedCtrl, NULL , 100 , &ucBlinkingFreq, 2, &xMyHandle);
	xTaskCreate(LedBlink, NULL , 100 , &ucBlinkingFreq, 2, &xMyHandle);
	vTaskStartScheduler();
	while(1);
}