#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<stdlib.h>

int main()
{
    char s[1024];
    char buf[1024];
    int fd_fifo = open("/tmp/fifo", O_RDWR);
     if (fd_fifo == -1)
    {
        perror("");
        return -1;
    }
    int i = 0;
    
       if( access(s, F_OK | R_OK | W_OK ) )
        {
            read(fd_fifo,s,1024);
            sprintf(buf,"touch %s ",s);
            system(buf);
            sprintf(buf,"chmod 777  %s ",s);
            system(buf);
        }
    FILE * fd =fopen(s,"w+");
    while(1)
    { 
        read(fd_fifo,s,1024);
        printf("%s",s);
        fclose(fd);
    }
    close(fd_fifo);
    
     return 0;
}