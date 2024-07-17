// Written by Jai the c god (and Mathew a little bit)
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


int main(){
    // Init gpio
    if (gpioInitialise()<0) return -1;

    unsigned char Data[1];
    int handle = spiOpen(0, 6000000, 0);

    while(1){
        spiRead(handle, Data, 1);
    }
   

    // Terminate the library
    spiClose(handle);
    gpioTerminate();
    return 0;
}