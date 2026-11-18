
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
    struct sockaddr_in server,client;
    socklen_t len;
    FILE *fp;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=INADDR_ANY;

    bind(sockfd,(struct sockaddr*)&server,sizeof(server));

    printf("Server waiting...\n");
    pid_t pid=fork();
    while(1)
    {
        len=sizeof(client);

        int n=recvfrom(sockfd,filename,sizeof(filename)-1,0,
                 (struct sockaddr*)&client,&len);

        filename[n]='\0';

        sprintf(buffer,"PID: %d",getpid());
        sendto(sockfd,buffer,strlen(buffer)+1,0,
               (struct sockaddr*)&client,len);

        fp=fopen(filename,"rb");

        if(fp==NULL)
        {
            strcpy(buffer,"NOTFOUND");
            sendto(sockfd,buffer,strlen(buffer)+1,0,
                   (struct sockaddr*)&client,len);
        }
        else
        {
            strcpy(buffer,"FOUND");
            sendto(sockfd,buffer,strlen(buffer)+1,0,
                   (struct sockaddr*)&client,len);

            while((n=fread(buffer,1,1024,fp))>0)
            {
                sendto(sockfd,buffer,n,0,
                       (struct sockaddr*)&client,len);
            }

            fclose(fp);

            strcpy(buffer,"END");
            sendto(sockfd,buffer,strlen(buffer)+1,0,
                   (struct sockaddr*)&client,len);
        }
    }

    close(sockfd);
    return 0;
}
