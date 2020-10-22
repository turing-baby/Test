#include "my.h"

void Ts_exit(int fd_ts);
int Ts_init( void  );
void ts_get(int *x, int *y , int fd_ts  );
void send_cmd(int fd_fifo , char * cmd );

#define     FIFO_PATH   "/tmp/fifo"

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

    // 初始化触摸屏
     int fd_ts = Ts_init();
     if (-1 == fd_ts)
     {
         return -1 ;
     }
     

    //获取坐标值
    int x , y ;
    while(1)
    {
        ts_get(&x, &y , fd_ts );
        if(x < 100)
        {
            send_cmd(fd_fifo , "seek -5\n");
        }
        if(x > 700)
        {
            send_cmd(fd_fifo , "seek +5\n");
        }

    }


    // 关闭退出
    Ts_exit( fd_ts);


    return 0;
}
void send_cmd(int fd_fifo , char * cmd )
{
    int ret = write(fd_fifo , cmd , strlen(cmd));
}