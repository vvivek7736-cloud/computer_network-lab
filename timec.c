#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sockfd,n;
    char buffer[100]="time";
    struct sockaddr_in server;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&server,sizeof(server));
    n = recvfrom(sockfd,buffer,sizeof(buffer)-1,0,NULL,NULL);
    buffer[n]='\0';
    printf("Sevr time is : %s",buffer);
    close(sockfd);
    return 0;
}