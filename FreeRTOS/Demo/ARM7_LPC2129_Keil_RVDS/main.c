#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"
#include "keyboard.h"

void Keyboard (void *pvParameters){
	while(1){
		unsigned char i = 1;
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
				for(i = 1; i <= 3; i++) {
					ServoGoTo(12*i);
					ServoWait(100*i);
					ServoGoTo(0);
				}
				break;
			default:
				break;
		}
		vTaskDelay(100);
	}
}

int main( void ){
	ServoInit(200);

	xTaskCreate(Keyboard, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
