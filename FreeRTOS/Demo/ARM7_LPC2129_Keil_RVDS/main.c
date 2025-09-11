#include "FreeRTOS.h"
#include "task.h"
#include "led.h"



typedef struct {
	unsigned char ucBlinkingFreq;
	unsigned char ucActiveLed;
} sCtrlLed;

sCtrlLed myCtrlLed;

void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++){}
}

void LedBlink( void *pvParameters ){
	sCtrlLed *myLedParam = (sCtrlLed*)pvParameters;
	while(1){
		Led_Toggle(myLedParam -> ucActiveLed);
		vTaskDelay((1000/myLedParam -> ucBlinkingFreq)/2);
	}
}

void LedCtrl (void *pvParameters) {
	unsigned char ucSecondCounter = 0;
	sCtrlLed *myLedParam = (sCtrlLed*)pvParameters;
	while(1) {
		if(ucSecondCounter == 2) {
			myLedParam -> ucActiveLed == 4 ? (myLedParam -> ucActiveLed = 0) : myLedParam -> ucActiveLed++;
			ucSecondCounter = 0;
		} else {
			ucSecondCounter++;
		}
		if(myLedParam -> ucBlinkingFreq == 10) {
			myLedParam -> ucBlinkingFreq = 1;
		} else {
			myLedParam -> ucBlinkingFreq++;
		}
		vTaskDelay(1000);
	}
}

int main( void )
{
	Led_Init();
	myCtrlLed.ucActiveLed = 0;
	myCtrlLed.ucBlinkingFreq = 1;
	xTaskCreate(LedCtrl, NULL, 100, &myCtrlLed, 2, NULL);
	xTaskCreate(LedBlink, NULL , 100 , &myCtrlLed, 2 , NULL );
	vTaskStartScheduler();
	while(1);
}
