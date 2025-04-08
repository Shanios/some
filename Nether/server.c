#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define LOSS_P 20  // Packet loss probability
#define TIMEOUT 5   // Timeout in seconds

#define CLIENT_IP "127.0.0.1"

int main() {
    srand(time(NULL));
    int sock_fd, n_frame, s_frame, ack;
    struct sockaddr_in sock_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(PORT);
    sock_addr.sin_addr.s_addr = inet_addr(CLIENT_IP);

    // Bind the server socket
    if (bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("Bind failed");
        close(sock_fd);
        exit(1);
    }

    // Listen for connections
    listen(sock_fd, 5);
    printf("Server waiting for connection...\n");

    // Accept connection from client
    int new_socket = accept(sock_fd, (struct sockaddr*)&client_addr, &addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        close(sock_fd);
        exit(1);
    }

    // Receive number of frames and frame size from client
    recv(new_socket, &n_frame, sizeof(int), 0);
    recv(new_socket, &s_frame, sizeof(int), 0);
    printf("\nReceiving %d frames of size %d each.\n", n_frame, s_frame);

    char data[s_frame];
    int f = 0;

    // Receive frames and send acknowledgments
    while (f < n_frame) {
        // Receive data (frame) from client
        recv(new_socket, data, s_frame, 0);
        printf("\nReceived: %s\n", data);

        // Simulate packet loss based on LOSS_P
        if (rand() % 100 < LOSS_P) {
            printf("Simulating packet loss. No ACK sent for frame %d.\n", f);
        } else {
            // Send acknowledgment to client
            ack = f + 1; // Acknowledge the received frame
            send(new_socket, &ack, sizeof(int), 0);
            printf("Sent ACK-%d for frame %d\n", ack, f);
            f++;
        }

        sleep(1); // Simulate processing delay
    }

    // Send -1 to signal end of transmission
    ack = -1;
    send(new_socket, &ack, sizeof(int), 0);

    // Close connection
    close(new_socket);
    close(sock_fd);
    return 0;
}

