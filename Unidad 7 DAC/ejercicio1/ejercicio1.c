#include "lpc17xx_pinsel.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_dac.h"

#define NUM_WAVE_SAMPLES 1024

uint32_t waveForm[NUM_WAVE_SAMPLES];

void cfgWaveForm();
void cfgPinsel();
void cfgTimer();
void cfgDAC();

int main(void)
{
	cfgWaveForm();
	cfgPinsel();
	cfgTimer();
	cfgDAC();
	while(1){}
	return 0;
}

void cfgWaveForm(void)
{
	for(uint16_t i = 0; i < NUM_WAVE_SAMPLES; i++){
		WaveForm[i] = i;
	}
	return;
}

void cfgPinsel()
{
	PINSEL_CFG_Type pinMatch00 = {0};
	PINSEL_CFG_Type pinDAC = {0};
	
	pinMatch00.portNum = PINSEL_PORT_1;
	pinMatch00.pinNum = PINSEL_PIN_28;
	pinMatch00.funcNum = PINSEL_FUNC_3;
	pinMatch00.pinMode = PINSEL_TRISTATE;
	pinMatch00.openDrain = PINSEL_OD_NORMAL;
	
	pinDAC.portNum = PINSEL_PORT_0;
	pinDAC.pinNum = PINSEL_PIN_26;
	pinDAC.funcNum = PINSEL_FUNC_2;
	pinDAC.pinMode = PINSEL_TRISTATE;
	pinDAC.openDrain = PINSEL_OD_NORMAL;
	
	PINSEL_ConfigPin(&pinMatch);
	PINSEL_ConfigPin(&pinDAC);
	return;
}

void cfgTimer()
{
	TIM_TIMERCFG_Type timer = {0};
	TIM_MATCHCFG_Type match = {0};
	
	timer.prescaleOption = TIM_USVAL;
	timer.prescaleValue = 0;//tiempo en uS
	
	match.matchChannel = TIM_MATCH_CHANNEL_0;
	/**
	 * la frecuencia de la senial es 976Hz
	 * f-signal = f-delay / N-samples => f-signal = 1 / (t-delay * N-samples)
	 * reordenando tenemos...
	 * t-delay = tMatch = 1 / (f-signal * N-samples)
	 * enses caso => match = 1/ (974Hz * 1024) = 1uS 
	 * recordemos que el tiempo de match debe ser mayor o igual que
	 * el tiempo minimo de conversion del DAC que son 1uS siempre que 
	 * la capacitancia que se conecte a la salida no sea mayor a 100pf
	 * En el caso de que esta sea mayor a 100pF se debe dejar mayor tiempo
	 * para que el dac convierta  
	 */
	match.matchValue = 1;//match cada 1uS
	match.intOnMatch = ENABLE;
	match.resetOnMatch = ENABLE;
	match.stopOnMatch = DISABLE;
	match.extMatchOutputType = TIM_EXTMATCH_NOTHING;
	
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer);
	TIM_ConfigMatch(LPC_TIM0, &match);
	TIM_Cmd(LPC_TIM0, ENABLE);
	NVIC_EnableIRQ(TIMER0_IRQn);
	return;	
}

void cfgDAC(void)
{
	DAC_Init();//Inicializa el DAC
	DAC_SetBias(DAC_MAX_CURRENT_700uA);//seteamos frecuencia maxima de funcionamiento de 1Mhz y corriente de 700uA
	return;
}

void TIMER0_IRQHandler(void)
{
	if(TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT))//consultamos interrupcion por MR0
	{
		static uint16_t indexWave = 0;//actualizacion del valor al dac
		DAC_UpdateValue(waveForm[indexWave]);//Actualizacion de valor al DAC
		indexWave = (indexWave + 1) % NUM_WAVE_SAMPLES; // incrementa el buffer circular
		TIM_ClearIntPending(LPC_TIM0, TIM_MRO_INT);
	}
	return;
}