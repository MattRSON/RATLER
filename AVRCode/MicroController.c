/*
Spi communication description
Key value pairs each with 8 bits

keys for Rpi to avr
10000000 Keep alive
11000001 Motor1 Forward Speed 0xC1
11001001 Motor1 Reverse Speed 0xC9
11000010 Motor2 Forward Speed 0xC2
11001010 Motor2 Reverse Speed 0xCA
11000011 Motor3 Forward Speed 0xC3
11001011 Motor3 Reverse Speed 0xCB
11000100 Motor4 Forward Speed 0xC4
11001100 Motor4 Reverse Speed 0xCC
10001001 Brake 0x89
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

Pin mapping Atmega 328p
B0 - Read motor driver overtemp
B1
B2 - Chip select for SPI
B3 - SPI MOSI
B4 - SPI MISO
B5 - SPI SCK
B6
B7
C0 - ADC0 - Battery Voltage
C1 - ADC1 - 12V rail
C2 - ADC2 - 5V rail
C3 - ADC3 - 3.3V rail
C4 - ADC4 - Motor 1 Current
C5 - ADC5 - Motor 2 Current
C6 - Reset
ADC6 - Motor 3 Current
ADC7 - Motor 4 Current
D0
D1
D2
D3 - Motor Brake
D4 - Motor 1 Direction
D5 - Motor 1 PWM
D6 - Motor 2 PWM
D7 - Motor 2 Direction

To start use just the one timer for PWM, Testing other pin PWM outputs on the same timer later




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

void PWM_Init(void) {
    // Set Timer0 to Fast PWM mode, non-inverting output on OC0A and OC0B
    SET_BIT(DDRD, PD6); // OC0A pin as output motor 1 and 3 PWM
    SET_BIT(DDRD, PD5); // OC0B pin as output motor 2 and 4 PWM

    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
    // Set prescaler to 64 and start the timer
    TCCR0B = (1 << CS01) | (1 << CS00);

    OCR0A = 0;
    OCR0B = 0;
}

void set_motor(uint8_t motor, uint8_t dir, uint8_t speed) {
    // motor: 1-4, dir: 0x00 for forward, 0x20 for reverse, speed: 0-127
    // Speed controlled by PWM, so we can use the speed value to set the duty cycle of the PWM signal for the corresponding motor
    switch (motor)
    {
    case 1:
        // direction control
        if (dir == 0x00) {
            // Set direction pin for motor 1 to forward
            SET_BIT(PORTD, PD4);
        } else {
            // Set direction pin for motor 1 to reverse
            CLEAR_BIT(PORTD, PD4);
        }

        // speed control
        OCR0A = speed; // Assuming motor 1 is controlled by Timer0 PWM on OC0A (PD6)

        break;
    case 2:
        // direction control
        if (dir == 0x00) {
            // Set direction pin for motor 2 to forward
            SET_BIT(PORTD, PD7);
        } else {
            // Set direction pin for motor 2 to reverse
            CLEAR_BIT(PORTD, PD7);
        }
        // speed control
        OCR0B = speed; // Assuming motor 2 is controlled by Timer0 PWM on OC0B (PD5)

        break;
    case 3:
        // direction control
        if (dir == 0x00) {
            // Set direction pin for motor 3 to forward
            SET_BIT(PORTD, PD4); // Example pin, change as needed
        } else {
            // Set direction pin for motor 3 to reverse
            CLEAR_BIT(PORTD, PD4); // Example pin, change as needed
        }
        // speed control
        OCR0A = speed; // Assuming motor 3 is controlled by Timer1 PWM on OC0A (PD6), change as needed

        break;
    case 4:
        // direction control
        if (dir == 0x00) {
            // Set direction pin for motor 4 to forward
            SET_BIT(PORTD, PD7); // Example pin, change as needed
        } else {
            // Set direction pin for motor 4 to reverse
            CLEAR_BIT(PORTD, PD7); // Example pin, change as needed
        }
        // speed control
        OCR0B = speed; // Assuming motor 4 is controlled by Timer1 PWM on OC0B (PD5), change as needed 

        break;
    default:
        break;
    }

}



int main(void) {
    char subkey = 0x00;
    char subvalue = 0xFF;
    SPI_SlaveInit();
    PWM_Init();
    SET_BIT(DDRB,PB0);
    SET_BIT(DDRD,PD4); // Motor 1 and 3 direction pin
    SET_BIT(DDRD,PD7); // Motor 2 and 4 direction pin


    while (1) {
        // Everytime we get a new key or value both key and values are refreshed causing both speeds to update.
        if (flag == 1) {
            cli();
            subkey = key;
            sei();
            flag = 0;
        }
        else if (flag == 2) {
            cli();
            subvalue = value;
            sei();
            flag = 0;
        }
        /*
        if (subkey == 0xFF && subvalue == 0x7F) {
            SET_BIT(PORTB,PB0);
            //SPDR = value; 
            subkey = 0x00; // Clear the subkey and subvalue after processing
            subvalue = 0xFF;

            avrkey = 0x21; // Send back something as a test
            avrvalue = 0x34;
        } 
        else if (subkey == 0xFF && subvalue == 0x01) {
            CLEAR_BIT(PORTB,PB0);
            //SPDR = 0x12;
            subkey = 0x00; // Clear the subkey and subvalue after processing
            subvalue = 0xFF;

            avrkey = 0xFF; // Send back something as a test
            avrvalue = 0xE4;
        }
            */

        // Motor control
        if ((subkey & 0xF0) == 0xC0 && subvalue != 0xFF) { // Check if the key is a motor control command and value is not the default 0xFF 
            uint8_t motor = subkey & 0x07; // Extract motor number (1-4)
            uint8_t dir   = subkey & 0x08; // 0x00 for forward, 0x08 for reverse

            set_motor(motor, dir, subvalue*2); // Scale speed value to 0-255 for PWM duty cycle

            //avrkey = 0xFE;
            //avrvalue = subkey;
            subkey = 0x00; // Clear the subkey and subvalue after processing
            subvalue = 0xFF; 
            
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
        flag = 1;
        SPDR = avrkey;
    }
    else {
        value = data;
        flag = 2;
        SPDR = avrvalue;
    }

}