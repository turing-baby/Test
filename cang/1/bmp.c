#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define     LCD_PATH    "/dev/fb0"
#define     BMP_PATH    "./1.bmp"
#define     WIDTH           800
#define     HEIGHT          480
#define     BMP_PIXEL       3
#define     BMP_SIZE        WIDTH*HEIGHT*BMP_PIXEL
#define     LCD_SIZE        WIDTH*HEIGHT


int main(int argc, char const *argv[])
{
    // 打开文件
    int fd_lcd = open( LCD_PATH , O_RDWR );
    if ( -1 == fd_lcd)
    {
        perror("open LCD file error");
        return -1 ;
    }

    //2.   打开图片文件
     int fd_bmp = open( BMP_PATH , O_RDONLY );
    if ( -1 == fd_bmp )
    {
        perror("open  bmp file error");
        return -1 ;
    }

    // 3.   读取图片的数据
    char buf_bmp [ BMP_SIZE ] ;
    int ret = read(fd_bmp, buf_bmp , BMP_SIZE);
    printf("read:%d \n" , ret);


    //4.   把24位的图像数据转换为32为LCD数据
    int buf_lcd [ LCD_SIZE ] ;
    int y , x ;

    for ( y = 0; y < HEIGHT ; y++)
    {
        for ( x = 0; x < WIDTH  ; x++)
        {
            buf_lcd [x+y*800]  =    buf_bmp[(x+y*800)*3 + 0] << 0 | 
                                    buf_bmp[(x+y*800)*3 + 1] << 8 | 
                                    buf_bmp[(x+y*800)*3 + 2] << 16 ;  
        }
    }
    

    //5.   把转换后的数据显示到屏幕中
    write (fd_lcd  , buf_lcd  , LCD_SIZE*4 );


     
    //关闭文件
    close(fd_lcd);
    close(fd_bmp);


    

    return 0;
}