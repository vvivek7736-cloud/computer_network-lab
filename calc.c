
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

int main()
{
    int sockfd;
    double a,b;
    char op,res[100];

    struct sockaddr_in server;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    memset(&server,0,sizeof(server));

    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(sockfd,(struct sockaddr *)&server,sizeof(server));

    printf("Enter first number: ");
    scanf("%lf",&a);

    printf("Enter second number: ");
    scanf("%lf",&b);

    printf("Enter operator (+,-,*,/): ");
    scanf(" %c",&op);

    send(sockfd,&a,sizeof(a),0);
    send(sockfd,&b,sizeof(b),0);
    send(sockfd,&op,sizeof(op),0);

    recv(sockfd,res,sizeof(res),0);

    printf("Server response: %s\n",res);
    
    close(sockfd);

    return 0;
}
