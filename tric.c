#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>

int main()
{
    int sockfd ,n;
    int a[3][3];
    char buffer[100];
    struct sockaddr_in server;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(8080);
    connect(sockfd,(struct sockaddr*)&server,sizeof(server));
    printf("Connected\n");
    printf("1.Generate \n2.enter\n");
    printf("Enter matrix\n");
    int s;
    scanf("%d",&s);
    if(s==2){
        for(int i = 0;i<3;i++)
        {
            for(int j = 0;j<3;j++)
            {
             scanf("%d",&a[i][j]);
            }
        }
    }
    else if(s==1){
        printf("Generated matrix:\n");

        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                a[i][j]=rand()%10;
                printf("%d ",a[i][j]);
            }
            printf("\n");
        }
    }   
    send(sockfd,&a,sizeof(a),0);
    n=recv(sockfd,buffer,sizeof(buffer)-1,0);
    buffer[n]=0;
    printf("Server : %s",buffer);
    close(sockfd);
    return 0;
}