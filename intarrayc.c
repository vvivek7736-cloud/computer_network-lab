#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>
int main()
{
    int s,c,n;
    s=socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
    server.sin_port=htons(8080);
    connect(s,(struct sockaddr*)&server,sizeof(server));
    printf("Connected!!\n");
    printf("Enter the size of array : ");
    int size=0;
    scanf("%d",&size);
    int a[size];
    printf("Enter %d integers : \n",size);
    for(int i =0 ;i<size;i++){
        scanf("%d",&a[i]);
    }
    printf("Array : ");
    for(int i =0 ;i<size;i++){
        printf(" %d",a[i]);
    }
    printf("\nEnter target value : ");
    int tar;
    scanf("%d",&tar);
    send(s,&size,sizeof(size),0);
    send(s,&tar,sizeof(tar),0);
    send(s,a,size*sizeof(int),0);
    printf("Send to server ...\n");
    int mean,max,min,pos;
    recv(s,&max,sizeof(max),MSG_WAITALL);
    recv(s,&min,sizeof(min),MSG_WAITALL);
    recv(s,&mean,sizeof(mean),MSG_WAITALL);
    recv(s,&pos,sizeof(pos),MSG_WAITALL);
    printf("RESULT\n");
    printf("Max : %d\n",max);
    printf("Min : %d\n",min);
    printf("MEan : %d\n",mean);
    printf("Pos : %d\n",pos);
    close(s);
    return 0;
}