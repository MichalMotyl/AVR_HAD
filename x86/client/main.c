/*receiverprog.c - a server, datagram sockets*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* the port users will be connecting to */
#define MYPORT 8888
#define MAXBUFLEN 500
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    int read_cnt = 0;
    struct sockaddr_in server , client;
    char client_message[2000];
     char sendBuff[1025];
    time_t ticks; 
    int connected = 0;
    
    int temp1 =1;
    int temp2 =2;
    int temp3 =3;
    int temp4 =4;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 5000 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    
    while(1)
    {
        usleep(100000);
        if(connected==0)
        {
            //accept connection from an incoming client
            client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
            
            if (client_sock < 0)
            {
                perror("accept failed");
                return 1;
            }
            else
            {
                printf("Connection accepted %d\n",read_cnt);
                connected = 1;
            }
        }
        else
        {
            

            //ticks = time(NULL);
            //snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
            //write(client_sock, sendBuff, strlen(sendBuff)); 
            if(read(client_sock, sendBuff, 10)>0)
            {
                if(sendBuff[0] = 0x5a)
                {
                    printf("wyslij...\n");
                    sendBuff[0] = temp1 & 0x00ff;
                    sendBuff[1] = (temp1 & 0xff00)>>8;

                    sendBuff[2] = temp2 & 0x00ff;
                    sendBuff[3] = (temp2 & 0xff00)>>8;

                    sendBuff[4] = temp3 & 0x00ff;
                    sendBuff[5] = (temp3 & 0xff00)>>8;

                    sendBuff[6] = temp4 & 0x00ff;
                    sendBuff[7] = (temp4 & 0xff00)>>8;

                    //write(client_sock, sendBuff,10); 
                    int stat = send(client_sock,sendBuff, 10, MSG_NOSIGNAL);

                    if (stat == -1)
                    {
                        close(client_sock);
                        connected = 0;
                        printf("zamknij soket!!!!\n");
                    }
                    else
                    {
                        temp1++;
                        temp2++;
                        temp3++;
                        temp4++;
                        printf("%d %d %d %d\n",temp1,temp2,temp3,temp4);
                    }
                }
                
            }
            else
                {   close(client_sock);
                        connected = 0;
                        printf("zamknij soket!!!!\n");
                    
                }
        }
    }
/*
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        write(client_sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
*/
     
    return 0;
}