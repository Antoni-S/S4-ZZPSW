#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

void LettersTx (void *pvParameters){
    while(1){
        Transmiter_SendString("-ABCDEFGH-\n");
    while (eTransmiter_GetStatus()!=FREE){};
        vTaskDelay(300);
    }
}

void KeyboardTx (void *pvParameters) {
	while(1){
		if(eKeyboardRead() != RELEASED) {
			Transmiter_SendString("-KEYBOARD-\n");
			while (eTransmiter_GetStatus()!=FREE){};
		}
		vTaskDelay(300);
	}
}

int main( void ){
    UART_InitWithInt(300);
		KeyboardInit();
    xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
		xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1);
}
