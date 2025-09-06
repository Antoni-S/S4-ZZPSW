#include <lpc21xx.h>
#include "led.h"
void Delay(unsigned int uiMiliSec) {
	unsigned int uiLoopCtr, uiDelayLoopCount;
	uiDelayLoopCount = uiMiliSec*12000;
	for(uiLoopCtr=0;uiLoopCtr<uiDelayLoopCount;uiLoopCtr++) {}
}
int main(void){
	Led_Init();
	while(1){
		Led_Toggle(0);
		Delay(500);
	}

}
