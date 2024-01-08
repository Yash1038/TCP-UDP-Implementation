// This is the client side implementation

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
#include <time.h>

#define PORT 8080

#define MAX_CHUNK_SIZE 16
#define MAX_CHUNKS 100

#define ff(i,a,b) for(int i=a;i<b;i++)
#define f(i,n) ff(i,0,n)

typedef struct Packet {
    int seq_num;
    char data[MAX_CHUNK_SIZE];
} D_Packet;


int main(){

    struct sockaddr_in clnt_addr;
    int sockfd, n;
    socklen_t len;
    char buffer[1024];
    srand(time(0)); // This is for random missing acknowledgement

    // Creating UDP Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Binding details
    memset(&clnt_addr, 0, sizeof(clnt_addr));

    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_port = htons(PORT);
    clnt_addr.sin_addr.s_addr = INADDR_ANY;

    // Binding 
    if(bind(sockfd, (const struct sockaddr *)&clnt_addr, sizeof(clnt_addr)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is running\n");

    D_Packet packets[MAX_CHUNKS];
    int no_packets = 0;
    struct sockaddr_in server_addr;
    int total_chunks = 0;

    int ackn[MAX_CHUNKS];
    socklen_t server_len = sizeof(server_addr);
    struct Packet received_packet;

    while(1){
        
        int x = recvfrom(sockfd, &received_packet, sizeof(struct Packet), 0, (struct sockaddr *)&server_addr, &server_len);
        if(x > 0){
            
            if(received_packet.seq_num == -1){
                // This is when the packet recieved is terminal one
                break;
            }

            if(ackn[received_packet.seq_num] == 1){
                // This is when the packet is duplicate
                continue;
            }else{
                packets[received_packet.seq_num] = received_packet;

                // if(rand() % 3 == 0){
                //     // This is when the acknowledgement is lost
                //     // printf("Acknowledgement lost for chunk %d\n", received_packet.seq_num);
                //     continue;
                // }

                total_chunks++;
                ackn[received_packet.seq_num] = 1;

                printf("Received chunk %d: %s\n", received_packet.seq_num, received_packet.data);
                // Sending the chunk number we received
                int number = received_packet.seq_num;
                // sendto(sockfd, (const char *)str, strlen(str), 0, (const struct sockaddr *)&server_addr, server_len);
                sendto(sockfd, &number, sizeof(number), 0, (const struct sockaddr *)&server_addr, server_len);
            }
        }
    }

    // Reconstruct the original data
    char message[total_chunks * MAX_CHUNK_SIZE + 100];
    memset(message, 0, sizeof(message));

    f(i, total_chunks){
        strncat(message, packets[i].data, MAX_CHUNK_SIZE);
    }

    printf("The original message is: %s\n", message);
    
    close(sockfd);

    return 0;

}