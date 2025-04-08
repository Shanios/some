#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void wait_for_ack(int sockfd){
  usleep(100000);
}

int simulate_ack_loss(){
  return (rand()%2);
}

int main(){
  srand(time(NULL));
  int sockfd,retry_count;
  struct sockaddr_in server_addr;
  char message[BUFFER_SIZE];
  char buffer[BUFFER_SIZE];
  char ack[BUFFER_SIZE];
  if( (sockfd = socket(AF_INET,SOCK_DGRAM,0))<0){
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  
  if(inet_pton(AF_INET,SERVER_IP,&server_addr.sin_addr)<=0){
    perror("Invalid address");
    exit(EXIT_FAILURE);
  }
  
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  
  if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) == -1){
    perror("connection failure");
    exit(EXIT_FAILURE);
  }
  
  while(1){
    printf("client");
    fgets(message,sizeof(message),stdin);
    message[strcspn(message,"\n")] = 0;
    send(sockfd,message,strlen(message),0);
    printf("client send :- %s\n",message);
    wait_for_ack(sockfd);
    int n = recv(sockfd,ack,sizeof(ack),0);
    if(n>0){
      ack[n] = '\0';
      printf("client recieved ack :- %s\n",ack);
      
      if(simulate_ack_loss()){
        printf("client simulate ack loss\n");
        retry_count ++;
        printf("Retry %d\n",retry_count);
        continue;
      }
      if( strcmp(ack,"ACK") == 0)
        break;
    }
    else{
      retry_count++;
      printf("no ack recieved retrying %d \n",retry_count);
    }
    if(retry_count >= 3){
      printf("Maximum retry limit reached , Exiting\n");
      break;
    }
  }
  close(sockfd);
  return 0;
}
