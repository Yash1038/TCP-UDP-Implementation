// This is the TCP server implementation for the game rock paper scissors

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

    struct sockaddr_in server_address, client1_address, client2_address;
    socklen_t client1_address_len = sizeof(client1_address);
    socklen_t client2_address_len = sizeof(client2_address);
    int sockfd, n;

    // Creating TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket Creation Error");
        exit(EXIT_FAILURE);
    }

    // Binding the socket to address and the port
    memset(&server_address, 0, sizeof(server_address));
    memset(&client1_address, 0, sizeof(client1_address));
    memset(&client2_address, 0, sizeof(client2_address));

    // Filling server information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failure");
        exit(EXIT_FAILURE);
    }

    // Checking for incoming connections
    if (listen(sockfd, 2) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connections...\n");

    // Accepting a connection
    int client1_socket = accept(sockfd, (struct sockaddr*)&client1_address, &client1_address_len);
    if (client1_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connection from: %s:%d\n", inet_ntoa(client1_address.sin_addr), ntohs(client1_address.sin_port));

    // Accepting a connection
    int client2_socket = accept(sockfd, (struct sockaddr*)&client2_address, &client2_address_len);
    if (client2_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connection from: %s:%d\n", inet_ntoa(client2_address.sin_addr), ntohs(client2_address.sin_port));

    // While loop for conversation
    int round_no = 1;

    while(1){
        // From here on Recieving data part starts
        char buffer1[1024];
        ssize_t bytes_received1 = recv(client1_socket, buffer1, sizeof(buffer1), 0);
        if (bytes_received1 < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        buffer1[bytes_received1] = '\0';

        char buffer2[1024];
        ssize_t bytes_received2 = recv(client2_socket, buffer2, sizeof(buffer2), 0);
        if (bytes_received2 < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        buffer2[bytes_received2] = '\0';

        int result = -1;
        /*
        0 - Draw
        1 - Client 1 wins
        2 - Client 2 wins
        -1 - Invalid input
        */

       // Comparing the messages
       if(strcmp(buffer1, buffer2) == 0){
            result = 0;
       }

       if(strcmp(buffer1,"0") == 0 && strcmp(buffer2,"1") == 0){
            result = 2;
        }

        if(strcmp(buffer1,"0") == 0 && strcmp(buffer2,"2") == 0){
            result = 1;
        }

        if(strcmp(buffer1,"1") == 0 && strcmp(buffer2,"0") == 0){
            result = 1;
        }

        if(strcmp(buffer1,"1") == 0 && strcmp(buffer2,"2") == 0){
            result = 2;
        }

        if(strcmp(buffer1,"2") == 0 && strcmp(buffer2,"0") == 0){
            result = 2;
        }

        if(strcmp(buffer1,"2") == 0 && strcmp(buffer2,"1") == 0){
            result = 1;
        }

        // Sending the result to both the clients

        // char result_buffer[1024];
        char result_buffer1[1024];
        char result_buffer2[1024];

        if(result == 0){
            // strcpy(result_buffer, "Draw");
            strcpy(result_buffer1, "Draw");
            strcpy(result_buffer2, "Draw");
        }
        else if(result == 1){
            // strcpy(result_buffer,"Client1 wins");
            strcpy(result_buffer1,"You won");
            strcpy(result_buffer2,"You lost");
        }else if(result == 2){
            // strcpy(result_buffer,"Client2 wins");
            strcpy(result_buffer1,"You lost");
            strcpy(result_buffer2,"You won");
        }else{
            // strcpy(result_buffer,"Invalid input");
            strcpy(result_buffer1,"Invalid input");
            strcpy(result_buffer2,"Invalid input");
        }

        printf("End of round %d\n", round_no);

        // Sending the result to both the clients

        ssize_t bytes_sent_1 = send(client1_socket, result_buffer1, strlen(result_buffer1), 0);
        ssize_t bytes_sent_2 = send(client2_socket, result_buffer2, strlen(result_buffer2), 0);

        if (bytes_sent_1 < 0 || bytes_sent_2 < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }

        round_no++;

        // Now again taking consent

        // Reuse the buffer
        memset(buffer1, 0, sizeof(buffer1));
        memset(buffer2, 0, sizeof(buffer2));

        // Receiving consent from client 1
        ssize_t bytes_received_1 = recv(client1_socket, buffer1, sizeof(buffer1), 0);
        if (bytes_received_1 < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        // Receiving consent from client 2
        ssize_t bytes_received_2 = recv(client2_socket, buffer2, sizeof(buffer2), 0);
        if (bytes_received_2 < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        // Checking if both the clients want to continue

        if(strcmp(buffer1, "Y") == 0 && strcmp(buffer2, "Y") == 0){
            // Both the clients want to continue
            printf("Both the clients want to continue\n");
            ssize_t bytes_sent_1 = send(client1_socket, "Y", strlen("Y"), 0);
            ssize_t bytes_sent_2 = send(client2_socket, "Y", strlen("Y"), 0);
            if(bytes_sent_1 < 0 || bytes_sent_2 < 0){
                perror("send");
                exit(EXIT_FAILURE);
            }
            continue;
        }else{
            // One of the clients don't want to continue
    
            printf("Atleast one of the clients don't want to continue\n");
            ssize_t bytes_sent_1 = send(client1_socket, "Exit", strlen("Exit"), 0);
            ssize_t bytes_sent_2 = send(client2_socket, "Exit", strlen("Exit"), 0);
            if(bytes_sent_1 < 0 || bytes_sent_2 < 0){
                perror("send");
                exit(EXIT_FAILURE);
            }
            break;
        }


    }

    // Closing the sockets
    close(client1_socket);
    close(client2_socket);
    close(sockfd);
}