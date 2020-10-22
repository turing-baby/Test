#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>



/*
头文件:
    #include <sys/types.h>
    #include <sys/stat.h>
原型:
    int mkfifo(const char *pathname, mode_t mode);
参数分析:
    Pathname  -->  需要创建管道的名字+路径
    Mode --> 管道文件的权限设置
返回值:
    成功 0
    失败 -1
*/

void * TS_THREAD(void * arg )
{
    int fd_ts = *(int *)arg ;
    ts_xy xy ; 
    while(1)
    {
        xy = ts_get( fd_ts  );
        printf("(%d,%d)\n" , xy.x , xy.y);
        if (xy.x> 0 && xy.x < 800 && xy.y > 0 && xy.y < 480)
        {
            flag = 1 ;
        }
        

    }


}




main()
{


    pthread_t TID ;
    pthread_create(&TID, NULL ,TS_THREAD, &fd_ts );
    
    while(1)
    {
        if (flag == 1 )
        {
            printf("干一个活！！ \n");
            printf("搞定， 又睡觉去！！ \n");
            flag = 0 ;
        }
        
        

    }


    return 0;


}