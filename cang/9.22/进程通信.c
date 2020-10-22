#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

int main()
{
  
    int a=0;
	int fd = open("./mytest.txt",O_RDWR);
	
    pid_t pid=fork();
    if(pid == 0)  //子进程
     {
		 while(1)
		 {
			
			char buf[1024]; 
			scanf("%s",buf);
			lseek(fd,0,SEEK_SET);
			write(fd,buf,1024);
		 }
     }
        if(pid > 0)  //父进程
        {
			while(1)
			{
				char  buf[1024];
				char  buff[1024];
				
				for(int i = 0 ; i < 1024 ; i++)
				{
					buff[i] = buf[i] ;
				}
				lseek(fd,0,SEEK_SET);
				read(fd,buf,sizeof(buf));
				
				if(strcmp(buf , buff) != 0)
				{	
					printf("%s\n", buf);
				
				}
					sleep(2);
			}
        }
    
} 
