#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 8081
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void wait_for_ack(int sock_fd){
  usleep(100000);
}

int simulate_loss(){
  return rand()%2; 
}
int main(){
  int sock_fd,retry_count = 0,valread = 0;
  srand(time(NULL));
  struct sockaddr_in client_addr;
  char message[BUFFER_SIZE] = {0};
  char response[BUFFER_SIZE] = {0};
  
  sock_fd = socket(AF_INET,SOCK_DGRAM,0);
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(PORT);
  
  inet_pton(AF_INET,SERVER_IP,&client_addr.sin_addr);
  client_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  
  connect(sock_fd,(struct sockaddr*)&client_addr,sizeof(client_addr));
  
  while(1){
    printf("\nsent :- ");
    fgets(message,BUFFER_SIZE,stdin);
    message[strcspn(message,"\n")]=0;
  
    send(sock_fd,message,BUFFER_SIZE,0);
  
    valread = recv(sock_fd,response,BUFFER_SIZE,0);
  
    if(valread>0){
      if(simulate_loss()){
        printf("simulated loss");
        retry_count++;
        printf("retry %d",retry_count);
        continue;
      }
      response[valread] = '\0';
      printf("response :- %s\n",response);
      if(strcmp(response,"ACK") == 0){
        break;
      }
    }
    else{
      retry_count++;
      printf("Packet loss\nretry :- %d\n",retry_count);
    }
    if(retry_count>=3){
      printf("Maximum retry count reached\n");
      break;
    }
  }
  close(sock_fd);
  return 0;
}

