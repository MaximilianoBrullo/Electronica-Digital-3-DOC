#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"

/*Definiciones, variables globales y macros*/
int match0Value = 1000;

/*Definicion de funciones*/
void cfgPinsel();
void cfgTimer();
void cfgInt();

/*Funcion principal*/
int main ()
{
	cfgPinsel();
	cfgTimer();
	while(1);
	return 0;
}

void cfgPinsel()
{
	PINSEL_CFG_Type led = {0};
	led.pinPort = PINSEL_PORT_0;
	led.pinNum = PINSEL_PIN_22;
	led.funNum = PINSEL_FUNC_0;
	led.pinMode = PINSEL_PINMODE_TRISTATE;
	led.openDrain = PINSEL_OD_NORMAL;
	PINSEL_configPin(&led);

    GPIO_SetDir(GPIO_PORT_0, 1<<22, GPIO_OUTPUT);
    return;
}

void cfgTimer()
{
	TIM_TIMERCFG_Type  timer0 = {TIN_USVAL, 1000};
	TIM_Init(LPC_TIM0, T)

    TIM_MATCHCFG_Type match0 = {0};
	match0.matchChannel = TIM_MATCH_CHANNEL_0;
    match0.intOnMatch = ENABLE;
    match0.stopOnMatch = DESABLE;
    match0.resetOnMatch = ENABLE;
    match0.extMatchOutputType = TIM_NOTHING;
    match0.matchValue = match0Value;
    TIM_CofigmMatch(LPC_TIM0, &match0);
    TIM_Cmd(LPC_TIM0, ENABLE);

    NVIC_EnableIRQ(TIMER0_IRQn);
    return;
}

voif cfgInt()
{
    EXTI_CFG_Type eint2 = {0};
    eint2.line = EXTI_EINT2;
    eint2.mode = EXTI_MODE_EDGE_SENSITIVE;
    eint2.polarity = EXTI_RISING_EDGE;
    EXTI_ClearFlag(EXTI_EINT2)
    EXTI_ConfigEnable(&eint2);
}

void TIMER0_IRQHandler(void)
{
    static uint8_t i = 0;
    if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
    {
        if(i=0)
        {
            GPIO_SetPins(GPIO_PORT_0, 1<<22);
            i=1;
        }
        else
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<22);
            i=0;
        }
    }
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    return;
}

void EINT2_IRQHandler(void)
{
    if(EXTI_GetFlag(EXTI_EINT2))
    {
        if(match0Value >= 10)
        {
            match0Value = match0Value / 2;
        }
        else
        {
            match0Value = 1000;
        }
        TIM_UpdateMatchValue(LPC_TIM0, TIM_MATCH_CHANNEL_0, match0Value);
    }
    cfgTimer()
    return;
}