#include "lpc17xx_gpdma.h"

#define ADC_BUFFER0_START 0x2007C000
#define ADC_BUFFER1_START 0x2007E000
#define ADC_BUFFER_SIZE ((ADC_BUFFER0_END - ADC_BUFFER0_START)/sizeof(uint32_t))

/*Definicion de Constantes y Macros*/
volatile uint32_t *adc_samples = (volatile uint32_t *)ADC_BUFFER0_START;
volatile bool ADC_FLAG =false;

/*Prototipos de funciones*/
void cfgDMA(void);
void storeADCsamples(void);

/*Funcion principal*/
int main(void)
{
    cfgDMA(void);
    while(1);
    return 0;
}

void cfgDMA(void)
{
    GPDMA_LLI_Type linkedListADC_DMA = {0};
    GPDMA_Channel_CFG_Type GPDMAcfg = {0};

    NVIC_DisableIRQ(DMA_IRQn);
    GPDMA_Init();
    
    linkedListADC_DMA.srcAddr = (uint32_t)&(LPC_ADC->ADDR0);
    linkedListADC_DMA.dstAddr = (uint32_t)adc_samples;
    linkedListADC_DMA.nextLLI = (uint32_t)&linkedListADC_DMA;
    linkedListADC_DMA.control = (ADC_BUFFER_SIZE<<0) | (2<<18)
                                | (1<<21) | (1<<27) & ~(1<<26);

    GPDMAcfg.channelNum = 0;//Seleccion del canal de transferencia
    GPDMAcfg.srcConn = GPDMA_CONN_ADC; // Selecciono el periferico fuente de datos
    GPDMAcfg.dstConn = 0;// No se usa un periferico destino
    GPDMAcfg.srcMemAddr = 0;//No se usa la memoria como fuente
    GPDMAcfg.dstMemAddr = (uint32_t)adc_samples;//Direccion de memoria destino
    GPDMAcfg.transferType = GPDMA_P2M;//Tipo de transferencia periferico a memoria
    GPDMAcfg.transferSize = ADC_BUFFER_SIZE;//Cantidad de datos a transferir
    GPDMAcfg.transferWidth = 0;//Tamanio de cada dato (8,16,32 bits)
    GPDMAcfg.linkedlist = (uint32_t)&linkedListADC_DMA;//Direccion de la estructura LLI
}