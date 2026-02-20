// simple_udp_server.c

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 5000

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Waiting for message on port %d...\n", PORT);

    // Receive message
    int n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0,
                     (struct sockaddr*)&client_addr, &addr_len);

    buffer[n] = '\0';   // Null-terminate

    printf("Received: %s\n", buffer);

    close(sockfd);
    return 0;
}