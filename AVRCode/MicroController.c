/*
Spi communication description
Key value pairs each with 8 bits

keys for Rpi to avr
10000000 Keep alive
10000001 Motor1 Forward Speed 0x81
10100001 Motor1 Reverse Speed 0xA1
10000010 Motor2 Forward Speed 0x82
10100010 Motor2 Reverse Speed 0xA2
10000011 Motor3 Forward Speed 0x83
10100011 Motor3 Reverse Speed 0xA3
10000100 Motor4 Forward Speed 0x84
10100100 Motor4 Reverse Speed 0xA4
10001001 Turn on Motors 0x89
10001010 Debug dump 0x8A


keys for avr to Rpi
11111111 Nothing to report 0xFF
11111110 Speed of Motor 1 0xFE
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

Pin mapping
ADC0 - Battery Voltage
ADC1 - 12V rail
ADC2 - 5V rail
ADC3 - 3.3V rail
To start use just the one timer for PWM, Testing other pin PWM outputs on the same timer later
D3 - Motor enabler ??
D4 - Motor 1 Direction
D5 - Motor 1 PWM
D6 - Motor 2 PWM
D7 - Motor 2 Direction
B0 - Read motor driver overtemp



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
volatile uint8_t avrkey = 0;
volatile uint8_t avrvalue = 0;
volatile char flag = 0;

void SPI_SlaveInit(void) {
    // Set MISO as output
    SET_BIT(DDRB,PB4);
    // Enable SPI
    SET_BIT(SPCR, SPE);
    // Enable SPI interrupt
    SET_BIT(SPCR,SPIE);

    // Enable Global interrupt
    sei();
    
}
void set_motor(uint8_t motor, uint8_t dir, uint8_t speed) {
    // Placeholder function to control motors
    // motor: 1-4, dir: 0x00 for forward, 0x20 for reverse, speed: 0-127
    // Speed controlled by PWM, so we can use the speed value to set the duty cycle of the PWM signal for the corresponding motor


}



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

        if (subkey == 0xFF && subvalue == 0x7F) {
            SET_BIT(PORTB,PB0);
            //SPDR = value; 
            subkey = 0x00; // Clear the subkey and subvalue after processing
            subvalue = 0x00;

            avrkey = 0x21; // Send back something as a test
            avrvalue = 0x34;
        } 
        else if (subkey == 0xFF && subvalue == 0x01) {
            CLEAR_BIT(PORTB,PB0);
            //SPDR = 0x12;
            subkey = 0x00; // Clear the subkey and subvalue after processing
            subvalue = 0x00;

            avrkey = 0xFF; // Send back something as a test
            avrvalue = 0xE4;
        }

        // Motor control
        if (subkey & 0x80) {
            uint8_t motor = subkey & 0x0F; // Extract motor number (1-4)
            uint8_t dir   = subkey & 0x20; // 0x00 for forward, 0x20 for reverse

            set_motor(motor, dir, subvalue);

            //avrkey = 0xFE;
            //avrvalue = subkey;
        }
    }
}

//SPIE flag for interrupt
ISR(SPI_STC_vect)
{

    char data = SPDR;
    // Check the MSB of the received data to determine if it's a key or value
    // All keys have the MSB set to 1, while values have the MSB set to 0
    if (TEST_BIT(data,7) == 1) {
        key = data;
        SPDR = avrkey;
    }
    else {
        value = data;
        SPDR = avrvalue;
    }
    flag = 1;

}