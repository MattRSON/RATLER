#include <stdio.h>
#include <stdlib.h>
#include <bcm2711.h>

int main(int argc, char** argv) {

 if (!bcm2711_init()) {
  return 1;
 }
 if (!bcm2711_spi_begin()) {
  return 1;
 }

 bcm2711_spi_setBitOrder(BCM2711_SPI_BIT_ORDER_MSBFIRST);
 bcm2711_spi_setDataMode(BCM2711_SPI_MODE0);
 bcm2711_spi_setClockDivider(BCM2711_SPI_CLOCK_DIVIDER_65536);
 bcm2711_spi_chipSelect(BCM2711_SPI_CS0);
 bcm2711_spi_setChipSelectPolarity(BCM2711_SPI_CS0, LOW);

 uint8_t read_data = bcm2711_spi_transfer(0xAA);

 if( read_data== 0xAA) printf("data received correctly");

 bcm2711_spi_end();
 bcm2711_close();
 return (EXIT_SUCCESS);
}