// Server side implementation using UDP

/*
0 - Rock
1 - Paper
2 - Scissors
*/

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
    
    struct sockaddr_in serverAddr, client1Addr, client2Addr;
    int sockfd, n;
    socklen_t len1, len2;
    // char buffer[1024];

    // Creating a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Binding the socket to the port 8080
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&client1Addr, 0, sizeof(client1Addr));
    memset(&client2Addr, 0, sizeof(client2Addr));

    // Filling server information
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    int roundno = 1;

    // Here starts a loop to receive messages from both clients

    while(1){
        // Receiving the message from client1
        char buffer1[1024];
        len1 = sizeof(client1Addr);

        n = recvfrom(sockfd, (char *)buffer1, 1024, MSG_WAITALL, (struct sockaddr *)&client1Addr, &len1);
        if(n < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer1[n] = '\0';

        // Receiving the message from client2
        char buffer2[1024];
        len2 = sizeof(client2Addr);

        n = recvfrom(sockfd, (char *)buffer2, 1024, MSG_WAITALL, (struct sockaddr *)&client2Addr, &len2);
        if(n < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer2[n] = '\0';

        int result = -1;
        // 0 - Draw
        // 1 - Client1 wins
        // 2 - Client2 wins

        // Comparing the messages and sending the result to both clients

        if(strcmp(buffer1,buffer2) == 0){
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

        char result_str_1[1024];
        char result_str_2[1024];
        char result_str[1024];
        if(result == 0){
            strcpy(result_str,"Draw");
            strcpy(result_str_1,"Draw");
            strcpy(result_str_2,"Draw");
        }
        else if(result == 1){
            strcpy(result_str,"Client1 wins");
            strcpy(result_str_1,"You won");
            strcpy(result_str_2,"You lost");
        }
        else if(result == 2){
            strcpy(result_str,"Client2 wins");
            strcpy(result_str_1,"You lost");
            strcpy(result_str_2,"You won");
        }
        else{
            strcpy(result_str,"Invalid Operation");
            strcpy(result_str_1,"Invalid Operation");
            strcpy(result_str_2,"Invalid Operation");
        }

        printf("End of round %d\n",roundno);

        // Sending the result to both the clients

        int n1 = sendto(sockfd, (const char *)result_str_1, strlen(result_str_1), MSG_CONFIRM, (const struct sockaddr *)&client1Addr, len1);
        int n2 = sendto(sockfd, (const char *)result_str_2, strlen(result_str_2), MSG_CONFIRM, (const struct sockaddr *)&client2Addr, len2);

        if(n1 < 0 || n2 < 0){
            perror("Sendto failed");
            exit(EXIT_FAILURE);
        }

        // Now again recieving the messages from both clients whether to continue or not

        n = recvfrom(sockfd, (char *)buffer1, 1024, MSG_WAITALL, (struct sockaddr *)&client1Addr, &len1);
        if(n < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }

        n = recvfrom(sockfd, (char *)buffer2, 1024, MSG_WAITALL, (struct sockaddr *)&client2Addr, &len2);
        if(n < 0){
            perror("Recvfrom failed");
            exit(EXIT_FAILURE);
        }


        if(strcmp(buffer1,"Y") == 0 && strcmp(buffer2,"Y") == 0){
            printf("Both clients want to continue\n");
            roundno++;
            n1 = sendto(sockfd, (const char *)buffer1, strlen(buffer1), MSG_CONFIRM, (const struct sockaddr *)&client1Addr, len1);
            n2 = sendto(sockfd, (const char *)buffer2, strlen(buffer2), MSG_CONFIRM, (const struct sockaddr *)&client2Addr, len2);
            if(n1 < 0 || n2 < 0){
                perror("Sendto failed");
                exit(EXIT_FAILURE);
            }
        }
        else{
            // send the exit message to both clients
            printf("Atleast one of the clients wants to exit\n");
            char exit_str[1024];
            strcpy(exit_str,"Exit");
            n1 = sendto(sockfd, (const char *)exit_str, strlen(exit_str), MSG_CONFIRM, (const struct sockaddr *)&client1Addr, len1);
            n2 = sendto(sockfd, (const char *)exit_str, strlen(exit_str), MSG_CONFIRM, (const struct sockaddr *)&client2Addr, len2);
            if(n1 < 0 || n2 < 0){
                perror("Sendto failed");
                exit(EXIT_FAILURE);
            }
            break;
        }

    }

    close(sockfd);


}