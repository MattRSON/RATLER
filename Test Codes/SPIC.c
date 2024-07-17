// Written by Jai the c god (and Mathew a little bit)
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define Select1 2
#define Select2 3
#define Select3 4
#define Select4 17
#define Select5 27
#define Select6 22
#define Select7 14
#define Select8 15
#define Select9 18
#define Select10 23
#define Select11 24
#define Select12 25

FILE *fp;

int main(){
    // Init gpio
    if (gpioInitialise()<0) return -1;

    time_t rawtime;
    struct tm *timeinfo;
    char datetime_str[20]; // This will hold the formatted datetime string

    // Get current system time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the datetime string as per the specified format
    sprintf(datetime_str, "%04d%02d%02d_%02d%02d%02d",
            1900 + timeinfo->tm_year, // Year (1900 + current year)
            1 + timeinfo->tm_mon,     // Month (0-based index, so add 1)
            timeinfo->tm_mday,        // Day of the month
            timeinfo->tm_hour,        // Hour
            timeinfo->tm_min,         // Minute
            timeinfo->tm_sec);        // Second

    strcat(datetime_str,".csv");
    fp = fopen(datetime_str, "a");


    unsigned char Data[2];
    uint16_t BulkData[12];
    int handle = spiOpen(0, 6000000, 0);
    // Set pins as outputs
    /*
    gpioSetMode(Select1, PI_OUTPUT);
    gpioSetMode(Select2, PI_OUTPUT);
    gpioSetMode(Select3, PI_OUTPUT);
    gpioSetMode(Select4, PI_OUTPUT);
    gpioSetMode(Select5, PI_OUTPUT);
    gpioSetMode(Select6, PI_OUTPUT);
    gpioSetMode(Select7, PI_OUTPUT);
    gpioSetMode(Select8, PI_OUTPUT);
    gpioSetMode(Select9, PI_OUTPUT);
    gpioSetMode(Select10, PI_OUTPUT);
    gpioSetMode(Select11, PI_OUTPUT);
    gpioSetMode(Select12, PI_OUTPUT);

    // Writes all the pins high
    struct timespec begin, end; 
    
    /*
    gpioWrite(Select1, 1);
    gpioWrite(Select2, 1);
    gpioWrite(Select3, 1);
    gpioWrite(Select4, 1);
    gpioWrite(Select5, 1);
    gpioWrite(Select6, 1);
    gpioWrite(Select7, 1);
    gpioWrite(Select8, 1);
    gpioWrite(Select9, 1);
    gpioWrite(Select10, 1);
    gpioWrite(Select11, 1);
    gpioWrite(Select12, 1);
    */

    while(1){
        spiRead(handle, Data, 2);
    }
   

    // Terminate the library
    fclose(fp);
    spiClose(handle);
    gpioTerminate();
    return 0;
}