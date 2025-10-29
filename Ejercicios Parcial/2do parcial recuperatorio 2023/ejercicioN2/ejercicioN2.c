#include "lpc17xx_pinsel.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_dac.h"
#include "lpc17xx_dma.h"

/*Definiciones y Macros*/
#define BUFFER_SIZE 4096
#define DIRECCION_BLOQUE_1 (uint32_t *) 0x20080000
#define DIRECCION_BLOQUE_2 (uint32_t *) 0x20081000
#define DIRECCION_BLOQUE_3 (uint32_t *) 0x20082000

/*Prototipos de funciones*/
void cfgPinsel();
void cfgDAC();
void cfgDMA();

int main(){
    cfgPinsel();
    cfgDAC();
    cfgDMA();
    while (true)
    {
    }
    return 0;
}

void cfgPinsel(){
    PINSEL_CFG_Type pinCfg;
    pinCfg.portNum = PINSEL_PORT_0;
    pinCfg.pinNum = PINSEL_PIN_26;
    pinCfg.funcNum = PINSEL_FUNC_2;
    pinCfg.pinMode = PINSEL_PINMODE_TRIESTATE;
    pinCfg.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&pinCfg);
    return;
}

void cfgDAC(){
     
}

void cfgDMA(){
    GPDMA_LLI_Type lli_bloque1_DAC = {0};
    GPDMA_LLI_Type lli_bloque2_DAC = {0};
    GPDMA_LLI_Type lli_bloque3_DAC = {0};
    GPDMA_Channel_CFG_Type dma = {0};

    NVIC_DisableIRQ(DMA_IRQn);
    GPDMA_Init();

    lli_bloque1_DAC.srcAddr = (uint32_t)DIRECCION_BLOQUE_1
    lli_bloque1_DAC.dstAddr = (uint32_t)&(LPC_DAC->DACR);
    lli_bloque1_DAC.nextLLi = (uint32_t)&lli_bloque2_DAC;
    lli_bloque1_DAC.control = (BUFFER_SIZE<<0) | (1<<19) | (1<<22) | (1<<26)

    lli_bloque2_DAC.srcAddr = (uint32_t)DIRECCION_BLOQUE_2
    lli_bloque2_DAC.dstAddr = (uint32_t)&(LPC_DAC->DACR);
    lli_bloque2_DAC.nextLLi = (uint32_t)&lli_bloque3_DAC;
    lli_bloque2_DAC.control = (BUFFER_SIZE<<0) | (1<<19) | (1<<22) | (1<<26)

    lli_bloque3_DAC.srcAddr = (uint32_t)DIRECCION_BLOQUE_3
    lli_bloque3_DAC.dstAddr = (uint32_t)&(LPC_DAC->DACR);
    lli_bloque3_DAC.nextLLi = (uint32_t)&lli_bloque1_DAC;
    lli_bloque3_DAC.control = (BUFFER_SIZE<<0) | (1<<19) | (1<<22) | (1<<26)

    dma.channelNum = 0;
    dma.transferSize = BUFFER_SIZE;
    dma.transferWidth = 0;
    dma.srcMemAddr = (uint32_t)DIRECCION_BLOQUE_1;
    dma.dstMemAddr = (uint32_t)&(LPC_DAC->DACR);
    dma.transferType = GPDMA_M2P;
    dma.srcConn = 0;
    dma.dstConn = GPDMA_CONN_DAC;
    dma.linkedList = (uint32_t)&lli_bloque1_DAC;
    GPDMA_Setup(&dma);
    GPDMA_ChannelCmd(0,ENABLE);
}