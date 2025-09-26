/*
 * ejercicioTimer2.c
 *
 *  Created on: 25 sep. 2025
 *      Author: maxib
 *	Calcular
 */

/*
 * Escribir el codigo que configure el timer0 paracumplic con la especificaciones
 * dadas. Considerar la freciencias del core en 100Mhz y una division de reloj de
 * 2.
 * Especificaciones:
 * PR=2
 * MRx=6
 * interrupcion y reset activados
 * */

/*Directivas de inclusion*/
#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"

/*Definiciones y macros*/


/*Definicion de funciones*/
void configTimer(void);
void configPinsel(void);

/*Funcion principal*/
int main(void){
	configPinsel();
	configTimer();
	return 0;
}

void configPinsel(void){
	PINSEL_CFG_Type pinselTimerCfg;
	
	pinselTimerCfg.portNum = PINSEL_PORT_0;
	pinselTimerCfg.pinNum = PINSEL_PIN_28;
	pinselTimerCfg.funcNum = PINSEL_FUNC_3;//la cuncion 3 corresponde al MAT0.0
	pinselTimerCfg.pinMode = PINSEL_PULLUP;
	pinselTimerCfg.openDrain = PINSEL_OD_NORMAL;
	
	PINSEL_ConfigPin(&pinselTimerCfg);
	return;
}

void configTimer(void){
	TIM_TIMERCFG_Type configTimer0;
	TIM_MATCHCFG_Type configMatch;
	
	configTimer0.prescaleOption = TIM_USVAL;
	configTimer0.prescaleValue = 2;
	
	configMatch.matchChannel = TIM_MATCH_CHANNEL_0;
	configMatch.intOnMatch = ENABLE;
	configMatch.stopOnMatch = DISABLE;
	configMatch.resetOnMatch = ENABLE;
	configMatch.extMatchOutputType = TIM_EXTMATCH_TOGGLE;
	
	TIM_Init(LPC_TIM0, TIME_TIMER_MODE, &configTimer0);
	TIM_ConfigMatch(LPC_TIM0, &configMatch);
	TIM_Cmd(LPC_TIM0, ENABLE);
	
	NVIC_EnableIRQ(TIMER0_IRQn);
}