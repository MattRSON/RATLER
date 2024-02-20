#include <avr/io.h>
 #include <util/delay.h>

 int main()
 {
     // Set built-in LED pin as output
     DDRB |= (1 << DDB0);
     while (1) {
         PORTB |=  (1 << PB0);   // LED on
         _delay_ms(10);         // wait 500ms
         PORTB &= ~(1 << PB0);   // LED off
         _delay_ms(10);         // wait 500ms
     }
     return 0;
 }