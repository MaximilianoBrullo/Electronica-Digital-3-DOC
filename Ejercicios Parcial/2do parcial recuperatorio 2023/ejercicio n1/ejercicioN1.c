#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"

uint16_t valorADC = 0;
uint16_t valorPWM = 0;

void cfgPinsel(void);
void cfgGPIO(void);
void cfgTimer0(void);
void cfgTimer1(void);
void cfgADC(void);

int main(void) {
    SystemInit();
    cfgPinsel();
    cfgGPIO();
    cfgTimer0();
    cfgTimer1();
    cfgADC();

    while (1) {
    }
}

void cfgPinsel(void) {
    PINSEL_CFG_Type pinCfg = {0};
    pinCfg.portNum = PINSEL_PORT_0;
    pinCfg.pinNum = PINSEL_PIN_10;
    pinCfg.funcNum = PINSEL_FUNC_1;
    pinCfg.pinMode = PINSEL_PINMODE_TRISTATE;
    pinCfg.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&pinCfg);

    GPIO_SetDir(GPIO_PORT_0, (1<<10), GPIO_DIR_OUTPUT);
    return;
}

void cfgTimer0(void){
    TIM_TIMERCFG_Type timerCfg = {0};
    timerCfg.prescaleOption = TIM_USVAL;
    timerCfg.prescaleValue = 1000; // 1mS

    TIM_MATCHCFG_Type matchCfg = {0};
    matchCfg.matchChannel = TIM_MATCH_CHANNEL_0
    matchCfg.matchValue = 30000; // 30mS
    matchCfg.intOnMatch = ENABLE;
    matchCfg.resetOnMatch = DESABLE;
    matchCfg.StopOnMatch = DISABLE;
    matchCfg.extMatchOutputType = TIM_EXTMATCH_NOTHING;

    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timerCfg);
    TIM_ConfigMatch(LPC_TIM0, &matchCfg);

    matchCfg.matchChannel = TIM_MATCH_CHANNEL_1;
    matchCfg.matchValue = 120000; // 120S
    matchCfg.intOnMatch = ENABLE;
    matchCfg.resetOnMatch = ENABLE;
    matchCfg.StopOnMatch = DISABLE;
    matchCfg.extMatchOutputType = TIM_EXTMATCH_NOTHING;

    TIM_ConfigMatch(LPC_TIM0, &matchCfg);


    TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
    NVIC_EnableIRQ(TIMER0_IRQn);
    TIM_Cmd(LPC_TIM0, ENABLE);
    return;
}

void cfgTimer1(void){
    TIM_TIMERCFG_Type timerCfg = {0};
    timerCfg.prescaleOption = TIM_USVAL;
    timerCfg.prescaleValue = 1; // 1uS
    TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &timerCfg);
    
    TIM_MATCHCFG_Type matchCfg = {0};
    matchCfg.matchChannel = TIM_MATCH_CHANNEL_0;
    matchCfg.matchValue = 50; // 50uS
    matchCfg.intOnMatch = ENABLE;
    matchCfg.resetOnMatch = ENABLE;
    matchCfg.StopOnMatch = DISABLE;
    matchCfg.extMatchOutputType = TIM_EXTMATCH_NOTHING;
    TIM_ConfigMatch(LPC_TIM1, &matchCfg);

    TIM_MATCHCFG_Type matchCfgVarieble = {0};
    matchCfgVarieble.matchChannel = TIM_MATCH_CHANNEL_1;
    matchCfgVarieble.matchValue = 25; // 25uS
    matchCfgVarieble.intOnMatch = ENABLE;
    matchCfgVariable.resetOnMatch = DESABLE;
    matchCfgVariable.StopOnMatch = DISABLE;
    matchCfgVariable.extMatchOutputType = TIM_EXTMATCH_NOTHING;
    TIM_ConfigMatch(LPC_TIM1, &matchCfgVarieble);
    
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
    NVIC_EnableIRQ(TIMER1_IRQn);
    TIM_Cmd(LPC_TIM1, DESABLE);

    return;
}
void cfgGPIO(void){
    return;
}

void cfgADC(void){
    ADC_Init(200000); // 200KHz
    ADC_ChannelCmd(ADC_CHANNEL_0, ENABLE);// CHANNEL 0
    ADC_PinConfig(ADC_CHANNEL_0, ENABLE); // P0.23
    ADC_BurstCmd(DESABLE);
    ADC_StartCmd(ADC_START_MAT01, ENABLE); // Start on MAT0.1
    return;
}

void TIMER0_IRQHandler(void){
    if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT) == SET && TIM_GetIntEstatus(LPC_TIM0, TIM_MR1_INT) == RESET){
        TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
        valorADC =+ ADC_ChannelGetData(ADC_CHANNEL_0);
    }else if(TIM_GetIntStatus(LPC_TIM0, TIM_MR1_INT) == SET){
        valorADC =+ ADC_ChannelGetData(ADC_CHANNEL_0);
        TIM_ClearIntPending(LPC_TIM0, TIM_MR1_INT);
        valorADC = valorADC / 4;
        if(valorADC < 1241){
            ADC_BurstCmd(DESABLE);
            TIM_Cmd(LPC_TIM1, DESABLE);
            GPIO_ClearValue(GPIO_PORT_0, (1<<10));
        }
        else if(valorADC >= 1240 && valorADC <= 2482){
            ADC_BurstCmd(ENABLE);
            TIM_Cmd(LPC_TIM1, ENABLE);
        }
        else{
            ADC_BurstCmd(DESABLE);
            TIM_Cmd(LPC_TIM1, DESABLE);
            GPIO_SetValue(GPIO_PORT_0, (1<<10));
        }
        valorADC = 0;
    }
}


void TIMER1_IRQHandler(void){
    if(TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT) == SET)
        valorPWM = ADC_GetChannelData(ADC_CHANNEL_0);
        TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
        float cicloTrabajo = (float) (25.0+20.0*(valorPWM/1241-1));
        TIM_UpdateMatchValue(LPC_TIM1, TIM_MATCH_CHANNEL_1, cicloTrabajo);
}