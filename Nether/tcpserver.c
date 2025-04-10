#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>


#define PORT 8081
#define BUFFER_SIZE 1024

int main(){
  int server_fd,new_socket,valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[BUFFER_SIZE] = {0};
  char *hello = "Message recieved";
  printf("Server is listening\n");
  
  //socket creation
  if( (server_fd = socket(AF_INET,SOCK_STREAM,0)   ) == 0 ){
      perror("Socket Creation Failed\n");
      exit(EXIT_FAILURE);
    }
  else
    printf("Socket created succesfully\n");
  
  //data initialization for bind
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  
  if( bind( server_fd, (struct sockaddr*)&address, sizeof(address)) < 0 ){
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }
  else
    printf("Binding completed\n");
 
  if(listen (server_fd,3) < 0){
    perror("Listen failed");
    exit(EXIT_FAILURE);
  }
  else
    printf("Listening to socket %d \n",PORT);
  
  if( (new_socket = accept(server_fd,(struct sockaddr *)&address,(socklen_t*)&addrlen)) < 0 ){
    perror("accept_failed");
    exit(EXIT_FAILURE);
  }
  else
    printf("accept succesfull \n");
  
  valread = read (new_socket,buffer,BUFFER_SIZE);
  printf("Message recieved is \n %s \n" ,buffer);
  
  send(new_socket,hello,strlen(hello),0);
  printf("Response sent to client \n");
  
  close(new_socket);
  return 0;
}

