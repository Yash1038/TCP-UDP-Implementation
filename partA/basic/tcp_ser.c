// This is a server implementation of TCP

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

    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    int sockfd, n;

    // Creating TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        perror("Socket Creation Error");
        exit(EXIT_FAILURE);
    }

    // Binding the socket to address and the port
    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));

    // Filling server information
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Binding failure");
        exit(EXIT_FAILURE);
    }

    // Checking for incoming connections
    if (listen(sockfd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accepting a connection
    int client_socket = accept(sockfd, (struct sockaddr*)&client_address, &client_address_len);
    if (client_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connection from: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // From here on Recieving data part starts
    char buffer[1024];
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("Received data: %s", buffer);

    // Responding data back to client
    const char* response = "Server received the message.";
    ssize_t bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    printf("Server sent message\n");

    close(sockfd);
    close(client_socket);

}