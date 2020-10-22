#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
 #include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#define     FIFO_PATH   "/tmp/fifo"


void send_cmd(int fd_fifo , char * cmd );


int main(int argc, char const *argv[])
{
    system("mplayer -input file=/tmp/fifo -slave -quiet 1.avi &");

    if( access(FIFO_PATH, F_OK | R_OK | W_OK ) )
    {
        if(mkfifo(FIFO_PATH, 0666 ) == -1 )
        {
            perror("管道文件创建失败!! ");
            return -1 ;
        }
    }

    int fd_fifo = open(FIFO_PATH , O_RDWR);
    if (-1 == fd_fifo)
    {
        perror("打开管道文件失败!! ");
        return -1 ;
    }


    int ret ;
    while(1)
    {

        send_cmd(fd_fifo , "seek +5\n");
        sleep(5);
     
    }

    return 0;
}


void send_cmd(int fd_fifo , char * cmd )
{
    int ret = write(fd_fifo , cmd , strlen(cmd));
    printf("写入字节数为:%d \n " , ret );

}