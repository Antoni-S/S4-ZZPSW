#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "semphr.h"
#include "string.h"
#include "uart.h"
#include "keyboard.h"

xSemaphoreHandle xSemaphore;
QueueHandle_t xQueue;

void Rtos_Transmiter_SendString (void *pvParameters) {
	char cMessage[32];
	while(1) {
		if(xQueueReceive(xQueue, cMessage, portMAX_DELAY) == pdPASS) {
			xSemaphoreTake(xSemaphore, portMAX_DELAY);
			Transmiter_SendString(cMessage);
			while (eTransmiter_GetStatus()!=FREE){};
			xSemaphoreGive(xSemaphore);
		}
		vTaskDelay(30);
	}
}

void LettersTx (void *pvParameters){
	char cMessage[32];
	TickType_t TickStart, TickStop, TickLength;
	CopyString("-ABCDEFGH-:", cMessage);
	
	while(1){
		TickStart = xTaskGetTickCount();
		if(xQueueSend(xQueue, cMessage, portMAX_DELAY) != pdPASS)
			Led_Toggle(0);
		xQueueSend(xQueue, cMessage, portMAX_DELAY);
		CopyString("-ABCDEFGH-:", cMessage);
		TickStop = xTaskGetTickCount();
		TickLength = TickStop - TickStart;
		AppendUIntToString(TickLength, cMessage);
		AppendString("\n", cMessage);
		vTaskDelay(1000);
	}
}

void KeyboardTx (void *pvParameters) {
	while(1){
		if(eKeyboardRead() != RELEASED) {
			xQueueSend(xQueue, "-KEYBOARD-\n", portMAX_DELAY);
		}
		vTaskDelay(30);
	}
}

int main( void ){
	UART_InitWithInt(300);
	KeyboardInit();
	vSemaphoreCreateBinary(xSemaphore);
	xSemaphoreGive(xSemaphore);
	xQueue = xQueueCreate(5,32);
	Led_Init();
	
	xTaskCreate(LettersTx, NULL, 128, NULL, 1, NULL );
	xTaskCreate(KeyboardTx, NULL, 128, NULL, 1, NULL);
	xTaskCreate(Rtos_Transmiter_SendString, NULL, 128, NULL, 1, NULL);
	vTaskStartScheduler();
	while(1);
}
