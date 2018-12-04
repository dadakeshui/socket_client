#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include "wrap.h"
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#define MAXLINE 80
#define SERVER_PORT 8000
#define SERVER_IP "192.168.1.149"

int main(int argc, const char* argv[])
{
    struct sockaddr_in server_add;
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0); 
    char buf[100] = {0}; 
    //服务器端口号绑定
    
    bzero(&server_add, sizeof(server_add));
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(SERVER_PORT);
//    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_pton(AF_INET, SERVER_IP, &server_add.sin_addr.s_addr);

    printf("server_ip: %s \n", inet_ntop(AF_INET, &server_add.sin_addr, buf, sizeof(buf)));
    
    Connect(sockfd, (struct sockaddr*)&server_add, sizeof(server_add));
    int n;
    while(fgets(buf, MAXLINE, stdin) != NULL)
    {
        Write(sockfd, buf, strlen(buf));
        n = Read(sockfd, buf, MAXLINE);
        if(n == 0)
        {
            printf("the other side has been closed\n");
            break;
        }
        Write(STDOUT_FILENO, buf, n);
    }

    return 0;

}
