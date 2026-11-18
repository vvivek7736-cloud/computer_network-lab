#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sockfd,newfd;
    int opt=1;
    char buffer[100];
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket failed");
        return 1;
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(8080);
    if(bind(sockfd,(struct sockaddr *)&server,sizeof(server))<0){
        perror("bind");
        close(sockfd);
        return 1;
    }
    if(listen(sockfd,5)<0){
        perror("listen");
        close(sockfd);
        return 1;
    }
    printf("Server is listening....\n");
    newfd=accept(sockfd,(struct sockaddr*)&client,&len);
    if(newfd<0){
        perror("accpet");
        close(sockfd);
        return 1;
    }
    printf("Client Connected\n");
    while(1){
        memset(buffer,0,sizeof(buffer));
        int n = recv(newfd,buffer,sizeof(buffer)-1,0);
        if(n<=0){
            break;
        }
        buffer[n]='\0';
        printf("Client : %s",buffer);
        if(strncmp(buffer,"bye",3)==0)
            break;
        printf("Server : ");
        if(fgets(buffer,sizeof(buffer),stdin)==NULL)
            break;
        if(send(newfd,buffer,strlen(buffer)+1,0)<=0)
            break;
        if(strncmp(buffer,"bye",3)==0)
            break;
    }
    close(sockfd);
    close(newfd);
    printf("Chat ended!!!\n");
    return 0;
}