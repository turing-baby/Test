#ifndef     __MY_LIST__
#define     __MY_LIST__





#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h> 
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */




int wenjian_f(char add[982])
{
    //创建管道文件
    if (access("/home/gec/fifo" , F_OK | R_OK |W_OK))
    {
        int ret = mkfifo("/home/gec/fifo" , 0666);
        if (ret < 0)
        {
            perror("");
            return -1;
        }
        
    }
    //打开管道文件
    int fifo_fd = open("/home/gec/fifo" , O_RDWR);

    //打开目标文件 主函数传参  打开需要传输的文件
    int fd = open( add , O_RDWR);
    if (fd <0 )
    {
        perror("");
        return -1;
    }
    
	while(1)
	{
		//读取目标文件
		char buf[1024] = {0};
		int size=read(fd,buf,sizeof(buf));
        printf("shu%d\n",size);
			if(size <= 0)
			{
				printf("文件发送完毕\n");
				break;
			}

		//写入目标文件
		write(fifo_fd,buf,size);
	}

    close(fd);
    close(fifo_fd);
   
    return 0;
}



int wenjian_s(char aee[1024])
{
    
	//access 文件IO, 检查文件是否 可读R_OK  可写 W_OK，存在 F_OK
    if (access("/home/gec/fifo" , F_OK | R_OK |W_OK))
    {
        int ret = mkfifo("/home/gec/fifo" , 0666);
        if (ret < 0)
        {
            perror("");
            return -1;
        }
        
    }
	
	
    //打开管道文件 ,以只读的方式打开管道， 当没有写端，则读不会阻塞,读到0 不会卡，再while（1）里面一直运行
    // 以只读的方式就不会卡，一直读到0为止。
    int fifo_fd = open("/home/gec/fifo" , O_RDONLY);
    // 当读管道是不以只读的方式打开时，以可读写的方式打开时，读到没有数据的时候会堵塞
    // int size = read(fifo_fd , buf , 8196);就是说读到这一步会一直堵塞，就是卡在着不动
    // 读出来的值读多少是 多少
    // int fifo_fd = open("/home/gec/fifo" , O_RDWR);

	  //创建一个新文件
    int new_fd = open(aee, O_RDWR |O_CREAT);
    if (new_fd < 0)
    {
        perror("");
        return -1;
    }


	while(1)
	{
		//读取管道文件数据
		char buf[8196] = {0};
		int size = read(fifo_fd , buf , 8196);
			if(size <= 0)
			{
				printf("文件接受完毕\n");
			    break;
			}
  

		//写入新文件
		write(new_fd,buf,size);
	}

    close(new_fd);
    close(fifo_fd);
    return 0;
}

#endif