// This is the client implementation of TCP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080

// Take input from the client

int main(){
    int sockfd, n;
    struct sockaddr_in server_address;

    // Creating a TCS socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket Creation Error");
        exit(EXIT_FAILURE);
    }

    // Filling server information
    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Connecting to the server
    if(connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Sending data to the server
    char buffer[1024];
    printf("Enter the data to be sent to the server: ");
    fgets(buffer, 1024, stdin);
    n = send(sockfd, buffer, strlen(buffer), 0);
    if(n < 0){
        perror("Send failed");
        exit(EXIT_FAILURE);
    }else{
        printf("Message sent to the server\n");
    }

    // recieving data from the server
    memset(buffer, 0, sizeof(buffer));
    n = recv(sockfd, buffer, sizeof(buffer), 0);
    if(n < 0){
        perror("Recieve failed");
        exit(EXIT_FAILURE);
    }
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;

}