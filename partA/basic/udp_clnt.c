// This is the client side code for the UDP connection

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

    struct sockaddr_in serverAddr;
    int sockfd, n;
    socklen_t len;
    char buffer[1024];

    // Creating a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Filling server information
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    len = sizeof(serverAddr);

    // Sending the message to the server
    // take the input from the user
    char input[1024];
    printf("Enter the message to be sent to the server: ");
    scanf("%[^\n]%*c", input);
    // char *hello = "Hello from the client.";
    n = sendto(sockfd, (const char *)input, strlen(input), MSG_CONFIRM, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(n < 0){
        perror("Sendto failed");
        exit(EXIT_FAILURE);
    }else{
        printf("Message sent to the server.\n");
    }

    // Receiving the message from the server
    int m;
    m = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&serverAddr, &len);
    if(m < 0){
        perror("Recvfrom failed");
        exit(EXIT_FAILURE);
    }
    buffer[m] = '\0';
    printf("Server : %s\n", buffer);

    close(sockfd);
    return 0;
}