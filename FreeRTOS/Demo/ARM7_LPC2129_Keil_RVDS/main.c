#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"
#include "keyboard.h"

void Keyboard (void *pvParameters){
	while(1){
		switch(eKeyboardRead()){
			case BUTTON_0:
				ServoCallib();
				break;
			case BUTTON_1:
				ServoGoTo(50);
				break;
			case BUTTON_2:
				ServoGoTo(100);
				break;
			case BUTTON_3:
				ServoGoTo(150);
				break;
			default:
				break;
		}
		vTaskDelay(100);
	}
}

int main( void ){
	ServoInit(100);

	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
