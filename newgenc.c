#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sockfd,n;
    char buffer[100],res[1000];
    char *word;
    struct sockaddr_in server;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        perror("Socket");
        return 1;
    }
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    printf("Enter the sentence : ");
    if(fgets(buffer,sizeof(buffer),stdin)==NULL){
        perror("Input error");
        close(sockfd);
        return 1;
    }
    buffer[strcspn(buffer,"\n")]='\0';
    if(sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&server,sizeof(server))<0){
        perror("send");
        close(sockfd);
        return 1;
    }
    n= recvfrom(sockfd,res,sizeof(res)-1,0,NULL,NULL);
    if(n<0){
        perror("recv");
        close(sockfd);
        return 1;
    }
    res[n]='\0';
    printf("Translated : %s\n",res);
    
    close(sockfd);
    return 0;
}