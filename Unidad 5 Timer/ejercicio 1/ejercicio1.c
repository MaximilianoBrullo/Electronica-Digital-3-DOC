#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_timer.h"

void cfgPin();
void cfgTimer();

int main()
{
	cfgPin();
	cfgTimer();
	while(1);
	return 1;
}

void cfgPin()
{
	PINSEL_CFG_Type led = {0};
	led.portNum = PINSEL_PORT_0;
	led.pinNum = PINSEL_PIN_22;
	led.funcNum = PINSEL_FUNC_0;
	PINSEL_ConfigPin(&led);
	GPIO_SetDir(GPIO_PORT_0, 0xFFFFFFFF, GPIO_OUTPUT);
}

void cfgTimer()
{
	TIM_TIMERCFG_Type timer = {TIM_USVAL, 1000};
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);

	TIM_MATCHCFG_Type match;
	match.matchChannel = TIM_MATCH_CHANNEL_0;
	match.intOnMatch = ENABLE;
	match.stopOnMatch = DISABLE;
	match.resetOnMatch = ENABLE;
	match.extMatchOutputType = TIM_NOTHING;
	match.matchValue = 1000;
	TIM_ConfigMatch(LPC_TIM0, &match);

	TIM_Cmd(LPC_TIM0, ENABLE);

	NVIC_EnableIRQ(TIMER0_IRQn);
	return;
}

void TIMER0_IRQHandler(void)
{
	static uint8_t i = 0;
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))
	{
		if(i == 0)
		{
			GPIO_SetPins(GPIO_PORT_0, 0xFFFFFFFF);
			i=1;
		}else if(i == 1)
		{
			GPIO_ClearPins(GPIO_PORT_0, 0xFFFFFFFF);
			i=0;
		}
	}
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}
