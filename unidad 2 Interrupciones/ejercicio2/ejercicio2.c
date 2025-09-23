#include "LPC17xx.h"

void configGPIO(void);
void configEINT1(void);


int main(void) {
    
    configGPIO();
    configEINT1();

    while (1) {
    // Bucle principal vacío, la lógica se maneja en la ISR
    }
}

void configGPIO(void) {
    // Configurar configuro el P0.0 a GPIO
    for(int i = 1; i < 32; i++){
    LPC_GPIO->FIOMASK |= (0b1<<i);
    }
    LPC_PINCON->PINSEL0 &=  ~(0b0<<0);//no es necesario porque el valor por reseteo es 00
    LPC_GPIO->FIODIR &= ~(0b0<<0);//tampoco es necesario ya que el valor de reseteo es 0 (input)
    LPC_GPIOINT->IO0IntEnR |= (0b1<<0);//activo la interrupcion por flanco ascendente en el p0.0
    LPC_GPIOINT->IO0IntClr |= (0b1<<0);//limpio el flag de interrupcion del pin P0.0
    NVIC_SetPriority(EINT3_IRQn, 3);//Le doy prioridad 3 a la interrupcion por GPIO
    NVIC_EnableIRQ(EINT3_IRQn);//habilita la interrupcion por puerto
}

void configEINT1(void){
    LPC_SC->EXTMODE |= (0b1<<1);//configuro la interrupcion externa por flanco
    LPC_SC->EXTPOLAR |= (0b0<<1);//selecciono el flanco de bajada, aunque no es necesario porque
    //por defecto esta configurado asi
    LPC_SC->EXTINT |= (0b1<<1);//limpio la bandera de la EINT1
    NVIC_SetPRiority
}