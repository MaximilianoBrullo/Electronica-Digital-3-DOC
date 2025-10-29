#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_adc.h"

void cfgPCB();
void cfgTimer();
void cfgADC();

int main(){
    cfgPCB();
    cfgTimer();
    cfgADC();
    while(1);
    return 1;
}

void cfgPCB()
{
    PINSEL_CFG_Type led = {0};
    led.portNum = PINSEL_PORT_0;
    led.pinNum = PINSEL_PIN_22;
    led.funcNum = PINSEL_FUNC_0;
    led.pinMode = PINSEL_MODE_TRISTATE;
    led.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&led);//ledRojo
    GPIO_SetDir(&led);
    led.pinNum = PINSEL_PIN_18;
    PINSEL_ConfigPin(&led);//ledVerde
    GPIO_SetDir(&led);
    led.pinNum = PINSEL_PIN_20;
    PINSEL_ConfigPin(&led);//ledAmarillo;
    GPIO_SetDir(&led);
}

void cfgTimer()
{
    TIM_TIMERCFG_Type = {TIM_USVAL, 1000};
    
    TIM_MATCHCFG
}

void cfgADC()
{
    ADC_Init(200000);
    ADC_ChannelCmd(ADC_CHANNEL_0, ENABLE);
    ADC_Burset
}

void ADC_IRQHandler