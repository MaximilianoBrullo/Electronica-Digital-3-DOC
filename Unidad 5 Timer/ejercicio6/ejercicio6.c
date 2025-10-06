#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_exti.h"

/*Definiciones, variables globales y macros*/
volatile uint8_t capflag = 0;
volatile unint32_t currCapValue = 0;
volatile uint32_t lastCapValue = 0;

/*prototipos de funciones*/
void cfgPinsel(void);
vois cfgTimer(void);

int main(void)
{
    cfgPinsel();
    cfgTimer();
    while(1);
    return 0;
}

cfgPinsel(void)
{
    PINSEL_CFG_Type led;
    led.portNum = PINSEL_PORT_0;
    led.pinNum = PINSEL_PORT_22;
    led.funcNum = PINSEL_FUNC_0;
    led.pinMode = PINSEL_PINMODE_TRISTATE;
    led.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&led);
    GPIO_SetDir(PORT_0, PIN_22, OUTPUT);

    PINSEL_CFG_Type boton;
    boton.portNum = PINSEL_PORT_1;
    boton.pinNum = PINSEL_PIN_26;
    boton.funcNum = PINSEL_FUNC_3;
    boton.pinMode = PINSEL_TRISTATE;
    boton.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&boton);

    return;
}

void cfgTimer(void)
{
    TIM_COUNTERCFG_Type counterCfg;
    counterCfg.CountInputSelect = TIM_CAPTURE_CHANNEL_0;

    TIM_Init(LPC_TIM0, TIM_COUNTER_RISING_MODE, &counterCfg);

    TIM_CAPTURECFG_Type captureCfg;
    captureCfg.captureChannel = TIM_CAPTURE_CHANNEL_0;
    captureCfg.risingEdge = ENABLE;
    captureCfg.fallingEdge = DISABLE;
    captureCfg.intOnCapture = ENABLE;
    TIM_ConfigCapture(LPC_TIM0, &captureCfg);
    
    TIM_Cmd(LPC_TIM0, ENABLE);
    NVIC_EnableIRQ(TIMER0_IRQn);
    
    return;
}

void TIMER0_IRQHandler(void)
{
    if(TIM_GetIntCaptureStatus(LPC_TIM0, TIM_CRO_INT))
    {
        currCapValue = TIM_GetCaptureValue(LPC_TIM0, TIM_CAPTURE_CHANNEL_0);

        if(capFlag == 0)
        {
            lastCapValue = currCapValue;
            capFlag = 1;
        } else {
            uint32_t diffticks = currCapValue - lastCapValue;
            float ticksTime = 1.0f / 25000000.0f; //25MHz
            float diffTime = diffticks * ticksTime;
            if(diffTime >= 1.0f)
            {
                GPIO_SetValue(PORT_0, PIN_22);
            } else {
                GPIO_ClearValue(PORT_0, PIN_22);
            }
            lastCapValue = currCapValue;
        }
    }
    TIM_ClearIntCapturePending(LPC_TIM0, TIM_CRO_INT);
}