#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"

xTaskHandle xMyHandle;



int main( void )
{
	unsigned char ucBlinkingFreq = 10;
	
	Led_Init();
	
	xTaskCreate(, NULL , 100 , &ucBlinkingFreq, 2, NULL);
	xTaskCreate(, NULL , 100 , &ucBlinkingFreq, 2, &xMyHandle);
	vTaskStartScheduler();
	while(1);
}