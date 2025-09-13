## Contador Hexadecimal con Botón
Utilice 7 pines GPIO para controlar un display de 7 segmentos. El programa debe mostrar de manera
cíclica los 16 dígitos hexadecimales (0-F) de forma ascendente.
El incremento de la cuenta debe ser manejado por una interrupción por flanco de subida en un pin GPIO.
• Pines: Utilice un pin del Puerto 0 como entrada para el pulsador y 7 pines del Puerto 2 para el
display de 7 segmentos.
• Condiciones:
o Configure el pin de entrada para que genere una interrupción solo cuando se detecte un
flanco de subida (paso de 0 a 1).
o La lógica de incremento del contador debe residir en el Handler de la Interrupción (ISR).