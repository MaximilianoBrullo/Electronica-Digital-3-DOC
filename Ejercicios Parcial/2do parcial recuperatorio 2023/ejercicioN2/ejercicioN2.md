## Ejercicio 2
Se tienen tres bloques de datos de 4KBytes de longitud cada uno en el cual se han guardado tres formas de onda. Cada muestra de la onda es un valor de 32 bits que se ha capturado desde el ADC. Las direcciones de inicio de cada bloque son representadas por macros del estilo DIRECCION_BLOQUE_N, con N=0,1,2.
Se pide que, usando DMA y DAC se genere una forma de onda por la salida analógica de la LPC1769. La forma de onda cambiará en función de una interrupción externa conectada a la placa de la siguiente manera:
● 1er interrupción: Forma de onda almacenada en bloque 0, con frecuencia de señal de 60[KHz].
● 2da interrupción: Forma de onda almacenada en bloque 1 con frecuencia de señal de 120[KHz].
● 3ra interrupción: Forma de onda almacenada en bloque 0 y bloque 2 (una a continuación de la otra) con frecuencia de señal de 450[KHz].
● 4ta interrupción: Vuelve a comenzar con la forma de onda del bloque 0.
En cada caso se debe utilizar el menor consumo de energía posible del DAC.