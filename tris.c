#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sockfd,newfd,i,j;
    int opt=1;
    int a[3][3];
    char res[100];
    int upper=1,lower=1;
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("socket failed");
        return 1;
    }
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(8080);
    if(bind(sockfd,(struct sockaddr *)&server,sizeof(server))<0){
        perror("bind");
        close(sockfd);
        return 1;
    }
    if(listen(sockfd,5)<0){
        perror("listen");
        close(sockfd);
        return 1;
    }
    printf("Server is listening....\n");
    newfd=accept(sockfd,(struct sockaddr*)&client,&len);
    if(newfd<0){
        perror("accpet");
        close(sockfd);
        return 1;
    }
    printf("Client Connected\n");
    recv(newfd,a,sizeof(a),MSG_WAITALL);
    printf("Matrix receieved  : \n");
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            printf("%d",a[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(i>j && a[i][j]!=0)
                upper=0;
            if(i<j && a[i][j]!=0)
                lower=0;
        }
    }
    if(lower==1 && upper==1)
        strcpy(res,"Diagonal");
    else if(upper==1)
        strcpy(res,"Upper");
    else if(lower==1)
        strcpy(res,"Lower");
    else
        strcpy(res,"Neither");
    printf("Result sent : %s\n",res);
    if(send(newfd,res,strlen(res)+1,0)<=0){
        perror("Send");
    }
    close(sockfd);
    close(newfd);
    printf("Chat ended!!!\n");
    return 0;
}