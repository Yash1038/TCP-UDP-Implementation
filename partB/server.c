// This is the server side implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/select.h>

#define PORT 8080

#define MAX_CHUNK_SIZE 16

#define ff(i,a,b) for(int i=a;i<b;i++)
#define f(i,n) ff(i,0,n)

typedef struct Packet {
    int seq_num;
    char data[MAX_CHUNK_SIZE];
} D_Packet;

int main(){

    struct sockaddr_in clntAddr;
    int sockfd, n;
    socklen_t len;
    char buffer[1024];

    // Creating a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Binding 
    memset(&clntAddr, 0, sizeof(clntAddr));

    clntAddr.sin_family = AF_INET;
    clntAddr.sin_port = htons(PORT);
    clntAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Setting the socket to non-blocking
    int flags = fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, flags);

    char message[] = "Hello World! This is a test message. Here we are trying to implement TCP using UDP";
    int message_len = strlen(message);

    int no_chunks = message_len / MAX_CHUNK_SIZE;
    if(message_len % MAX_CHUNK_SIZE != 0){
        no_chunks++;
    }

    printf("Total number of chunks: %d\n", no_chunks);


    // Now dividing the whole message into chunks
    D_Packet chunks[no_chunks];
    memset(chunks, 0, sizeof(chunks));

    f(i,no_chunks){
        chunks[i].seq_num = i;
        strncpy(chunks[i].data, message+(i*MAX_CHUNK_SIZE), MAX_CHUNK_SIZE);
    }

    int ack[no_chunks];
    memset(ack, 0, sizeof(ack));

    while(1){
        int resend = 0;

        f(i,no_chunks){
            if(ack[i] == 0){
                resend = 1;
                // Sending ith chunk
                sendto(sockfd, &chunks[i], sizeof(chunks[i]), 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr));
                printf("Sent chunk %d\n", i);
                usleep(10000);

            }
        }

        if(resend == 0){
            D_Packet termination;
            termination.seq_num = -1;
            strcpy(termination.data, "END");
            termination.data[3] = '\0';
            sendto(sockfd, &termination, sizeof(termination), 0, (struct sockaddr*)&clntAddr, sizeof(clntAddr));
            printf("Sent termination packet\n");
            break;
        }

        // Now waiting for the acks
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        // Making socket stop after a timeout of 0.1 second
        struct timeval timer;
        timer.tv_sec = 0;
        timer.tv_usec = 10000;

        D_Packet ack_packet;


        n = select(sockfd+1, &readfds, NULL, NULL, &timer);

        if(n > 0 && FD_ISSET(sockfd, &readfds)){
            // Some ack has arrived
            // int m = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clntAddr, &len);
            int m = recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, (struct sockaddr*)&clntAddr, &len);
            if(m < 0){
                perror("Error in receiving ack");
                exit(EXIT_FAILURE);
            }
            // buffer[m] = '\0';
            printf("Received ack: %d\n", ack_packet.seq_num);
            // int ack_num = atoi(buffer);
            ack[ack_packet.seq_num] = 1;
            usleep(10000);
        }

    }

    close(sockfd);
    return 0;

}
