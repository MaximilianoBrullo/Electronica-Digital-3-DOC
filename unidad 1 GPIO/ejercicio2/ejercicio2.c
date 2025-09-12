#include "LPC17xx.h"                    // Device header


void configGPIO(void);

int main(void){
    
    uint8_t acumulador = 0;
    uint8_t valor = 0;
    
    configGPIO();
    
    while(1){
        
        valor = (LPC_GPIO0->FIOPIN) & (0b1111<<0); //leo P0.0 al P0.3
        
        if((LPC_GPIO0->FIOPIN) & (1<<4)){ //si P0.4 es 1
            acumulador += valor; //sumo
        }
        else{ //si P0.4 es 0
            acumulador -= valor; //resto
        }
        
    }
    
}

configGPIO(void){
    for (int i=6; i<31; i++){ //enmascaro el resto de pines del puerto 0
        LPC_GPIO0->FIOMASK |= (1<<i);//enmascaro P0.5 al P0.31
    }
    LPC_PINCON->PINSEL0 &= ~(0b1111111111<<0); //configuro P0.0 al P0.4 como GPIO
    LPC_GPIO0->FIODIR &= ~(0b11111<<0); //configuro P0.0 al P0.4 como entrada
    LPC_PINCON->PINMODE0 |= (0b1111111111<<0); //resistencia de pull down en P0.0 al P0.4
}