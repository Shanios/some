#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int main(){
  int sockfd;
  struct sockaddr_in server_addr,client_addr;
  socklen_t addrlen = sizeof(client_addr);
  char buffer[BUFFER_SIZE];
  
  if( ( sockfd = socket(AF_INET,SOCK_DGRAM,0) )<0 ){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  
  if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  
  printf("server is listening to port %d \n",PORT);
  while(1){
    int n = recvfrom(sockfd,buffer,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,&addrlen);
    if(n<0){
      perror("Recieve Failed");
      continue;
    }
    buffer[n] = '\0';
    printf("recieved from client : %s\n",buffer);
    const char* response = "Message recieved";
    sendto(sockfd,response,strlen(response),0,(struct sockaddr*)&client_addr,addrlen);
  }
  close(sockfd);
  return 0;
}
