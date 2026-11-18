
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main()
{
    int sockfd,newfd;
    double a,b,result;
    char op,res[100];
    int opt=1;

    struct sockaddr_in server;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    memset(&server,0,sizeof(server));

    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(8080);

    bind(sockfd,(struct sockaddr *)&server,sizeof(server));

    listen(sockfd,5);

    printf("Server listening...\n");

    newfd=accept(sockfd,NULL,NULL);

    printf("Client connected\n");

    recv(newfd,&a,sizeof(a),MSG_WAITALL);
    recv(newfd,&b,sizeof(b),MSG_WAITALL);
    recv(newfd,&op,sizeof(op),MSG_WAITALL);

    if(op=='+')
    {
        result=a+b;
        sprintf(res,"Result = %.2lf",result);
    }
    else if(op=='-')
    {
        result=a-b;
        sprintf(res,"Result = %.2lf",result);
    }
    else if(op=='*')
    {
        result=a*b;
        sprintf(res,"Result = %.2lf",result);
    }
    else if(op=='/')
    {
        if(b==0)
            strcpy(res,"Division by zero not allowed");
        else
        {
            result=a/b;
            sprintf(res,"Result = %.2lf",result);
        }
    }
    else
        strcpy(res,"Invalid operator");

    printf("%s\n",res);

    send(newfd,res,strlen(res)+1,0);

    close(newfd);
    close(sockfd);

    return 0;
}
