#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_exti.h"

/*Definiciones, variables globales y macros*/
uint8_t secuencia = 1;
/*Definicion de funciones*/
void cfgPinsel();
void cfgTimer();
void cfgInt();

int main ()
{
    cfgPinsel();
    cfgTimer();
    cfgInt();
    while(1);
    return 0;
}

cfgPinsel()
{
    PINSEL_CFG_Type salida = {0};
    salida.portNum = PINSEL_PORT_0;
    salida.pinNum = PINSEL_PIN_0;
    salida.funNum = PINSEL_FUNC_0;
    salida.pinMode = PINSEL_PINMODE_TRIESTATE;
    salida.openDrain = PINSEL_OD_NORMAL;
    PINSEL_configPin(&salida);
    salida.portNum = PINSEL_PORT_1;
    PINSEL_configPin(&salida);
    salida.pinNum = PINSEL_PIN_2;
    PINSEL_configPin(&salida);
    salida.pinNum = PINSEL_PIN_3;
    PINSEL_configPin(&salida);

    GPIO_SertDir(GPIO_PORT_0, 0x0F, GPIO_OUTPUT);
    
    PINSEL_CFG_Type entrada = {0};
    entrada.portNum = PINSEL_PORT_2;
    entrada.pinNum = PINSEL_PIN_12;
    entrada.funcNum = PINSEL_FUNC_1;
    entrada.pinMode = PINSEL_PINMODE_TRISTATE;
    entrada.openDrain = PINSEL_OD_NORMAL;
    PINSEL_configPin(&entrada);
    
    return;
}

cfgTimer()
{
    TIM_TIMERCFG_Type timer0 = {TIM_USVAL, 1000};
    TIM_Init(LPC_TIM0, &timer0);
    
    TIM_MATCHCFG_Type match0 = {0};
    match0.matchChannel = TIM_MATCH_CHANNEL_0;
    match0.intOnMatch = ENABLE;
    match0.stopOnMatch = DESABLE;
    match0.resetOnMatch = DESABLE;
    match0.extMatchOutputType = TIM_NOTHING;
    match0.matchValue = 0;
    TIM_ConfigMatch(LPC_TIM0, &match0);

    TIM_MATCHCFG_Type match1 = {0};
    match1.matchChannel = TIM_MATCH_CHANNEL_1;
    match1.intOnMatch = ENABLE;
    match1.stopOnMatch = DESABLE;
    match1.resetOnMatch = DESABLE;
    march1.extMatchOutputType = TIM_NOTGING;
    match1.matchValue = 5;
    TIM_ConfigMatch(LPC_TIM0, &Match1);

    TIM_MATCHCFG_Type match2 = {0};
    match2.matchChannel = TIM_MATCH_CHANNEL_2;
    match2.intOnMatch = ENABLE;
    match2.stopOnMatch = DESABLE;
    match2.resetOnMatch = DESABLE;
    match2.extMatchOutputType = TIM_NOTHING;
    match2.matchValue = 10;
    TIM_ConfigMatch(LPC_TIM0, &match2);

    TIM_MATCHCFG_Type match3 = {0};
    match3.matchChannel = TIM_MATCH_CHANNEL_3;
    match3.intOnMatch = ENABLE;
    match3.stopOnMatch = DESABLE;
    match3.resetOnMatch = ENABLE;
    match3.extMatchOutputType = TIM_NOTHING;
    match3.matchValue = 15;
    TIM_ConfigMatch(LPC_TIM0, &match3);

    TIM_Cmd(LPC_TIM0, ENABLE);
    NVIC_EnableIRQ(TIMER0_IRQn);
    return;
}

void cfgInt()
{
    EXTI_CFG_Type eint2 = {0};
    eint2.line = EXTI_EINT2;
    eint2.mode = EXT_MODE_EDGE_SENSITIVE;
    eint2.polarity = EXTI_RISING_EDGE;
    EXTI_ClearFlag(EXTI_EINT2);
    EXTI_ConfigEnable(&eint2);
    NVIC_SetPriority(EINT2_IRQn, 5);
    return;
}

void TIMER0_IRQHandler(void)
{
    if(secuencia == 1)
    {
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<3);
            GPIO_SetPins(GPIO_PORT_0, 1<<0);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<0);
            GPIO_SetPins(GPIO_PORT_0, 1<<1);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR2_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<1);
            GPIO_SetPins(GPIO_PORT_0, 1<<2);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR3_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<2);
            GPIO_SetPins(GPIO_PORT_0, 1<<3);
        }
    }
    else
    {
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<2);
            GPIO_SetPins(GPIO_PORT_0, 1<<0);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<3);
            GPIO_SetPins(GPIO_PORT_0, 1<<1);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR2_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<0);
            GPIO_SetPins(GPIO_PORT_0, 1<<2);
        }
        if(TIM_GetIntStatus(LPC_TIM0, TIM_MR3_INT))
        {
            GPIO_ClearPins(GPIO_PORT_0, 1<<1)
            GPIO_SetPins(GPIO_PORT_0, 1<<3);
        }
    }
    TIM_ClearPending(LPC_TIM0, TIMMR0_INT);
    TIM_ClearPending(LPC_TIM0, TIMMR1_INT);
    TIM_ClearPending(LPC_TIM0, TIMMR2_INT);
    TIM_ClearPending(LPC_TIM0, TIMMR3_INT);
    return;
}

void EINT2_IRQHandler(void)
{
    if(EXTI_GetFlag(EXTI_EINT2))
    {
        GPIO_ClearPins(GPIO_PORT_0, 0x0F);
        if(secuencia == 1)
        {
            secuencia = 2;
        }
        else
        {
            secuencia = 1;
        }
        EXTI_ClearFlag(EXTI_EINT2);
    }
    return;
}