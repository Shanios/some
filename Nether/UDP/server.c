#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main(){
  int server_fd,valread;
  struct sockaddr_in server_addr,client_addr;
  socklen_t addrlen = sizeof(client_addr);
  char message[BUFFER_SIZE] = {0};
  char response[BUFFER_SIZE] = {0};
  
  server_fd = socket(AF_INET,SOCK_DGRAM,0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
  
  while (1){
    valread = recvfrom(server_fd,message,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,&addrlen);
    message[valread] = '\0';
    printf("recieved :- %s\n",message);
    strcpy(response,"Your message has been recieved\n");
    if(strcmp(message,"quit") == 0){
      strcpy(response,"ACK");
      sendto(server_fd,response,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,addrlen);
      break;
    }
    sendto(server_fd,response,BUFFER_SIZE,0,(struct sockaddr*)&client_addr,addrlen);
    }
    close(server_fd);
  return 0;
}
