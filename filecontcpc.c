#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main()
{
    int sockfd, n, i = 0;
    char filename[100], buffer[1024], ch;
    struct sockaddr_in server;

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    printf("Enter filename: ");
    scanf("%s", filename);
    send(sockfd, filename, strlen(filename) + 1, 0);
    while(i<99 && recv(sockfd,&ch,1,0)>0 && ch!='\0'){
        buffer[i++]=ch;
    }
    buffer[i]='\0';
    printf("\n%s\n",buffer);
    int found;
    while((n=recv(sockfd,buffer,sizeof(buffer),0))>0){
        fwrite(buffer,1,n,stdout);
    }
    recv(sockfd,found,sizeof(found),0);
    if(found==1){
        printf("Found!!!");

    }
    else{
        printf("not found!!!");
    }
    close(sockfd);
    return 0;
}