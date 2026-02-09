#include <avr/io.h>
#include <util/delay.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

 int main()
 {
     // Set built-in LED pin as output
     SET_BIT(DDRB,DDB0);
     CLEAR_BIT(DDRB,DDB1);
     SET_BIT(PORTB,PB1); //Pull ups
     while (1) {
        if (TEST_BIT(PINB,PB1)) {
            SET_BIT(PORTB,PB0);
        }   
        else {
            CLEAR_BIT(PORTB,PB0);
        }
        /*
         SET_BIT(PORTB,PB0);        // LED on
         _delay_ms(500);         // wait 500ms
         CLEAR_BIT(PORTB,PB0);      // LED off
         _delay_ms(500);         // wait 500ms
         */
     }
     return 0;
 }