## Múltiples Interrupciones
Cree un programa que use dos fuentes de interrupción para controlar secuencias de LEDs diferentes.
• Configuración:
o Configure el pin P0.0 para generar una interrupción por flanco de subida.
o Configure una interrupción externa (EINT1) en el pin P2.11 por flanco de bajada.

• Lógica:
o Cuando se detecta la interrupción del pin P0.0, se debe ejecutar una secuencia de 4 LEDs o 4
números si está utilizando un display de 7 segmentos
o Cuando se detecta la interrupción externa del pin P2.11, se debe ejecutar una secuencia de
parpadeo diferente.
• Condiciones: Configure la interrupción externa (EINT1) para que tenga una mayor prioridad que la
interrupción del GPIO del Puerto 0.