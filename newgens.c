#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main()
{
    int sockfd,n,i,l;
    char buffer[100],out[1000],punct[2];
    char *word;
    char *abbr[12] = {
        "tbh", "ig", "tbf", "atm", "irl", "lol",
        "asap", "omg", "ttyl", "idk", "nvm", "idc"
    };

    char *full[12] = {
        "to be honest", "I guess", "to be fair",
        "at the moment", "in real life",
        "laughing out loud", "as soon as possible",
        "oh my god", "talk to you later",
        "I don't know", "never mind", "I don't care"
    };
    struct sockaddr_in server,client;
    socklen_t len=sizeof(client);
    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0){
        perror("Socket");
        return 1;
    }
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    if(bind(sockfd,(struct sockaddr*)&server,sizeof(server))<0){
        perror("Bind");
        close(sockfd);
        return 1;
    }
    printf("Sever is waiting....\n");
    n = recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&client,&len);
    if(n<0){
        perror("REcv");
        close(sockfd);
        return 1;
    }
    buffer[n]=0;
    out[0]='\0';
    word=strtok(buffer," ");
    int found;
    while(word!=NULL){
        punct[0]=0;
        punct[1]=0;
        l = strlen(word);
        if(l>0 && (word[l-1]=='.' || word[l-1]==','))
        {
            punct[0]=word[l-1];
            word[l-1]=0;
        }
        found=0;
        for(i=0;i<12;i++){
            if(strcmp(abbr[i],word)==0)
            {
                strcat(out,full[i]);
                found=1;
                break;
            }
        }
        if(found==0){
            strcat(out,word);
        }
        strcat(out,punct);
        strcat(out," ");
        word=strtok(NULL," ");
    }
    printf("Translated : %s\n",out);
    if(sendto(sockfd,out,strlen(out)+1,0,(struct sockaddr*)&client,len)<0){
        perror("send");
        close(sockfd);
        return 1;
    }
    close(sockfd);
    return 0;
}