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
				ServoGoTo(30);
				break;
			case BUTTON_2:
				ServoGoTo(60);
				break;
			case BUTTON_3:
				ServoSpeed(30);
				ServoGoTo(12);
			
				ServoSpeed(20);
				ServoGoTo(24);
			
				ServoSpeed(15);
				ServoGoTo(36);
			
				ServoSpeed(6);
				ServoGoTo(0);
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
