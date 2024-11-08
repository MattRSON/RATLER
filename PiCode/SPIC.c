
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


int main(){
    // Init gpio
    if (gpioInitialise()<0) return -1;

    unsigned char DataTX[1];
    unsigned char DataRX[1];
    int handle = spiOpen(0, 262144, 0);
    int input;

    while(1){
        printf("Enter Instructions\n");
        scanf("%d", &input);

        DataTX[0] = input;
        spiXfer(handle, DataTX, DataRX, 1);
        sleep(1);


        
        // DataTX[0] = 0;
        // spiXfer(handle, DataTX, DataRX, 1);
        // sleep(1);
        // DataTX[0] = 1;
        // spiXfer(handle, DataTX, DataRX, 1);
        // sleep(1);
        
    }
   

    // Terminate the library
    spiClose(handle);
    gpioTerminate();
    return 0;
}