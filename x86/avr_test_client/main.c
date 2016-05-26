#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "192.168.1.103"
#define BUFLEN 64  //Max length of buffer
#define PORT 8888   //The port on which to send data

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    int clientSocket, portNum, nBytes;
      char buffer[1024];
      struct sockaddr_in serverAddr;
      socklen_t addr_size;

      /*Create UDP socket*/
      clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

      /*Configure settings in address struct*/
      serverAddr.sin_family = AF_INET;
      serverAddr.sin_port = htons(8888);
      serverAddr.sin_addr.s_addr = inet_addr("192.168.1.103");
      memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

      /*Initialize size variable to be used later on*/
      addr_size = sizeof serverAddr;

      while(1){
        printf("Type a sentence to send to server:\n");
        fgets(buffer,64,stdin);
        printf("You typed: %s",buffer);

        nBytes = 64;//strlen(buffer) + 1;

        /*Send message to server*/
        sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);

        /*Receive message from server*/
                    nBytes = recvfrom(clientSocket,buffer,64,0,NULL, NULL);

        printf("Received from server: %s\n",buffer);

      }
    return 0;
}
