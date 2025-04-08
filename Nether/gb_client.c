#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/time.h>
#include<fcntl.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
#define FRAME_COUNT 10
#define WINDOW_SIZE 4
#define TIMEOUT_SEC 2

int base = 0, next_seq = 0;
int sock_fd;
struct sockaddr_in serv_addr;
socklen_t addr_len = sizeof(serv_addr);
char buffer[BUFFER_SIZE];

void send_frame(int seq) {
    sprintf(buffer, "Frame-%d", seq);
    sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, addr_len);
    printf("Sent: %s\n", buffer);
}

int main() {
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    fcntl(sock_fd, F_SETFL, O_NONBLOCK); // Non-blocking recv

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    int ack;
    time_t timer_start = time(NULL);

    while (base < FRAME_COUNT) {
        while (next_seq < base + WINDOW_SIZE && next_seq < FRAME_COUNT) {
            send_frame(next_seq);
            next_seq++;
        }

        int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &addr_len);
        if (n > 0) {
            buffer[n] = '\0';
            sscanf(buffer, "ACK-%d", &ack);
            printf("Received: %s\n", buffer);
            if (ack >= base + 1) {
                base = ack;
                timer_start = time(NULL); // Reset timer
            }
        }

        // Timeout: retransmit all unacknowledged frames
        if (difftime(time(NULL), timer_start) > TIMEOUT_SEC) {
            printf("Timeout! Resending frames from %d to %d\n", base, next_seq - 1);
            for (int i = base; i < next_seq; i++) {
                send_frame(i);
            }
            timer_start = time(NULL);
        }

        usleep(500000); // Small delay
    }

    close(sock_fd);
    return 0;
}

