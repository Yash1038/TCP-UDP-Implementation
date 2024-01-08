// This file contains the server side code for the UDP connection

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

int main(){

    struct sockaddr_in serverAddr, clientAddr;
    int sockfd, n;
    socklen_t len;
    char buffer[1024];


    // Creating a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Binding the socket to the port 8080
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));

    // Filling server information
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Receiving the message from the client
    len = sizeof(clientAddr);

    n = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&clientAddr, &len);
    if(n < 0){
        perror("Recvfrom failed");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);

    // Sending the message to the client
    char *hello = "Hello From Server";
    n = sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&clientAddr, len);
    if(n < 0){
        perror("Sendto failed");
        exit(EXIT_FAILURE);
    }
    printf("Server sent Message\n");

    // Closing the socket
    close(sockfd);

    return 0;

}
