
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>

int main()
{
    int sockfd,n;
    char filename[100],buffer[1024];
    struct sockaddr_in server;
    socklen_t len=sizeof(server);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    printf("Enter filename: ");
    scanf("%99s",filename);

    sendto(sockfd,filename,strlen(filename)+1,0,
           (struct sockaddr*)&server,sizeof(server));

    n=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,
               (struct sockaddr*)&server,&len);

    if(n<=0)
        return 1;

    buffer[n]='\0';
    printf("\n%s\n",buffer);

    n=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,
               (struct sockaddr*)&server,&len);

    if(n<=0)
        return 1;

    buffer[n]='\0';

    if(strcmp(buffer,"NOTFOUND")==0)
    {
        printf("File Not Found!!!\n");
    }
    else
    {
        printf("File Contents:\n");

        while(1)
        {
            n=recvfrom(sockfd,buffer,sizeof(buffer),0,
                       (struct sockaddr*)&server,&len);

            if(n<=0)
                break;

            if(n==4 && memcmp(buffer,"END",3)==0 && buffer[3]=='\0')
                break;

            fwrite(buffer,1,n,stdout);
        }
    }

    close(sockfd);
    return 0;
}
