#ifndef     __MY_LIST__
#define     __MY_LIST__


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h> 
#include <sys/mman.h>
#include <linux/input.h>

#define     TS_PATH     "/dev/input/event0"

int sc_c()
{
    int fd= open("/dev/fb0",O_RDWR);
    int fg= open("./12.bmp" ,O_RDWR);
    lseek(fg , 54 , SEEK_SET);
    char buf_2[800*480*3] ;
    int * p_fd = (int *) mmap( NULL , 800*480*4 , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0);
    int red_1 = read(fg, buf_2 ,800*480*3 );
    int buf_3[480*800*3];   //24  huan 32

    for (int  y = 0; y < 480 ; y++)
    {
        for (int  x = 0; x < 800 ; x++)
        {
            buf_3 [x+(479-y)*800]  =      buf_2[(x+y*800)*3 + 0] << 0 | 
                                            buf_2[(x+y*800)*3 + 1] << 8 | 
                                            buf_2[(x+y*800)*3 + 2] << 16 ;  
        }
    }


      for(int y = 0 ; y < 480 ; y++)
    {
        for(int x = 0;x<800 ;x++)
        {
            *(p_fd + x + y*800)=  buf_3 [x + y*800 ] ;
        
        }
    }

    // write(fd , buf_3 ,480*800*4);
    close(fd);
    close(fg);
}

#endif