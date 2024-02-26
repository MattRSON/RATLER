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
    DDRB |= (1<<DDB4);
    // Enable SPI
    SET_BIT(SPCR, SPE);
    SET_BIT(SPCR,SPIE);
}

/*
char SPI_Receive(void) {
    // Wait for reception to complete
    while(!(SPSR & (1<<SPIF))) {}
    //return data register
    return SPDR;
}
*/
ISR (SPIF_vector){
    data = SPDR;
}

int main(void) {

    SPI_SlaveInit;
    sei();
    DDRB |= (1<<DDB0);
    // try an spi interupt
    while (1) {
        //data = (int) SPI_Receive;
        if (data == 1) {
            SET_BIT(PORTB,DDB0);
        }
        else {
            CLEAR_BIT(PORTB,DDB0);
        }
    }


}