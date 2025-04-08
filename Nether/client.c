#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define LOSS_P 20  // Packet loss probability
#define TIMEOUT 5   // Timeout in seconds
#define SERVER_IP "127.0.0.1"

int main() {
    srand(time(NULL));
    int sock_fd, n_frame, s_frame, ack;
    struct sockaddr_in sock_addr;

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(PORT);
    sock_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("Connection failed");
        close(sock_fd);
        exit(1);
    }

    // Receive number of frames and frame size from the server
    recv(sock_fd, &n_frame, sizeof(int), 0);
    recv(sock_fd, &s_frame, sizeof(int), 0);
    printf("\nSending %d frames of size %d each.\n", n_frame, s_frame);

    char data[s_frame];
    int f = 0;

    // Send frames and wait for acknowledgments
    while (f < n_frame) {
        // Prepare the frame (data to be sent)
        snprintf(data, s_frame, "Frame-%d", f);
        send(sock_fd, data, s_frame, 0);
        printf("\nSent: %s\n", data);

        // Wait for acknowledgment (ACK) from server
        int received_ack = 0;
        time_t start_time = time(NULL);
        while (time(NULL) - start_time < TIMEOUT) {
            if (recv(sock_fd, &ack, sizeof(int), 0) > 0) {
                if (ack == f + 1) {
                    printf("Received ACK-%d for frame %d\n", ack, f);
                    f++;
                    received_ack = 1;
                    break;
                }
            }
        }

        if (!received_ack) {
            printf("ACK-%d not received for frame %d. Resending...\n", f + 1, f);
        }

        sleep(1); // Simulate processing delay
    }

    // Close connection
    close(sock_fd);
    return 0;
}

