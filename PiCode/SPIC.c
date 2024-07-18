
// #include <stdio.h>
// #include <stdlib.h>
// #include <pigpio.h>
// #include <time.h>
// #include <unistd.h>
// #include <string.h>


// int main(){
//     // Init gpio
//     if (gpioInitialise()<0) return -1;

//     unsigned char DataTX[1];
//     unsigned char DataRX[1];
//     int handle = spiOpen(0, 1000000, 0);
//     int input;

//     while(1){
//         // printf("Enter Instructions\n");
//         // scanf("%d", &input);

//         // DataTX[0] = input;
//         // spiXfer(handle, DataTX, DataRX, 1);
//         // sleep(1);


        
//         DataTX[0] = 0;
//         spiXfer(handle, DataTX, DataRX, 1);
//         sleep(1);
//         DataTX[0] = 1;
//         spiXfer(handle, DataTX, DataRX, 1);
//         sleep(1);
        
//     }
   

//     // Terminate the library
//     spiClose(handle);
//     gpioTerminate();
//     return 0;
// }
#include <avr/io.h>
#include <util/delay.h>

 int main()
 {
     // Set built-in LED pin as output
     DDRB |= (1 << DDB0);
     while (1) {
         PORTB |=  (1 << PB0);   // LED on
         _delay_ms(10);         // wait 500ms
         PORTB &= ~(1 << PB0);   // LED off
         _delay_ms(10);         // wait 500ms
     }
     return 0;
 }