#include "lpc17xx_gpdma.h"


#define ADC_BUFFER0_START 0x2007C000
#define ADC_BUFFER0_END   0x2007E000
#define ADC_BUFFER0_SIZE ((ADC_BUFFER0_END - ADC_BUFFER0_START) / sizeof(uint32_t ))

volatile uilt32_t *adc_samples = (volatile uint32_t *) ADC_BUFFER0_START;
volatile bool ADC_FLAG = false;

void cfgGPDMA();
void storeADCsamples();

int main()
{
    cfgGPDMA();
    while(1)
    {
        if(ADC_FLAG == true) storeADCsamples;
    }
    return 0;
}

void storeADCsamples(void)
{
    GPDMA_ChannelCmd(GPDMA_CHANNEL_0, ENABLE);//Inicializamos el envio de datos por canal 0 de DMA
    return;
}

void cfgGPDMA()
{
	GPDMA_LLI_Type lli0_ch0 = {0};//Estructura para la lista enlazada para el canal0 del DMA
	GPDMA_Channel_CFG_Type ADC_DMA_ch0 = {0};//Estructura de cConfiguracion para canal 0 del DMA
	
	NVIC_Disable(DMA_IRQn);//Deshabilitacion de Interrupcion por DMA
	GPDMA_Init();//Inicializacion del Controlador GPDMA
	
	//------------INICIO-cfg-LLI---------------------------------------------------
	lli0_ch0.srcAddr = (uint32_t)&(LPC_ADC->ADDR0);//Direccion de fuente de datos(Periferico)
	lli0_ch0.dstAddr = (uint32_t)adc_samples;//Direccion de Destino de Datos(Memoria)
	lli0_ch0.nextLLI = (uint32_t)&lli0_ch0;//Proxima direccion de destino de datos: LLI Repetitiva
	lli0_ch0.control = (ADC_BUFFER_SIZE<<0)//Tamanio de la Transferencia de la LLI
			|(2<<18)//Ancho de la transferencia de Fuente(32bits)
			|(2<<21)//Ancho de la transferencia de Destino(32bits)
			|(1<<27)//Incremento en la direccion de destino
			& ~(1<<26);//No Incremeintamos la direccion de fuente
	//--------------FIN-cfg-LLI--------------------------------------
	
	//--------------INICIO-cfg-CHANNEL----------------------------------
	ADC_DMA_ch0.channelNum = GPDMA_CHANNEL_0;//Seleccion del canal de transferencia
	ADC_DMA_ch0.srcConn = GPDMA_ADC;//Periferico de funde te datos
	ADC_DMA_ch0.dstConn = 0;//Periferico de datos de destino sin uso
	ADC_DMA_ch0.srcMemAddr = 0;//Direccion de memoria como fuente de datos sin uso
	ADC_DMA_ch0.dstMemAddr = (uint32_t)adc_samples;//Direccion de destino de datos de memoria
	ADC_DMA_ch0.transferType = GPDMA_P2M;//Tipo de transferencia (Periferico a Memoria)
	ADC_DMA_ch0.transferSize = ADC_BUFFER0_SIZE;//Tamanio de la transferencia de datos
	ADC_DMA_ch0.transferWidth = 0;//Ancho de la transferencia de Datos (SinUso)
	ADC_DMA_ch0.linkedList = (uint32_t)&lli0_ch0;
	
	GPDMA_Setup(&ADC_DMA_ch0);
	return;
}