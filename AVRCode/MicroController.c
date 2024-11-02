#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
//#include <avr/interrupt.h>
#include <util/delay.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

volatile char data = 0;

void SPI_SlaveInit(void) {
    // Set MISO as output
    SET_BIT(DDRB,PB4);
    // Enable SPI
    SET_BIT(SPCR, SPE);
    
}

//int Rx
char SPI_Receive() {
    // Wait for reception to complete
    //SPDR = Rx;

    while(!(SPSR & (1<<SPIF))) {}
    //return data register
    return SPDR;
}


int main(void) {
    //int Send = 0x34;

    SPI_SlaveInit();
    SET_BIT(DDRB,PB0);
    SET_BIT(PORTB,PB0);

    while (1) {
        data = SPI_Receive();//(Send);
        if (data == 0x80) {
            SET_BIT(PORTB,PB0);
            SPDR = 0xFF; 
        } 
        else {
            CLEAR_BIT(PORTB,PB0);
            SPDR = 0x12;
        }
    }
    return 0;


}
