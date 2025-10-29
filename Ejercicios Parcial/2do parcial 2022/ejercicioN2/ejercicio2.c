#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_exti.h"

uint32_t waveFormSub[1024]; // Arreglo para la forma de onda triangular parte de subida
uint32_t waveFormBaj[1024]; // Arreglo para la forma de onda Triangular parte de bajada
void cargarWaveform();
void cfgPinsel();
void cfgtimer();
void cfgDAC();

int main()
{
    cargarWaveform();
    cfgPinsel();
    cfgtimer();
    cfgDAC();
    
    while(1);
    return 0;
}

void cargarWaveform(){
    uint16_t i;
    for (i; i<1024; i++){
        waveFormSub[i] = i;
    }
    for (i; i>0; i--){
        waveFormBaj[i] = i;
    }
}

void cfgPinsel()
{
    PINSEL_CFG_Type PinCfg;

    // Configuro el pin del DAC
    PinCfg.Funcnum = 2;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 0;
    PinCfg.Pinnum = 26;
    PINSEL_ConfigPin(&PinCfg);
}

void cfgtimer()
{
    TIM_TIMERCFG_Type TIM_ConfigStruct;
    TIM_MATCHCFG_Type TIM_MatchConfigStruct;

    // Configuro el timer
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue = 1; // 1us

    // Inicializo el timer 0
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &TIM_ConfigStruct);

    // Configuro el match para que interrumpa cada 1ms
    TIM_MatchConfigStruct.MatchChannel = 0;
    TIM_MatchConfigStruct.IntOnMatch = ENABLE;
    TIM_MatchConfigStruct.ResetOnMatch = ENABLE;
    TIM_MatchConfigStruct.StopOnMatch = DISABLE;
    TIM_MatchConfigStruct.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
    TIM_MatchConfigStruct.MatchValue = 1;// 1uS

    TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);

    // Habilito la interrupcion del timer 0
    NVIC_EnableIRQ(TIMER0_IRQn);

    // Inicio el timer 0
    TIM_Cmd(LPC_TIM0, ENABLE);
}

void cfgDAC()
{
    DAC_CURRENT_OPT currentDAC = {DAC_MAX_CURRENT_700uA};
    DAC_Init();
    DAC_SetBias(&currentDAC);
}

void TIMER0_IRQHanler(){
    static uint16_t i = 0;
    if(i<1024){
        DAC_UpdateValue(WaveFormSub[i]);
    } else if (i<2048 && i>=1024){
        DAC_UpdateValue(WaveFormBaj[i]);
    }
    i++;
    if(i==2048) i=0;
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}