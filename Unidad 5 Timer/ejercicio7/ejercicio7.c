#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"

#define PIN22 (1<<22)

volatile uint32_t lastCapValue = 0;
volatile uint32_t currCapValue = 0;
volatile uint8_t flag = 0;

void cfgPinsel(void);
void cfgTimer(void);

int main()
{
    cfgPinsel();
    cfgTimer();
    while(1){}
    return 0;
}

void cfgPinsel(void)
{
    PINSEL_CFG_Type redLed = {0};
    redLed.portNum = PINSEL_PORT_0;
    redLed.pinNum = PINSEL_PIN_22;
    redLed.funcNum = PINSEL_FUNC_0;
    redLed.pinMode = PINSEL_TRISTATE;
    redLed.openDrain = PINSEL_OD_NORMAL;

    PINSEL_CFG_Type cap0Tim3 = redLed;
    cap0Tim3.pinNum = PINSEL_PIN_23;
    cap0Tim3.funcNum = PINSEL_FUNC_3;

    PINSEL_ConfigPin(&redLed);
    PINSEL_ConfigPin(&cap0Tim3);

    GPIO_SetDir(GPIO_PORT_0, (1<<22), GPIO_OUTPUT);
}

void cfgTimer(void)
{
	TIM_COUNTERCFG_Type counter = {0}; //estructura para configurar el timer en modo contador
	counter.countInputSelect = TIM_CAPTURE_CHANNEL_0; //pin de entrada cap0

	TIM_CAPTURECFG_Type capture = {0}; //estructura para configurar el pin de patura
	capture.captureChannel = TIM_CAPTURE_CHANNEL_0; //habilitacion del canal de CAPTURE0
	capture.risingEdge = ENABLE; //evento de vaptura por flaco ascendente
	capture.fallingEdge = DISABLE; //evento de captura por flanco descendente
	capture.intOnCapture = ENABLE; // interrupcion durante el evento de captura

	TIM_Init(LPC_TIM3, TIM_COUNTER_RISING_MODE, &counter); //Configuracion de la modalidad del timer0 como contador
	TIM_ConfigCapture(LPC_TIM3, &capture); //Configuracion del capture
	TIM_Cmd(LPC_TIM3, ENABLE); //Habilitacion del timer0
	NVIC_EnableIRQ(TIMER3_IRQn);
	return;
}

void TIMER3_IRQHandler(void)
{
	if(TIM_GetIntStatus(LPC_TIM3, TIM_CR0_INT))
	{
		currCapValue = TIM_GetCaptureValue(LPC_TIM3, TIM_CAPTURE_CHANNEL_0);
		if(flag == 0)
		{
			lastCapValue = currCapValue;
			flag = 1;
		} else {
			uint32_t diffTick = (currCapValue - lastCapValue);
			float timeTick = 1.0f / 25000000.0f; //40nS ya que el cclk es de 100 y preescalamos dividiendo en 4
			float time = timeTick * diffTick;
			if(time >= 1.0f)
			{
				GPIO_SetPins(GPIO_PORT_0, PIN22);
			}
		}

		TIM_ClearIntPending(LPC_TIM3, TIM_CR0_INT);
	}
}
