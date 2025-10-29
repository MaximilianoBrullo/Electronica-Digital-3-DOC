#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_gpdma.h"

#define FRECUENCIA 200000
/*Defino la direccion de la memoria con un puntero*/
uint16_t *buffer = (uint16_t)0x20080000;//se castea para que el hexadecimal sea puntero
GPDMA_LLI_Type dmaListADC = {0};

void cfgPCB(void);
void cfgADC(void);
void cfgDMA(void);

int main (void)
{
	cfgPCB();
	cfgADC();
	cfgDMA();
}

void cfgPCB()
{
	PINSEL_CFG_Type adc0 = {0};
	adc0.portNum = PINSEL_PORT_0;
	adc0.pinNum = PINSEL_PIN_23;
	adc0.funcNum = PINSEL_FUNC_1;
	adc0.pinMode = PINSEL_TRISTATE;
	adc0.openDrain = PINSEL_OD_NORMAL;

	PINSEL_ConfigPin(&adc0);
}

void cfgADC()
{
	ADC_Init(FRECUENCIA);//HABILITA
	ADC_ChannelCmd(ADC_CHANNEL_0, ENABLE);//HABILITA EL CANAL 0 DEL ADC
	ADC_BurstCmd(ENABLE);//HABILITA EL MODO RAFAGA
}

void cfgDMA()
{	GPDMA_LLI_Type dmaListADC = {0};
	GPDMA_Channel_CFG_Type canalADC = {0};
	canalADC.chanelNum = GPDMA_CHANNEL_0;
	canalADC.transferSize = 256;
	canalADC.srcMemAddr = LPC_ADC->ADGDR;
	canalADC.destMemAddr = (uint16_t)buffer;
	canalADC.transferType = GPDMA_P2M;//periferico a memoria
	canalADC.srcConn = GPDMA_ADC;
	canalADC.linkedList = (uint32_t)(uintptr_t)&dmaListADC;

	dmaListADC.srcAddr = LPC_ADC->ADGDR;
	dmaListADC.dstAddr = (uint32_t)(uintptr_t)buffer;
	dmaListADC.nextLLI = (uint32_t)(uintptr_t)&dmaListADC;
	dmaListADC.control = 256 | 1<<12 | 1<<15 | 1<<18 | 1<<22 | 1<<27;

	GPDMA_Init();
	GPDMA_Setup(&canalADC);
	GPDMA_ChannelCmd(GPDMA_CHANNEL_0,ENABLE);

	GPDMA_LLI_Type dmaListADC = {0};
}
