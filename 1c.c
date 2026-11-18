#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main()
{
    int sockfd;
    struct sockaddr_in server;
    char buffer[100];
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(sockfd,(struct sockaddr*)&server,sizeof(server))<0){
        perror("connect");
        close(sockfd);
        return 1;
    }
    printf("Client Connected to Server \n");
    while(1){
        printf("You : ");
        if(fgets(buffer,sizeof(buffer),stdin)==NULL)
            break;
        if(send(sockfd,buffer,strlen(buffer)+1,0)<=0)
            break;
        if(strncmp(buffer,"bye",3)==0)
            break;
        memset(buffer,0,sizeof(buffer));
        int n = recv(sockfd,buffer,sizeof(buffer)-1,0);
        if(n<0)break;
        buffer[n]='\0';
        printf("Sever : %s",buffer);
        if(strncmp(buffer,"bye",3)==0)
            break;
    }
    close(sockfd);
    printf("Chat Ended !!!");
    return 0;
}