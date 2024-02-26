#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

volatile int data = 0;

void SPI_SlaveInit(void) {
    // Set MISO as output
    SET_BIT(DDRB,DDB4);
    // Enable SPI
    SET_BIT(SPCR, SPE);
    // Enable SPI interupts
    SET_BIT(SPCR,SPIE);
    SET_BIT(PORTB,PB1);
    
}

/*
char SPI_Receive(void) {
    // Wait for reception to complete
    while(!(SPSR & (1<<SPIF))) {}
    //return data register
    return SPDR;
}
*/
ISR (SPI_STC_vect){
    data = SPDR;
    FLIP_BIT(PORTB,PB1);
}

int main(void) {

    SPI_SlaveInit();
    sei();
    SET_BIT(DDRB,DDB0);
    SET_BIT(DDRB,DDB1); // Test LED
    // try an spi interupt
    while (1) {
        //data = (int) SPI_Receive;
        if (data == 1) {
            SET_BIT(PORTB,PB0);
        }
        else {
            CLEAR_BIT(PORTB,PB0);
        }
    }


}