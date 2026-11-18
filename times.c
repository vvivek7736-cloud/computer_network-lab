
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main()
{
    int sockfd;
    char buffer[100];
    time_t t;
    struct sockaddr_in server,client;
    socklen_t len;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0)
    {
        perror("socket ");
        return 1;
    }
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;
    if(bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0)
    {
        perror("Bind ");
        close(sockfd);
        return 1;
    }
    printf("Server is Waiting..\n");
    signal(SIGCHLD,SIG_IGN);
    while(1)
    {
        len=sizeof(client);
        recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&client,&len);
        if(fork()==0){
            t=time(NULL);
            strcpy(buffer,ctime(&t));
            sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&client,len);
            close(sockfd);
            return 0;
        }
    }
    close(sockfd);
    return 0;
}