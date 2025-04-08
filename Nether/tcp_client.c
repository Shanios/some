#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>

#define PORT 8081
#define BUFFER_SIZE 1024

void create_socket();

int main(){
  int sock_fd = 0,valread;
  struct sockaddr_in address;
  char message[100];
  char buffer[BUFFER_SIZE] = {0};
  
  if( (sock_fd = socket(AF_INET,SOCK_STREAM,0) ) == 0 ){
      perror("Socket Creation Failed");
      exit(EXIT_FAILURE);
    }
  else
    printf("Socket created succesfully");

  
  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  
  if (inet_pton(AF_INET,"127.0.0.1",&address.sin_addr) <= 0 ){
    perror("invalid address");
    exit(EXIT_FAILURE);
  }
  
  if ( connect( sock_fd , (struct sockaddr *)& address , sizeof (address)) < 0 ){
    perror("connetion failed");
    exit(EXIT_FAILURE);
  }
  
  printf("Enter the message to sent :\n");
  scanf("%s",message);
  send(sock_fd,message,strlen(message),0);
  printf("Message sent to server");
  valread = read(sock_fd,buffer,BUFFER_SIZE);
  printf("recieved response \n %s \n",buffer);
  close(sock_fd);
  return 0;
}


