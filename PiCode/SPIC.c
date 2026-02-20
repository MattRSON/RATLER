//UDP Server listening for drive instructions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define PORT 5000 //UDP port
#define PACKET_SIZE 5
#define TIMEOUT_MS 200 //Timeout after disconnection

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    uint8_t buffer[PACKET_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("UDP Server listening on port %d\n", PORT);

    struct timeval last_packet, now;
    gettimeofday(&last_packet, NULL);

    while (1) {

        int n = recvfrom(sockfd, buffer, PACKET_SIZE, MSG_DONTWAIT,
                         (struct sockaddr*)&client_addr, &addr_len);

        if (n == PACKET_SIZE) {
            gettimeofday(&last_packet, NULL);

            int8_t x1 = (int8_t)buffer[0];
            int8_t y1 = (int8_t)buffer[1];
            int8_t x2 = (int8_t)buffer[2];
            int8_t y2 = (int8_t)buffer[3];
            uint8_t buttons = buffer[4];

            printf("X1:%d Y1:%d X2:%d Y2:%d BTN:%u\n", x1, y1, x2, y2, buttons);

            // TODO: forward to SPI here
        }

        gettimeofday(&now, NULL);
        long elapsed_ms =
            (now.tv_sec - last_packet.tv_sec) * 1000 + (now.tv_usec - last_packet.tv_usec) / 1000;

        if (elapsed_ms > TIMEOUT_MS) {
            // Watchdog: stop motors
            // send zero speeds to SPI
        }

        usleep(1000);
    }

    close(sockfd);
    return 0;
}