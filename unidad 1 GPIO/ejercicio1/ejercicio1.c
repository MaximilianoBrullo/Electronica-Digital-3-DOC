#include "LPC17xx.h"                    // Device header

void delayMs(int n);
void configGPIO(void);

int main(void)
{
    configGPIO();
    while(1){
        activarLuzA();
        delayMs(500);
        apagarLuzA();
        delayMs(500);
        activarLuzB();
        delayMs(500);
        apagarLuzB();
        delayMs(500);
    }
    
}

void configGPIO(void)
{
    for(int i=0; i<10; i++){
        LPC_PINCON->PINSEL0 &= ~(0b11<<(i*2)); // Configuro pin a GPIO desde el P0.0 a P0.9
        LPC_GPIO0->FIODIR |= (1<<i);       // Configuro pin como salida desde el P0.0 a P0.9
    }
}

void activarLuzA(void)
{
    for(int i=0; i<10; i++){
        LPC_GPIO0->FIOSET |= (1<<i);       // Enciendo el LED desde el P0.0 a P0.9
        delayMs(100);
    }
}

void apagarLuzA(void)
{
    for(int i=0; i<10; i++){
        LPC_GPIO0->FIOCLR |= (1<<i);       // Apago el LED desde el P0.0 a P0.9
        delayMs(100);
    }
}

void activarLuzB(void){
    for(int i=9; i>=0; i--){
        LPC_GPIO0->FIOSET |= (1<<i);       // Enciendo el LED desde el P0.9 a P0.0
        delayMs(500);
    } 
}

void apagarLuzB(void){
    for(int i=9; i>=0; i--){
        LPC_GPIO0->FIOCLR |= (1<<i);       // Apago el LED desde el P0.9 a P0.0
        delayMs(500);
    } 
}