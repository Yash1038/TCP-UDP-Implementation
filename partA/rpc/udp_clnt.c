// This is client 1 side code for the UDP connection

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

    // Loop begins for sending and recieving messages

    while(1){

        char input[1024];
        printf("Enter your move(rock, paper, scissors as 0,1,2): ");
        scanf("%[^\n]%*c", input);

        // Sending the message to the server
        n = sendto(sockfd, (const char *)input, strlen(input), MSG_CONFIRM, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if(n < 0){
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // recieving the Result
        int m;
        m = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&serverAddr, &len);
        if(m < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer[m] = '\0';
        printf("Result from Server: %s\n", buffer);

        // Sending the consent for next round
        char consent[1024];
        printf("Do you want to continue(Y/N): ");
        scanf("%[^\n]%*c", consent);
        n = sendto(sockfd, (const char *)consent, strlen(consent), MSG_CONFIRM, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));
        if(n < 0){
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // Recieving the consent for next round
        m = recvfrom(sockfd, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr *)&serverAddr, &len);
        if(m < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer[m] = '\0';

        if(strcmp(buffer, "Exit") == 0){
            printf("Game Over\n");
            break;
        }else{
            printf("Next Round\n");
        }
    }

    close(sockfd);
    return 0;
}