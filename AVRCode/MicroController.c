/*
Spi communication discription
Key value pairs each with 8 bits

keys for Rpi to avr
10000000 Keep alive
10000001 Motor1 Forward Speed
10000010 Motor1 Reverse Speed
10000011 Motor2 Forward Speed
10000100 Motor2 Reverse Speed
10000101 Motor3 Forward Speed
10000110 Motor3 Reverse Speed
10000111 Motor4 Forward Speed
10001000 Motor4 Reverse Speed
10001001 Power Motors
10001010 Debug dump


keys for avr to Rpi
11111111 Nothing to report
11111110 Speed of Motor 1
11111101 Speed of Motor 3
11111100 Batt Voltage
11111011 12V rail
11111010 5V rail
11111001 3.3V rail
11111000 Motor Driver Overtemp
11110111 Motor Current 1
11110110 Motor Current 2
11110101 Motor Current 3
11110100 Motor Current 4

*/
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

volatile uint8_t key = 0;
volatile uint8_t value = 0;
volatile char flag = 0;

void SPI_SlaveInit(void) {
    // Set MISO as output
    SET_BIT(DDRB,PB4);
    // Enable SPI
    SET_BIT(SPCR, SPE);
    SET_BIT(SPCR,SPIE);
    sei();
    
}


// char SPI_Receive(int Rx) {
//     // Wait for reception to complete
//     SPDR = Rx;

//     while(!(SPSR & (1<<SPIF))) {}
//     //return data register
//     return SPDR;
// }


int main(void) {
    char subkey = 0x0;
    char subvalue = 0x0;
    SPI_SlaveInit();
    SET_BIT(DDRB,PB0);

    while (1) {
        if (flag) {
            cli();
            subkey = key;
            subvalue = value;
            sei();
            flag = 0;
        }

        if (subkey == 0xFF) {
            SET_BIT(PORTB,PB0);
            SPDR = 0xFF; 
        } 
        else {
            CLEAR_BIT(PORTB,PB0);
            SPDR = 0x12;
        }
    }


}

//SPIE flag for interrupt
ISR(SPI_STC_vect)
{

    char data = SPDR;
    if (TEST_BIT(data,7)) {
        key = data;
    }
    else {
        value = data;
    }
    flag = 1;

}