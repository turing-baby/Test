#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int  system_l(char * name);
int main(int argc, char const *argv[])
{
	system_l("touch 1.c");


}


int  system_l(char * name)
{
	
	char ch1[] = "touch 1.c";
	char * ch2 = " ";
	char * p = strtok(ch1,ch2);
	char * a[3];
	int i = 1;
	a[0]=p;
	while(1)
	{
		p = strtok(NULL,ch2);

		
		a[i] = p;
		i++;
		if (p == NULL)
		{
			break;
		}	
	}
	for(int j=0;j<3;j++)
	{
		printf("a[%d]:%s\n",j,a[j]);
	}
	pid_t pid;
	pid = fork();
	if (pid == 0)
	{
		printf("子进程执行\n");
		execvp(a[0],a);
		printf("子进程结束！\n");
		return 0;
	}
	printf("父进程等待子进程结束\n");
	wait(NULL);
	sleep(1);
	printf("子进程退出成功！\n");

	return 0;

}

