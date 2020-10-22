#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>

int main()
{
    int fd[2];
    int add = pipe(fd);
    if (add < 0)
    {
        perror("");
        return 0;
    }
    pid_t pid = fork();
    if (pid == 0 )
    {
       char arr[1024];
       read(fd[0],arr,1024);
       printf("子进程读取到的数据：%s\n",arr);
    }
    else if (pid > 0 )
    {
       write(fd[1],"hello",5);
    }
    close(fd[0]);
    close(fd[1]);
    int i;
    wait(&i);
    return 0 ;
}