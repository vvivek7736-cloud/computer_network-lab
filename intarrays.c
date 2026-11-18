#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int s,c;
    int tar,z;
    s=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(8080);
    bind(s,(struct sockaddr*)&server,sizeof(server));
    listen(s,3);
    printf("Server watingg...\n");
    c=accept(s,(struct sockaddr*)&client,&len);
    printf("Client Connected!\n");
    int a[100];
    recv(c,&z,sizeof(z),MSG_WAITALL);
    recv(c,&tar,sizeof(tar),MSG_WAITALL);
    recv(c,a,z*sizeof(int),MSG_WAITALL);
    printf("size : %d\n",z);
    int max=0,min=9999;
    printf("Target value : %d\n",tar);
    printf("Array recvied : ");
    for(int i =0 ;i<z;i++){
        printf(" %d",a[i]);
    }
    int mean=0,pos=-1;
    for(int i =0;i<z;i++){
        if(max<a[i])max=a[i];
        if(min>a[i])min=a[i];
        mean+=a[i];
        if(a[i]==tar)pos=i;
    }
    mean=mean/z;
    send(c,&max,sizeof(max),0);
    send(c,&min,sizeof(min),0);
    send(c,&mean,sizeof(mean),0);
    send(c,&pos,sizeof(pos),0);
    printf("\nResult send to client!\n");
    close(c);
    close(s);
    return 0;
}