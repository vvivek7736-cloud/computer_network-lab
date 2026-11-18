
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

int main()
{
    int s,n;
    char buf[1024];
    struct sockaddr_in server;

    s=socket(AF_INET,SOCK_STREAM,0);

    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(s,(struct sockaddr*)&server,sizeof(server))<0){
        perror("connect");
        close(s);
        return 1;
    }

    if(fork()==0){
        while(1){
            n=recv(s,buf,1023,0);

            if(n<=0)
                break;

            buf[n]='\0';
            printf("%s",buf);
        }
    }
    else{
        while(1){
            printf("Enter : ");

            if(fgets(buf,1024,stdin)==NULL)
                break;

            buf[strcspn(buf,"\n")]=0;
            send(s,buf,strlen(buf)+1,0);
        }

        shutdown(s,SHUT_WR);
        wait(NULL);
    }

    close(s);
    return 0;
}
