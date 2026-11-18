
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{
    int s,c,cl[10]={0},ind[10]={0};
    int count=0,i,j,max,n;
    int opt=1;
    char buffer[1024],msg[1200];
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);

    s=socket(AF_INET,SOCK_STREAM,0);

    setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(8080);

    if(bind(s,(struct sockaddr *)&server,sizeof(server))<0){
        perror("bind");
        close(s);
        return 1;
    }

    listen(s,5);
    printf("Server is listening...\n");

    while(1){
        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(s,&fd);
        max=s;

        for(i=0;i<10;i++){
            if(cl[i]>0){
                FD_SET(cl[i],&fd);
                if(cl[i]>max)
                    max=cl[i];
            }
        }

        select(max+1,&fd,NULL,NULL,NULL);

        if(FD_ISSET(s,&fd)){
            len=sizeof(client);
            c=accept(s,(struct sockaddr*)&client,&len);

            for(i=0;i<10;i++){
                if(cl[i]==0)
                    break;
            }

            if(i==10){
                close(c);
                continue;
            }

            cl[i]=c;
            ind[i]=++count;

            sprintf(msg,"You are client %d\n",count);
            printf("%d connected\n",count);
            send(c,msg,strlen(msg)+1,0);
        }

        for(i=0;i<10;i++){
            if(cl[i]>0 && FD_ISSET(cl[i],&fd)){
                n=recv(cl[i],buffer,1023,0);

                if(n<=0){
                    sprintf(msg,"Client %d disconnected\n",ind[i]);
                    printf("%s",msg);

                    close(cl[i]);
                    cl[i]=0;
                    ind[i]=0;
                }
                else{
                    buffer[n]='\0';
                    sprintf(msg,"Client %d : %s\n",ind[i],buffer);
                    printf("%s",msg);
                }

                for(j=0;j<10;j++){
                    if(cl[j]>0 && i!=j){
                        send(cl[j],msg,strlen(msg)+1,0);
                    }
                }
            }
        }
    }

    close(s);
    return 0;
}
