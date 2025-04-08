#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define LOSS_PROB 30  // percentage

int main() {
    srand(time(NULL));
    int sock_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_len = sizeof(cli_addr);
    char buffer[BUFFER_SIZE];
    int expected = 0;

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while (1) {
        int n = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli_addr, &addr_len);
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        int frame_num;
        sscanf(buffer, "Frame-%d", &frame_num);

        if (frame_num == expected) {
            if (rand() % 100 < LOSS_PROB) {
                printf("Simulated loss for Frame-%d\n", frame_num);
                continue; // drop packet
            }
            printf("Accepted: Frame-%d\n", frame_num);
            expected++;
        } else {
            printf("Out of order or duplicate frame. Expected: %d\n", expected);
        }

        sprintf(buffer, "ACK-%d", expected);
        sendto(sock_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&cli_addr, addr_len);
        printf("Sent: %s\n\n", buffer);
    }

    close(sock_fd);
    return 0;
}

