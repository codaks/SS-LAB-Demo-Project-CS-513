/*
ID : MT2023148
Name : Asmita Tripathi
Description : The client program running on port 8443
*/

#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include<stdlib.h>

char server_message[2000], client_message[2000];

void clearMemory(){
    memset(server_message,0,sizeof(server_message));
    memset(client_message,0, sizeof(client_message));
}

int main(void)
{
    int socket_fd, server_sock;
    struct sockaddr_in server_addr;
    int resp,val;

    clearMemory();
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_fd < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8440);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    server_sock = connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if( server_sock < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    
    
    // Receieving login menu
    if(recv(socket_fd, server_message, sizeof(server_message), 0) < 0){
        printf("Error while receiving login menu\n");
        return -1;
    }
    
    printf("%s",server_message);
    scanf("%s", client_message);
    if (send(socket_fd, client_message, strlen(client_message), 0) < 0){
        printf("Can't send clients choice\n");
        return -1;
    }

    clearMemory();
    //username
    recv(socket_fd, server_message, sizeof(server_message), 0);
    printf("%s",server_message);
    scanf("%s", client_message);
    send(socket_fd, client_message, strlen(client_message), 0);

    //password
    clearMemory();
    recv(socket_fd, server_message, sizeof(server_message), 0);
    printf("%s",server_message);
    scanf("%s", client_message);
    send(socket_fd, client_message, strlen(client_message), 0);

    //receiving role action
    do
    {
        clearMemory();
        if(recv(socket_fd, server_message, sizeof(server_message), 0) < 0){
            printf("Error while receiving profile menu\n");
            return -1;
        }
        
        printf("%s",server_message);
        scanf("%s", client_message);
        //sending role action
        if (send(socket_fd, client_message, strlen(client_message), 0) < 0){
            printf("Can't send profile action\n");
            return -1;
        }

        char *p = client_message;
        val = strtol(p, &p, 10); 
        switch(val)
        {
            case 1:
                    do 
                    {
                        clearMemory();
                    
                        recv(socket_fd, server_message, sizeof(server_message),0);
                        printf("%s",server_message);
                        scanf("%s", client_message);
                        send(socket_fd, client_message, sizeof(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            case 2 :
                     do 
                    {
                        clearMemory();
                    
                        recv(socket_fd, server_message, sizeof(server_message),0);
                        printf("%s",server_message);
                        scanf("%s", client_message);
                        send(socket_fd, client_message, sizeof(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            case 3:
                    do 
                    {
                       clearMemory();
                    
                        recv(socket_fd, server_message, sizeof(server_message),0);
                        printf("%s",server_message);
                        scanf("%s", client_message);
                        send(socket_fd, client_message, strlen(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            case 4:
                     do 
                    {
                       clearMemory();
                    
                        recv(socket_fd, server_message, sizeof(server_message),0);
                        printf("%s",server_message);
                        scanf("%s", client_message);
                        send(socket_fd, client_message, sizeof(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            case 5:
                    do 
                    {
                        clearMemory();
                    
                        int status = recv(socket_fd, server_message, sizeof(server_message),0);
                        while(status){
                            printf("%s\n",server_message);
                            clearMemory();
                            status = recv(socket_fd, server_message, sizeof(server_message),0);
                        }
                        
                        scanf("%s", client_message);
                        printf("Client sent : %s", client_message);
                        send(socket_fd, client_message, strlen(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            case 6:
                    do 
                    {
                        clearMemory();
                    
                        recv(socket_fd, server_message, sizeof(server_message),0);
                        printf("%s",server_message);
                        scanf("%s", client_message);
                        send(socket_fd, client_message, strlen(client_message), 0);
                    }while(strcmp(client_message,"9")!=0);
                    break;
            
        }
    }while(val<7);
    close(socket_fd);
    return 0;
}