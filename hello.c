#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char* argv[])
{
    int pid;
    pid = fork();
    if(pid > 0)
    {
        while(1)
        {
            sleep(1);
            printf("parent:hello world\n");
        }
    }
    else if(pid == 0)
    {
        while(1)
        {
            sleep(2);
            printf("child: hello world\n");
        }
    }
    else
    {
        perror("fork error");
        exit(1);
    }
    exit(0);
}
