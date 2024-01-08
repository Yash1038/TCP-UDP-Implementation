# Networks

## Part A - Basic

### Running the files

1. Run the following command to run the file for TCP server and client
```bash
gcc tcp_ser.c -o tcp_server
gcc tcp_clnt.c -o tcp_client

./tcp_server
./tcp_client
```

2. Run the following command to run the file for UDP server and client
```bash
gcc udp_ser.c -o udp_server
gcc udp_clnt.c -o udp_client

./udp_server
./udp_client
```

### Important Notes Regarding each

1. The TCP server and client are connected to each other and the client sends a message to the server and the server sends back the same message to the client.

2. The UDP server and client are connected to each other and the client sends a message to the server and the server sends back the same message to the client.

3. Note that there is not an exact conversation created for this part of implementation instead the client sends a message and the server sends back another message to the client and end of conversation.


## Part A - RPC Implementation

### Running the files

1. Run the following command to run the file for RPC server and client using TCP
```bash
gcc tcp_ser.c -o tcp_server
gcc tcp_clnt.c -o tcp_client1
gcc tcp_clnt.c -o tcp_client2

./tcp_server
./tcp_client1
./tcp_client2
```

2. Run the following command to run the file for RPC server and client using UDP
```bash
gcc udp_ser.c -o udp_server
gcc udp_clnt.c -o udp_client1
gcc udp_clnt.c -o udp_client2

./udp_server
./udp_client1
./udp_client2
```

### Important Notes Regarding each

1. The clients are asked to choose a move from the following options:
    - Rock (denoted by '0')
    - Paper (denoted by '1')
    - Scissors (denoted by '2')

2. The clients are expected to strictly enter the above mentioned values only. Any other value will be considered as an invalid move and the client will be asked to enter a valid move for the next turn if it happens.

3. The server will display the winner of the game after each turn.

4. The clients are expected to give consent for the next round to start using the following options:
    - Yes (denoted by 'Y')
    - No (denoted by 'N')

5. Any other value will be considered as a No and the game will end.

6. Acknowledgement is given to the client from the server after each turn in the form of Result. Resending of the same message is not implemented when the acknowledgement is not received.

7. The difference between TCP and UDP are that the clients cannot send a message to the server unless they are connected. In case of UDP, the clients can enter a message to the server without being connected to the server.


## Part B - Implementing TCP with the help of UDP

### Implementation

- The client establishes a UDP socket and binds it to a specific port. It continuously receives packets using recvfrom(), where it checks for a termination signal or if a packet is a duplicate. If a new and non-duplicate packet is received, it stores the data and sends an acknowledgment. Once all packets are received, the server reconstructs the original message by concatenating the stored chunks. Finally, it closes the socket.

- The server creates a UDP socket and assigns a specific port. It divides the message into chunks, assigns a sequence number to each, and sets the socket to non-blocking mode. The client then enters a loop to continuously send packets using sendto(). It tracks acknowledgments and retransmits unacknowledged packets. When all packets are acknowledged, the client sends a termination packet. The client's socket is then closed. This implementation aims to achieve reliable data transfer using UDP by employing retransmissions and acknowledgments similar to TCP, albeit with some inherent limitations.

### How is it different from regular TCP?

1. **Connection**: TCP is connection-oriented, where a handshake is performed to establish a connection before data transmission. This ensures reliable and ordered delivery of data. On the other side my implementation is connectionless, meaning there is no establishment of a connection before data transmission. Each packet is sent independently, and there is no acknowledgment of receipt.

2. **Ordering of Data**: TCP ensures that data is delivered in the correct order. If packets arrive out of order, TCP reorders them before passing them to the application. While I assign sequence numbers to chunks, there is no mechanism to ensure that they are delivered to the receiver in the correct order.

3. **Error Correction**: TCP includes error detection and correction through features like checksums but my implementation fails to do any.

4. **Flow Control**: TCP uses a sliding window protocol to ensure that the sender does not overwhelm the receiver with data. My implementation does not include any flow control.


### Extending the implementation to handle Flow Control

#### What is Flow Control?
- Flow control is a crucial aspect of network communication that ensures data is transmitted at an appropriate rate between a sender and a receiver. It's necessary because the speed at which a sender can generate data may exceed the rate at which the receiver can process it. Without flow control, this imbalance can lead to problems, including data loss, inefficient network utilization, and potentially overwhelming the receiver.

#### How to implement Flow Control in my implementation ?

- One major change can be that, rather than sending the entire data from the beginning, the implementation can include a sliding window approach wherein I start sending chucnks only numbered 0 to N, where N is the initial window size. Whenever an acknowledgement is recieved, the window changes with highest ack recieved + K as the new starting point. Similarly the base value can also be changed. This will ensure that the sender does not overwhelm the receiver with data.

- Also maintain a window size, and send only the number of packets less than the window size to ensure the receiver is not overwhelmed.




