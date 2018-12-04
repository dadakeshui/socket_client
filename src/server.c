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
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define IP_SER  "192.168.1.149"
#define SERVER_PORT 8000
#define MAXLINE 80

void do_child(int a)
{
    while(waitpid(0, NULL, WNOHANG) > 0); //只回收同一进程组的子进程
}


int main(int argc, const char* argv[])
{
    struct sockaddr_in server_add, client_add;

    //创建套接字
    int sockfd = Socket(AF_INET, SOCK_STREAM, 0);
     
    //设置服务器IP和端口
    bzero(&server_add, sizeof(server_add));

    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(SERVER_PORT);
//    server_add.sin_addr.s_addr = htonl(INADDR_ANY);

    inet_pton(AF_INET, IP_SER, &server_add.sin_addr.s_addr);
    char server_ip[100] = {0};
    printf("server ip: %s\n", inet_ntop(AF_INET, &server_add.sin_addr, server_ip, sizeof(server_ip)));
    //注册子进程处理函数
    struct sigaction act;
    act.sa_handler = do_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, NULL);

    //绑定端口
    Bind(sockfd, (struct sockaddr*)&server_add, sizeof(server_add));

    //监听端口,最多允许20个客户端连接
    Listen(sockfd, 20);

    while(1)
    {
        int confd,len;
        //阻塞等待客户端连接
        confd = Accept(sockfd, (struct sockaddr*)&client_add,(socklen_t*)&len);
        
        //有客户端连接，创建新进程
        pid_t pid;
        pid = fork();
        if(pid < 0)
        {
            perr_exit("fork error");
        }
        else if(pid == 0)            //子进程完成与客户端的读写
        {
            //关闭sock_fd
            Close(sockfd);
            char buf[MAXLINE] = {0};
            char str[100];
            int n,i;

            while(1)
            {
                n = Read(confd, buf, sizeof(buf));
                if(n == 0)
                {
                    printf("the other side has been closed\n");
                    break;
                }
                printf("receive from: %s at port: %d\n", inet_ntop(AF_INET, &client_add.sin_addr, str, sizeof(str)), ntohs(client_add.sin_port));
                buf[n] = '\0';
                printf("content from client : %s\n", buf);
                for(i = 0; i < n; i++)
                {
                    buf[i] = toupper(buf[i]);
                }
                printf("server send: %s\n", buf);
                //sleep(5);
                Write(confd, buf, n);
		write(STDOUT_FILENO,buf,n);
            }
            Close(confd);
            return 0;
            
        }
        else
        {
            //关闭confd
            Close(confd);
        }
    
    
    }
    return 0;
}


