/*
 * ejercicio3Timer.c
 *
 *  Created on: 26 sep. 2025
 *      Author: maxib
 */

/*Directivas de inclusion*/
#include "LPC17xx.h"
#include "LPC17xx_pinsel.h"
#include "LPC17xx_timer.h"

/*Macros y definiciones*/

/*Variables globales*/
static uint8_t counter = 0;

/*Definicion de funciones*/
void cfgPinsel(void);
void cfgTimer());

/*Funcion principal*/
int main(void){
	
	cfgPinsel();
	cfgTimer(1000);
	while(1);
	return 0;
}

void cfgTimer(int prescaler){
	TIM_TIMERCFG_Type cfgTimer2
	TIM_MATCHCFG_Type cfgMatch1
	
	cfgTimer2.PrescaleOptcion = TIM_PRESCALE_USVAL;
	cfgTimer2.PrecaleValue = prescaler;//valor de tiempo en el que el preescaler desborda
	
	cfgMatch1.MatchChannel = 1;
	cfgMarch1.MatchValue = 999;
	cfgMatch1.IntOnMatch = ENABLE;
	cfgMatch1.ResetOnMatch = ENABLE;
	cfgMatch1.StopOnMatch =DISABLE;
	cfgMatch1.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	
	TIM_Init(LPC_TIM2, TIM_TIMER_MODE,&cfgTimer2);
	TIM_ConfigMatch(LPC_TIM2, &cfgMatch1);
	TIM_Cmd(LPC_TIM2,ENABLE);
	
	NVIC_EnableIRQ(TIMER2_IRQn);
	return;
}

void cfgPinsel(){
	PINSEL_CFG_Type led;
	PINSEL_CFG_Type pulsador;
	
	led.Portnum = PINSEL_PORT_0;
	led.Pinnum = PINSEL_PIN_22;
	led.Funcnum = PINSEL_PIN_0;
	led.Pinmode = PINSEL_PINMODE_TRISTATE;
	led.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	GPIO_SetDir(PORT_0, PIN_22, OUTPUT);
	
	pulsador.Portnum = PINSEL_PORT_0;
	pulsador.Pinnum = PINSEL_PIN_7;
	pulsador.Funcnum = PINSEL_FUNC_3;
	pulsador.PinMode = PINSEL_PINMODE_PULLDOWN;
	pulsador.OpenDrain = PINSEL_PINMODE_NORMAL;
	
	GPIO_SetDir(PORT_0, PIN_7, INPUT);
	
	PINSEL_ConfigPin(led);
	PINSEL_ConfigPin(pulsador);
	return;
}

void TIMER_IRQHandler(void){
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)){
		GPIO_TogglePins(GPIO_PORT_0, (0x1 << 22));
	}
	return;
}

void INT_IRQHandler(void){
	if(counter <= 0){
		cfgTimer(1000);
	}else{
		cfgTimer(counter / 2);
	}
    return
}