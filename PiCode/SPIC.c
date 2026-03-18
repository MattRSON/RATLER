// UDP Server listening for drive instructions
#include <stdio.h>      // printf
#include <stdlib.h>     // exit, malloc, free
#include <string.h>     // memset, memcpy, strcmp
#include <unistd.h>     // close, usleep
#include <arpa/inet.h>  // htons, htonl, sockaddr_in, INADDR_ANY
#include <sys/socket.h> // socket, bind, recvfrom
#include <sys/time.h>   // struct timeval, gettimeofday
#include <pigpio.h>     // pigpio library for SPI communication with motor controllers
//#include <time.h>       // time functions for timeout handling

#define PORT 5000 // UDP port
#define PACKET_SIZE 9 // Size of the packet sent from the Steam Deck (x1, y1, x2, y2, L2, R2, dpad_x, dpad_y, buttons)
#define TIMEOUT_MS 200 // Timeout after disconnection (ms)
#define SPI_SPEED 262144 // SPI speed in Hz

int main() {
    // Variables to hold parsed input values initialized to zero
    int8_t x1 = 0; // left stick X
    int8_t y1 = 0; // left stick Y
    int8_t x2 = 0; // right stick X
    int8_t y2 = 0; // right stick Y
    uint8_t L2 = 0; // left trigger
    uint8_t R2 = 0; // right trigger
    int8_t dpad_x = 0; // d-pad X
    int8_t dpad_y = 0; // d-pad Y
    uint8_t buttons = 0; // button bitfield
    bool flag = false; // New data received flag

    // AVR Struct for spi communication
    typedef struct {
        uint8_t sync;      // 0xAA
        int8_t motor[4]; // Motor speed values (-127 to 127)
        // uint8_t buttons;
        uint8_t checksum;
    } AVRData;

    // Set up UDP socket for receiving packets from the Steam Deck
    int sockfd; // socket descriptor for UDP communication
    struct sockaddr_in server_addr, client_addr; // server/client address info
    socklen_t addr_len = sizeof(client_addr); // size of client address struct
    uint8_t buffer[PACKET_SIZE]; // buffer to hold incoming packet data

    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // create a UDP socket

    server_addr.sin_family = AF_INET; // use IPv4
    server_addr.sin_port = htons(PORT); // set listening port (network byte order)
    server_addr.sin_addr.s_addr = INADDR_ANY; // accept packets on any local interface

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)); // bind socket to the specified port

    printf("UDP Server listening on port %d\n", PORT); // log that server is ready

    // Initialize pigpio library for SPI communication with motor controllers
    if (gpioInitialise()<0) return -1;

    unsigned char DataTX[1];
    unsigned char DataRX[1];

    int handle = spiOpen(0, SPI_SPEED, 0);
    int input;
    printf("SPI Connected to AVR as speed %d\n", SPI_SPEED); // log SPI connection info

    // Variables for timeout handling
    struct timeval last_packet, now; // track the timestamp of the last received packet
    gettimeofday(&last_packet, NULL); // initialize last_packet to current time

    // Main loop to continuously listen for incoming packets and process them
    while (1) {

        // attempt to read a packet without blocking
        int n = recvfrom(sockfd, buffer, PACKET_SIZE, MSG_DONTWAIT,(struct sockaddr*)&client_addr, &addr_len);

        if (n == PACKET_SIZE) { // only process when full packet is received
            gettimeofday(&last_packet, NULL); // update last received time

            int8_t x1 = (int8_t)buffer[0]; // left stick X
            int8_t y1 = (int8_t)buffer[1]; // left stick Y
            int8_t x2 = (int8_t)buffer[2]; // right stick X
            int8_t y2 = (int8_t)buffer[3]; // right stick Y
            uint8_t L2 = buffer[4]; // left trigger
            uint8_t R2 = buffer[5]; // right trigger
            int8_t dpad_x = (int8_t)buffer[6]; // d-pad X
            int8_t dpad_y = (int8_t)buffer[7]; // d-pad Y
            uint8_t buttons = buffer[8]; // button bitfield
            bool flag = true; // New data received flag

            printf("X1:%d Y1:%d X2:%d Y2:%d L2:%u R2:%u DPX:%d DPY:%d BTN:%u\n", x1, y1, x2, y2, L2, R2, dpad_x, dpad_y, buttons); // print parsed inputs

        }
        if (flag) {
            // SPI implementation would go here, sending the parsed inputs to the motor controllers
            AVRData DataTX;
                DataTX.sync = 0xAA; // Sync byte to indicate start of packet
                DataTX.motor[0] = L2/2; // Mapping: left trigger controls motor 1 speed
                DataTX.motor[1] = R2/2; // Mapping: right trigger controls motor 2 speed
                DataTX.motor[2] = L2/2; // Mapping: left trigger also controls motor 3 speed
                DataTX.motor[3] = R2/2; // Mapping: right trigger also controls motor 4 speed
                // DataTX.buttons = buttons; // Optionally include button states in the packet
                DataTX.checksum = (DataTX.sync + DataTX.motor[0] + DataTX.motor[1] + DataTX.motor[2] + DataTX.motor[3]) & 0xFF; // Simple checksum calculation

            spiXfer(handle, (unsigned char*)&DataTX, DataRX, sizeof(AVRData)); // Send the structured data over SPI
            //DataTX = AVRData;
            //spiXfer(handle, DataTX, DataRX, 1);
        }

        gettimeofday(&now, NULL); // get current time for timeout calculation
        long elapsed_ms =
            (now.tv_sec - last_packet.tv_sec) * 1000 + (now.tv_usec - last_packet.tv_usec) / 1000; // ms since last packet

        if (elapsed_ms > TIMEOUT_MS) { // if no packet has arrived within timeout
            // Watchdog: Controller disconnected
            DataTX.sync = 0xAA;
            DataTX.motor[0] = 0;
            DataTX.motor[1] = 0;
            DataTX.motor[2] = 0;
            DataTX.motor[3] = 0;
            DataTX.checksum = (DataTX.sync + DataTX.motor[0] + DataTX.motor[1] + DataTX.motor[2] + DataTX.motor[3]) & 0xFF;
            spiXfer(handle, (unsigned char*)&DataTX, DataRX, sizeof(AVRData)); // Send the structured data over SPI
            printf("No packet received for %ld ms, stopping motors\n", elapsed_ms); // log timeout event
        }

        usleep(1000); // sleep 1ms to avoid busy looping
    }

    close(sockfd); // close the socket (unreachable in current infinite loop)
    spiClose(handle);
    gpioTerminate();
    return 0; // indicate successful exit
}