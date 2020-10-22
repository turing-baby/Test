#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>


#define     TS_PATH     "/dev/input/event0"


int Ts_init( void  )
{

     // 1. 打开触摸屏的设备文件
    int fd_ts = open( TS_PATH , O_RDONLY );
    if ( -1 == fd_ts )
    {
        perror("打开触摸平文件失败 ！！ ");
        return -1 ;
    }
    return fd_ts ;
}

void ts_get(int *x, int *y , int fd_ts  )
{
    int cont = 0 ;
   
       

    // 2. 读取文件内容
    struct input_event event; 

    while(1)
    {
        read(fd_ts , &event , sizeof(struct input_event));
    


         // 3. 数据分析
        if (event.type == EV_ABS)
        {
            if (event.code == ABS_X)
            {
                *x = (event.value*800)/1024;
                cont ++ ;
            }
            if (event.code == ABS_Y)
            {
                *y = (event.value*480)/600;
                cont ++ ;
            }
            
        }
        
        // 4. 为所欲为
        // printf("Time:%d:%d \t Type:%d \t Code:%d \t Value:%d \n",
        //         event.time.tv_sec,
        //         event.time.tv_usec,
        //         event.type,
        //         event.code,
        //         event.value );
        if (cont == 2 )
        {
            // printf("(%d,%d)\n" , x , y );
            cont = 0 ;
            return ;
        }
        
        
    }

    return ;
}


void Ts_exit(int fd_ts)
{

    // 5. 关闭文件
    close(fd_ts);

}