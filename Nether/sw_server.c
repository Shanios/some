#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<time.h>

#define PORT 8080

int main(){
  srand(time(NULL));
  int sock_fd,new_socket,n_frame,s_frame;
  struct sockaddr_in server_addr,client_addr;
  socklen_t addrlen = sizeof(client_addr);
  
  sock_fd = socket(AF_INET,SOCK_STREAM,0);
  
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  
  bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
  listen(sock_fd,5);
  new_socket = accept(sock_fd,(struct sockaddr*)&client_addr,&addrlen);
  
  printf("\nEnter the no of frames :- ");
  scanf("%d",&n_frame);
  printf("\nEnter the size of frame(bytes) :- ");
  scanf("%d",&s_frame);
  
  send(new_socket,&n_frame,sizeof(int),0);
  send(new_socket,&s_frame,sizeof(int),0);
  
  char data[s_frame];
  int f = 0,ack;
  
  while(f<n_frame){
    snprintf(data,s_frame,"Frame-%d",f);
    send(new_socket,data,s_frame,0);
    printf("\nsend :- %s ",data);
    recv(new_socket,&ack,sizeof(int),0);
    if(ack!= f+1){
      printf("\nResending the frame");
    }
    else{
      printf("\n recieved the ack-%d",ack);
      f++;
    }
    sleep(1);
  }
  ack = -1;
  send(new_socket,&ack,sizeof(int),0);
  close(new_socket);
  close(sock_fd);
  return 0;
}
