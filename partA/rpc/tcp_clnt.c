// This is the client implementation of TCP for Rock Paper Scissors game

/*
0 - rock
1 - paper
2 - scissors
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080

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

    // From here while loop starts
    while(1){

        char input[100];
        printf("Enter your move(rock, paper and scissors as 0,1,2): ");
        scanf("%[^\n]%*c", input);

        // Sending data to the server
        char buffer[1024];
        strcpy(buffer, input);
        n = send(sockfd, buffer, strlen(buffer), 0);
        if(n < 0){
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Recieving result from the server
        memset(buffer, 0, sizeof(buffer));
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        if(n < 0){
            perror("Recieve failed");
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

        // Asking user if he wants to continue
        printf("Do you want to continue(Y/N): ");
        scanf("%[^\n]%*c", input);
        n = send(sockfd, input, strlen(input), 0);
        if(n < 0){
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Recieving result from the server
        memset(buffer, 0, sizeof(buffer));
        n = recv(sockfd, buffer, sizeof(buffer), 0);
        if(n < 0){
            perror("Recieve failed");
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';

        if(strcmp(buffer, "Exit") == 0){
            printf("Thank you for playing\n");
            break;
        }else{
            printf("Let's play again\n");
        }


    }

    close(sockfd);

    return 0;
}