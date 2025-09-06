#include <LPC21xx.H>
#include "led.h"

#define LED0_bm (1<<16)
#define LED1_bm (1<<17)
#define LED2_bm (1<<18)
#define LED3_bm (1<<19)

void Led_Init(void) {
	IO1DIR = IO1DIR | LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	IO1SET = LED0_bm;
}

void LedOn(unsigned char ucLedIndeks) {
	IO1CLR = LED0_bm | LED1_bm | LED2_bm | LED3_bm;
	switch(ucLedIndeks) {
		case 0:
			IO1SET = LED0_bm;
			break;
		case 1:
			IO1SET = LED1_bm;
			break;
		case 2:
			IO1SET = LED2_bm;
			break;
		case 3:
			IO1SET = LED3_bm;
			break;
	}
}

enum StepDirection {RIGHT, LEFT};

void LedStep(enum StepDirection eLedDirection) {
	static unsigned int uiLedStep;
	switch(eLedDirection) {
		case RIGHT:
			uiLedStep = uiLedStep - 1;
			break;
		case LEFT:
			uiLedStep = uiLedStep + 1;
			break;
	}
	LedOn(uiLedStep % 4);
}

void Led_StepLeft(void) {
	LedStep(LEFT);
}

void Led_StepRight(void) {
	LedStep(RIGHT);
}

void Led_Toggle (unsigned char ucLedIndex) {
	int iLedMask = 0;
		switch(ucLedIndex) {
		case 0:
			iLedMask = LED0_bm;
			break;
		case 1:
			iLedMask = LED1_bm;
			break;
		case 2:
			iLedMask = LED2_bm;
			break;
		case 3:
			iLedMask = LED3_bm;
			break;
	}
	if(IO1PIN & iLedMask) {
		IO1CLR = iLedMask;
	} else {
		IO1SET = iLedMask;
	}
}
