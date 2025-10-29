#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_dac.h"

#define SRAM = 0x20080000

uint32_t FRECUENCIA = 200000;
uint16_t *buffer = (uint16_t*)SRAM;
uint16_t valorDAC = 0;

void cfgPCB();
void cfgTimer();
void cfgADC();
void cfgDAC();

int main(){
    cfgPCB();
    cfgTimer();
    cfgADC();
    while(1);
    return 1;
}

void cfgPCB()
{
    pinsel_cfg_type adc = {0};
    adc.pinNum = PINSEL_PIN_25;
    adc.portNum = PINSEL_PORT_0;
    adc.funcNum = PINSEL_FUNC_1;
    adc.pinMode = PINSEL_MODE_TRISTATE;
    adc.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&adc);//ADC0.2

    PINSEL_CFG_Type dac = {0};
    dac.portNum = PINSEL_PORT_0;
    dac.pinNum = PINSEL_PIN_26;
    dac.funcNum = PINSEL_FUNC_2;
    dac.pinMode = PINSEL_MODE_TRISTATE;
    dac.openDrain = PINSEL_OD_NORMAL;
    PINSEL_ConfigPin(&dac);//DAC0
}

void cfgADC()
{
    ADC_Init(FRECUENCIA);
    ADC_ChannelCmd(ADC_CHANNEL_2, ENABLE);
    ADC_BurstCmd(ENABLE);
}

void cfgDAC()
{
    DAC_Init();
    DAC_SetValue(valorDAC);
    DAC_UpdateValue();
}

void ADC_IRQHandler()
{   
    uint16_t valorADC = ADC_ChannelGetData(ADC_CHANNEL_2);//leo el valor del canal 2
    *buffer = valorADC;//guardo el valor en la direccion de memoria
    buffer++;//incremento el puntero para la siguiente muestra
    if(buffer >= (uint16_t*)(SRAM + 16)) //si el puntero supera las 16 muestras
    {
        ADC_DeInit();//deshabilito el ADC
        buffer = (uint16_t*)SRAM;
        for(int i=0; i<16; i++) //prendo los 3 leds
        {
            //sumo todos los valores de las muestras
            valorDAC += *buffer;
            buffer++;
        }
        valorDAC = valorDAC / 16; //hago el promedio
        DAC_SetValue(valorDAC); //seteo el valor del DAC
        DAC_UpdateValue(); //actualizo el valor del DAC
    }
    ADC_Init(FRECUENCIA);//habilito el ADC nuevamente
    ADC_IntClear(ADC_CHANNEL_2);
}z