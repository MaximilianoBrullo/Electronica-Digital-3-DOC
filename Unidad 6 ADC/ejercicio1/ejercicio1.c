#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"

void cfgPinsel(void);
void cfgTimer(void);
void cfgADC(void);

int main()
{
    cfgPinsel();
    cfgTimer();
    cfgADC();
    while(1){}
    return 0;
}

void cfgPinsel()
{
    PINSEL_CFG_Type pinADC = {0};
    PINSEL_CFG_Type pinMATCH = {0};

    pinADC.portNum = PINSEL_PORT_0;
    pinADC.pinNum = PINSEL_PIN_23;
    pinADC.funcNum = PINSEL_FUNC_1;//ADC0.0
    pinADC.pinMode = PINSEL_TRISTATE;
    pinADC.openDrain = PINSEL_OD_NORMAL;
    
    pinMATCH.portNum = PINSEL_PORT_1;
    pinMATCH.pinNum = PINSEL_PIN_29;
    pinMATCH.funcNum = PINSEL_FUNC_3;//MAT0.1
    pinMATCH.pinMode = PINSEL_TRISTATE;
    pinMATCH.openDrain = PINSEL_OD_NORMAL;
    
    PINSEL_ConfigPin(&pinADC);
    PINSEL_ConfigPin(&pinADC);
    return;
}

void cfgTimer()
{
	TIM_TIMERCFG_Type timer = {0};
	TIM_MATCHCFG_Type match = {0};
	
	timer.prescaleOption = TIM_USVAL;//en uS
	timer.precaleValue = 1000;//1000uS = 1mS
	
	match.matchChannel = TIM_MATCH_CHANNEL_1;//canal de match 1
	match.intOnMatch = DISABLE;//deshabilitada la interrupcion por match
	match.stopOnMatch = DISABLE;//deshabilitada la parada sel timer por match
	match.resetOnMatch = ENABLE;//habilitada el reseteo de la cuenta del timer por match
	match.extMatchOutputType = TIM_TOGGLE;//con el match, toglea la salida p1.29
	match.matchValue = 499;
	
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);//configuro modalidad del timer0
	TIM_ConfigMatch(LPC_TIM0, &match);//configuro timer0 como temporizador
	TIM_Cmd(LPC_TIM, ENABLE);//habilito el timer0
	return;
}

void cfgADC()
{
	ADC_Init(32000);//Inicializo el ADC con frecuencia de muestreo
	ADC_BurstCmd(DISABLE);//deshabilito el modo burst
	ADC_StartCmd(ADC_START_ON_MAT01);//inicializo conversion debido a MAT0.1
	ADC_EdgeStrtConfig(ADC_START_ON_FALLING);//conversion cuando se detecta flanco ascendente
	ADC_ChannelCmd(ADC_CHANNEL_0, ENABLE);//Habilitacion del canal 0
	ADC_IntConfig(ADC_ADINTEN0, ENABLE);//Habilito interrupcion por finalizacion de conversion del canal 0
	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

void ADC_IRQHandler()
{
	while(!ADC_ChannelGetStatus(ADC_CHANNEL_0, ADC_DATA_DONE)){}//ESPERA EL FIN DE LA CONVERSION DEL ADC CANAL0
	uint16_t ADC0Value = ADC_ChannelGetData(ADC_CHANNEL_0);
}