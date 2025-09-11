#include "FreeRTOS.h"
#include "task.h"
#include "led.h"

unsigned char ucBlinkingFreq = 1;

void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++){}
}

void LedBlink( void *pvParameters ){
	while(1){
		Led_Toggle(0);
		vTaskDelay((1000/ucBlinkingFreq)/2);
	}
}

void LedCtrl (void *pvParameters) {
	while(1) {
		if(ucBlinkingFreq == 10) {
			ucBlinkingFreq = 1;
		} else {
			ucBlinkingFreq++;
		}
		vTaskDelay(1000);
	}
}

int main( void )
{
	Led_Init();
	xTaskCreate(LedCtrl, NULL, 100, &ucBlinkingFreq, 2, NULL);
	xTaskCreate(LedBlink, NULL , 100 , &ucBlinkingFreq, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}
