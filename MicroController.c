#include <avr/io.h>

#define   SET_BIT(Reg, Bit) (Reg |=  (1 << (Bit)))
#define CLEAR_BIT(Reg, Bit) (Reg &= ~(1 << (Bit)))
#define  FLIP_BIT(Reg, Bit) (Reg ^=  (1 << (Bit)))
#define  TEST_BIT(Reg, Bit) ((Reg >> (Bit)) & 1)

void SPI_SlaveInit(void) {
    // Set MISO as output
    DDRB |= (1<<DDB4);
    // Enable SPI
    SPCR = (1<<SPE);
}

char SPI_Receive(void) {
    // Wait for reception to complete
    while(!(SPSR & (1<<SPIF))) {}
    //return data register
    return SPDR;
}

int main(void) {

    SPI_SlaveInit;

    DDRB |= (1<<DDB0);
    int data = 0;

    while (1) {
        data = (int) SPI_Receive;
        if (data == 1) {
            SET_BIT(PORTB,DDB0);
        }
        else {
            CLEAR_BIT(PORTB,DDB0);
        }
    }


}