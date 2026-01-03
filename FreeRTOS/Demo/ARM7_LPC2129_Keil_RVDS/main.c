#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

xSemaphoreHandle xSemaphore;

void Rtos_Transmiter_SendString (char cMessage[]) {
	xSemaphoreTake(xSemaphore, portMAX_DELAY);
	Transmiter_SendString(cMessage);
	while (eTransmiter_GetStatus()!=FREE){};
	xSemaphoreGive(xSemaphore);
}

void LettersTx (void *pvParameters){
	while(1){
		Rtos_Transmiter_SendString("-ABCDEFGH-\n");
		vTaskDelay(300);
	}
}

void KeyboardTx (void *pvParameters) {
	while(1){
		if(eKeyboardRead() != RELEASED) {
			Rtos_Transmiter_SendString("-KEYBOARD-\n");
		}
		vTaskDelay(300);
	}
}

int main( void ){
	UART_InitWithInt(300);
	KeyboardInit();
	vSemaphoreCreateBinary(xSemaphore);
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
