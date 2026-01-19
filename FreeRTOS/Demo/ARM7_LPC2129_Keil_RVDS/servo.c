#include "servo.h"
#include "led.h"
#include "keyboard.h"
#include <LPC21xx.H>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define DETECTOR_bm (1<<10)

enum ServoInstr {GOTO, WAIT, SPEED, CAL};
enum ServoState {CALLIB, IDLE, IN_PROGRESS};
static unsigned char ucDelay;
QueueHandle_t xQueue;

struct Servo{
  enum ServoState eState;
  unsigned int uiCurrentPosition;
  unsigned int uiDesiredPosition;
};

struct Instruction {
	enum ServoInstr eInstr;
	unsigned int uiInstrValue;
};

struct Servo sServo;

void DetectorInit(void){
  IO0DIR = IO0DIR&(~(DETECTOR_bm));
}


enum DetectorState {ACTIVE, INACTIVE};

enum DetectorState eReadDetector(){
  if((IO0PIN&DETECTOR_bm) == 0){
    return ACTIVE;
  }
  else{
    return INACTIVE;
  }
}

void Automat(void){
	struct Instruction sControl;
	
	switch(sServo.eState){
		
		case CALLIB:
			
			if(eReadDetector() == INACTIVE){
				LedStepLeft();
			}
			else{
				sServo.uiCurrentPosition = 0;
				sServo.eState = IDLE;
			}
			break;
			
		case IDLE:
			if(xQueueReceive(xQueue, &sControl, 0) == pdPASS) {
				switch(sControl.eInstr) {
					case GOTO:
						sServo.uiDesiredPosition = sControl.uiInstrValue;
						sServo.eState = IN_PROGRESS;
						break;
					
					case WAIT:
						vTaskDelay(sControl.uiInstrValue);
						break;
					
					case SPEED:
						ucDelay = sControl.uiInstrValue;
						break;
					
					case CAL:
						sServo.eState = CALLIB;
						break;
					
					default:
						break;
				}
			}
			break;

		case IN_PROGRESS:
			
			if(sServo.uiCurrentPosition < sServo.uiDesiredPosition){
				
				LedStepRight();
				sServo.uiCurrentPosition++;
				
			}
			else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
				
				LedStepLeft();
				sServo.uiCurrentPosition--;
				
			}
			else{
				sServo.eState = IDLE;
			}
			break;
	}
}
		
void vServoTask(void *pvParameters) {
	ucDelay = 1000/(*(unsigned char*)pvParameters);
	while(1) {
		Automat();
		vTaskDelay(ucDelay);
	}
}
    
    
void ServoInit(unsigned char ucServoFrequency){
  static unsigned char ucFrequency;
	ucFrequency = ucServoFrequency;
	
	LedInit();
	DetectorInit();
  sServo.eState = CALLIB;
	
	xQueue = xQueueCreate(5, sizeof(struct Instruction));
	
	xTaskCreate(vServoTask, NULL, 128, &ucFrequency, 2, NULL);
}

void ServoWait(unsigned int uiTicksToWait) {
	struct Instruction sControl;
	sControl.eInstr = WAIT;
	sControl.uiInstrValue = uiTicksToWait;
	xQueueSend(xQueue, &sControl, portMAX_DELAY);
};

void ServoSpeed(unsigned int uiSpeed) {
	struct Instruction sControl;
	sControl.eInstr = SPEED;
	sControl.uiInstrValue = uiSpeed;
	xQueueSend(xQueue, &sControl, portMAX_DELAY);
};
    
void ServoGoTo(unsigned int uiPosition){
	struct Instruction sControl;
	sControl.eInstr = GOTO;
	sControl.uiInstrValue = uiPosition;
  xQueueSend(xQueue, &sControl, portMAX_DELAY);
}

void ServoCallib(void){
	struct Instruction sControl;
	sControl.eInstr = CAL;
  xQueueSend(xQueue, &sControl, portMAX_DELAY);
}
