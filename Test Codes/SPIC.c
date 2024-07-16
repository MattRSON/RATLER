#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

int main(){
    // Init gpio
    if (gpioInitialise()<0) return -1;


    unsigned char DataTX[1];
    unsigned char DataRX[1];
    int handle = spiOpen(0, 1000000, 0);

    DataTX[0] = 1;
    DataRX[0] = 10;

    while(1){
        spiXfer(handle, DataTX, DataRX, 1);
        sleep(1);
    }
   

    // Terminate the library
    spiClose(handle);
    gpioTerminate();
    return 0;
}