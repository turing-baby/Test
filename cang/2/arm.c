#include "my_list.h"

int main(int argc,char * argv[])
{ 
    

    int x, y , cont = 0 ;
    int color_buf[800*480];
    // 1. 打开触摸屏的设备文件
    int fd_ts = open( TS_PATH , O_RDONLY );
    if ( -1 == fd_ts )
    {
        perror("打开触摸平文件失败 ！！ ");
        return -1 ;
    }
         

    // 2. 读取文件内容
    struct input_event event; 

    while(1)
    {
        sc_c();
        read(fd_ts , &event , sizeof(struct input_event));
    


         // 3. 数据分析
        if (event.type == EV_ABS)
        {
            if (event.code == ABS_X)
            {
                x = (event.value*800)/1024;
                cont ++ ;
            }
            if (event.code == ABS_Y)
            {
                y = (event.value*480)/600;
                cont ++ ;
            }
            
        }
        
        
        if (cont != 2 )
        {
            printf("(xxxxxx)\n" , x , y );
            cont = 0 ;
        }

        if (cont == 2 )
        {
            printf("(%d,%d)\n" , x , y );
            cont = 0 ;
        }
          
        int fd_lcd = open( "/dev/fb0" , O_RDWR );
        if ( -1 == fd_lcd)
            {
            perror("打开LCD文件失败 ！！");
             return -1 ;
            }         
        int * p_fd = (int *) mmap( NULL , 800*480 , PROT_READ | PROT_WRITE , MAP_SHARED , fd_lcd , 0);
        for(int i = 0  ; i < 800 ; i++ )
        {
            printf("1111");
            color_buf[i*800 + x]= 0xFF8B22;
        }
        for(int i = 0 ;  i > 480 ; i++ )
        {
            printf("2222");
            color_buf[y + i*800]=0xFF8B22;
        }             

    for(int y = 0 ; y < 480 ; y++)
    {
        for(int x = 0;x < 800 ;x++)
        {
            printf("3333");
            *(p_fd + x + y*800) =  color_buf [x+y*800 ] ;
            printf("4444");
        
        }
    }
        close(fd_lcd);
    }



    // 5. 关闭文件
    close(fd_ts);
    
}