#include "LPC17xx.h"                    // Device header


void mostrarValor7Segmentos(int valor);//enciende y apaga los pines del puerto 2 para
                                       //la representacion del signo en el display de 7 segmentos.
void configGPIO(void);//configuro pines GPIO
void clearDisplay(void);//apago todos los pines del puerto 2
void configIntGpio(void);//configuro las interrupciones GPIO
void aumentarValor(void);//aumento valor que se muestra en el display de 7 segmentos


void main(){
    configGPIO();
    configIntGpio();
    clearDisplay();
    mostrarValor7Segmentos(0); //muestro 0 en el display al iniciar
    while(1);
}

void EINT3_IRQHandler(void){//rutina de atencion a la interrupcion del boton 1
    if((LPC_GPIOINT->IOIntStatus) & 0x1){//pregunto si hay interrupcion pendiente en el puerto 2
        if((LPC_GPIOINT->IO2IntStatR) & 0x01){//pregunto si la interrupcion por flanco de subida fue en el pin 0
            aumentarvalor();
        }
    }
    LPC_GPIOINT->IO0IntClr |= 0x01; //limpio la interrupcion pendiente en el pin P0.0
}

void mostrarValor7Segmentos(int valor){
    clearDisplay();
    switch(valor){
        case 0:
            LPC_GPIO2->FIOSET = 0x3F; //0b00111111
            break;
        case 1:
            LPC_GPIO2->FIOSET = 0x06; //0b00000110
            break;
        case 2:
            LPC_GPIO2->FIOSET = 0x5B; //0b01011011
            break;
        case 3:
            LPC_GPIO2->FIOSET = 0x4F; //0b01001111
            break;
        case 4:
            LPC_GPIO2->FIOSET = 0x66; //0b01100110
            break;
        case 5:
            LPC_GPIO2->FIOSET = 0x6D; //0b01101101
            break;
        case 6:
            LPC_GPIO2->FIOSET = 0x7D; //0b01111101
            break;
        case 7:
            LPC_GPIO2->FIOSET = 0x07; //0b00000111
            break;
        case 8:
            LPC_GPIO2->FIOSET = 0x7F; //0b01111111
            break;
        case 9:
            LPC_GPIO2->FIOSET = 0x6F; //0b01101111
            break;
        default:
            clearDisplay();
            break;
    }
}

void clearDisplay(void){
    LPC_GPIO2->FIOCLR |= 0xFF; //apago todos los pines del puerto 2
}

void configGPIO(void){
    LPC_PINCON->PINSEL4 &= ~0xF; //configuro pines P0.0 a P0.7 como GPIO
    LPC_PINCON->PINSEL0 &= ~0x1; //configuro el pin P0.0 como GPIO
    LPC_GPIO2->FIOMASK |= 0xFFFFFF00 //enmascaro lo pines no usados
    LPC_GPIO0->FIOMASK |= 0xFFFFFFFE; //enmascaro los pines del P0.1 a P0.31 excepto el P0.0
}

void configIntGpio(void){
    LPC_GPIOINT->IO0IntEnR |= 0x01; //habilito interrupcion por flanco de subida en el pin P0.0
    LPC_GPIOINT->IO0IntClr |= 0x01; //limpio cualquier interrupcion pendiente en el pin P0.0
    NVIC_SetPriority(EINT3_IRQn, 1); //seteo prioridad de interrupcion EINT3
    NVIC_EnableIRQ(EINT3_IRQn); //habilito interrupcion EINT3
}

void aumentarValor(void){
    static int valor = 0;
    valor++;
    if(valor > 9){
        valor = 0;
    }
    mostrarValor7Segmentos(valor);
}