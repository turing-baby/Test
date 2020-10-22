#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<fcntl.h>
#include <sys/stat.h>
#include<stdlib.h>


int main (int argc,char *argv[])
{
    char s[1024];
    FILE * fd =fopen(argv[0],"r");
    if(fd == NULL )
    {
        perror("");
        return -1;
    }   
    if( access("/tmp/fifo1111", F_OK | R_OK | W_OK ) ) 
    {
        if(mkfifo("/tmp/fifo1111", 0777)  == -1 )
        {
            perror("管道文件创建失败!! ");
            return -1 ;
        }
    }
    int fd_fifo = open("/tmp/fifo", O_RDWR);
    if (fd_fifo == -1)
    {
        perror("");
        return -1;
    }
    write(fd_fifo,argv[0],sizeof(argv[0]));

    while(fgets(s, 1023, fd) != NULL)
    {
      write(fd_fifo, s, strlen(s));
      printf("%s",s);
    }
   close(fd_fifo);
   fclose(fd);



    return 0;
}