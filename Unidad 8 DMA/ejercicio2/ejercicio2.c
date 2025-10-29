#include "lpc17xx_gpdma.h"
#include "lpc17xx_dac.h"

#define DAC_BUFFER_START 0x20080000
#define NUM_WAVE_SAMPLES 382

volatile uint32_t *dac_samples = (volatile uint32_t *) DAC_BUFFER_START;
volatile bool DAC_FLAG = false;
volatile uint32_t DMA_CH0_TC = 0;//FLAG de cuenta Terminal para canal 0 de DMA
volatile uint32_t DMA_CH0_Err = 0;//FLAG de Error teeminal para canal 0 de DMA

void cfgDMA();
void loadDACwave();

int main()
{
    cfgDMA();
    while(1)
    {
        if(DAC_FLAG = true) loadDACwave();
    }
    return 0;
}

void loadDACwave()
{
    GPDMA_ChannelCmd(GPDMA_CHANNEL_0, ENABLE);//Inicializamos el envio de datos por el canal 0
    NVIC_EnableIRQ(DMA_IRQn);
    return;
}

void cfgDAC()
{
	uint32_t countDAC = 65535;//numero de cuentas para el timer dedicado al DAC
	DAC_CONVERTER_CFG_type confDAC = {0};
	
	confDAC.counterEnable = ENABLE;//habilitacion de timer dedicado al DAC
	confDAC.
	
}