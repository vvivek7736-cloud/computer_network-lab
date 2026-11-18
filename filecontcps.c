
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int main()
{
    int server_fd, client_fd, n;
    char filename[100], buffer[1024], ch;
    struct sockaddr_in server, client;
    socklen_t len;
    FILE *fp;
    pid_t pid;

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Set server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;
    bind(server_fd, (struct sockaddr *)&server, sizeof(server));

    // Listen for clients
    listen(server_fd, 5);

    signal(SIGCHLD, SIG_IGN);

    printf("File server waiting...\n");
    while(1)
    {
        len = sizeof(client);

        // Accept client
        client_fd = accept(server_fd,
                     (struct sockaddr *)&client, &len);

        // Create child process
        pid = fork();

        if (pid == 0)
        {
            close(server_fd);
            int i = 0;
            while(i < 99 && recv(client_fd,&ch,1,0)>0 && ch!='\0')
            {
                filename[i++]=ch;
            }
            filename[i] = '\0';
            sprintf(buffer,"PID is %d\n",getpid());
            send(client_fd,buffer,strlen(buffer)+1,0);
            fp=fopen(filename,"r");
            if(fp==NULL){
                sprintf(buffer,"File not Found!!!!\n");
                send(client_fd,buffer,strlen(buffer)+1,0);
            }
            else{
                while((n=fread(buffer,1,1024,fp))>0){
                    send(client_fd,buffer,n,0);
                }
                fclose(fp);
            }
            close(client_fd);
            return 1;
        }
        else{
            close(client_fd);
        }
    }
    close(server_fd);
    return 0;
}