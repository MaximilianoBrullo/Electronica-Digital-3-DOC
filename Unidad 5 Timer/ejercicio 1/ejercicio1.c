/*Directivad de inclusion*/
#include "LPC17xx.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"

/*Definicion de constantes y macros*/
#define OUTPUT (uint8_t) 1
#define INPUT (uint8_t) 0
#define PORT_0 (uint8_t) 0
#define PIN_22 ((uint8_t) (1<<22))


/*Definicion de variables globales*/

/*Definicion de funciones*/
void configGPIO(void);
void configTIMER0(void);

/*Funcion principal*/
int main(void)
{
    configGPIO(); /*Configuracion del GPIO*/
    configTimer(); /*Configuracion del TIMER0*/
    while(1); /*Bucle infinito*/
    return 0;
}

void configGPIO(void)
{
    PINSEL_CFG_Type cfgPinLED;//nombro a la estructura P0_22
    PINSEL_CFG_Type cfgPinMAT0;//nombro a la estructura P1_28
    
    cfgPinLED.Portnum = PINSEL_PORT_0;//indicamos puerto
    cfgPinLED.Pinnum = PINSEL_PIN_22;//indicamos pin
    cfgPinLED.Funcnum = PINSEL_FUNC_0;//indicamos funcion
    cfgPinLED.Pinmode = PINSEL_PINMODE_TRISTATE;//indicamos modo(si es entrada)
    cfgPinLED.OpenDrain = PINSEL_PINMODE_NORMAL;//config para el open drain
    
    GPIO_SetDir(PORT_0 ,PIN_22, OUTPUT);//configura el pin como entrada o salida
    
    cfgPinMAT0.Portnum = PINSEL_PORT_1;//indicamos puerto
    cfgPinMAT0.Pinnum = PINSEL_PIN_28;//indicamos pin
    cfgPinMAT0.Funcnum = PINSEL_FUNC_3;//indicamos funcion
    cfgPinMAT0.OpenDrain = PINSEL_PINMODEe_NORMAL;//activa o no el open drain.
    
    PINSEL_ConfigPin(&cfgPinLED);//se configuran los pines de acuerdo a los parametros
    PINSEL_ConfigPin(&cfgPinMAT0);//idem anterior
    return;
}

void configTimer(void){
	TIM_TIMERCFG_Type cfgTimerMode;
	TIM_MATCHCFG_Type cfgTimerMatch;
	
	cfgTimerMode.PrescaleOption = TIM_PRESCALE_USVAL;//configuramos que el valor agregado es en microsegundos
	cfgTimerMode.PrescaleValue = 1000;//valor maximo del prescaler
	
	cfgTimerMatch.MatchChannel = 0;
	cfgTimerMatch.MatchValue = 999;
	cfgTimerMatch.IntOnMatch = ENABLE;
	cfgTimerMatch.ResetOnMatch = ENABLE;
	cfgTimerMatch.StopOnMatch = DISABLE;
	cfgTimerMatch.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &cfgTimerMode);
	TIM_ConfigMatch(LPC_TIM0, &cfgTimerMatch);
	TIM_Cmd(LPC_TIM0, ENABLE);
	
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void TIMER_IRQHandler(void){//llamado a la interrupcion
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MRO_INT)){
		static uint8_t i = 0;
		if(i == 0){
			GPIO_SetValue(PORT_0, PIN_22);//pone en 1 el pin del puerto indicado
			i = 1;
		}else if(i == 1){
			GPIO_ClearValue(PORT_0,PIN_22);//pone en 0 el pin del puerto indicado
			i == 0;
		}
	}
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}