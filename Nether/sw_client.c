#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 8080
#define LOSS_P 20
#define SERVER_IP "127.0.0.1"
int main(){
  srand(time(NULL));
  int sock_fd,n_frame,s_frame,ack;
  struct sockaddr_in sock_addr;
  
  sock_fd = socket(AF_INET,SOCK_STREAM,0);
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_port = htons(PORT);
  sock_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  connect(sock_fd,(struct sockaddr*)&sock_addr,sizeof(sock_addr));
  recv(sock_fd,&n_frame,sizeof(int),0);
  recv(sock_fd,&s_frame,sizeof(int),0);
  printf("\n recieving %d frames of size %d each",n_frame,s_frame);
  char data[s_frame];
  int i=0 ;
  while(i<n_frame){
    recv(sock_fd,data,s_frame,0);
    if(strcmp(data,"-1") == 0)
      break;
    if (rand()%100 < LOSS_P){
      printf("\nframe %d not sending ACK",i);
    }
    else{
      ack= i+1;
      printf("\nFrame %d recieved",i);
      send(sock_fd,&ack,sizeof(int),0);
      printf(" sent ACK-%d",ack);
    }
    sleep(1);
  }
  close(sock_fd);
  return 0;
}
