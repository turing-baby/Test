#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
 
 
int main(int argc, char const *argv[])
{  
     int j ;
    scanf("%d",&j);
    for ( int i = 0 ; i < j; ++i)//创建子进程个数
    {
        pid_t pid = fork() ;
    	if ( 0 == pid)//判断是否为子进程
    	{  
           pause();
            exit(0);
    	}
    	//waitpid(pid,NULL,WUNTRACED);  等待当前子进程结束
    }
    printf("\n");
    printf("父进程:%d\n",getpid());
    while(1);
	return 0;
}