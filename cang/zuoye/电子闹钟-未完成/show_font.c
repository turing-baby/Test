#include "font.h"  //添加字库 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 
#include <sys/mman.h>
#include <linux/input.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>





unsigned int *lcd;

#define     WIDTH           800
#define     HEIGHT          480
#define     BMP_PIXEL       3
#define     BMP_SIZE        WIDTH*HEIGHT*BMP_PIXEL
#define     LCD_SIZE        WIDTH*HEIGHT



void ts_get(int *x, int *y , int fd_ts  )// 2. 读取文件内容
{
    int cont = 0 ;   
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
        if (cont == 2 )
        {
            printf("(%d,%d)\n" , *x , *y );
            cont = 0 ;
            return ;
        }
        
    }

    return ;
}


int xianshi(char * arr)
{
    
    // 打开文件
    int fd_lcd = open("/dev/fb0" , O_RDWR );
    if ( -1 == fd_lcd)
    {
        perror("open LCD file error");
        return -1 ;
    }



    // 内存映射
    int * p_lcd  = (int *)mmap( NULL ,   //内存地址的开始地址 (通常设为 NULL, 代表让系统自动选定地址)
                                LCD_SIZE*4 ,    // 宽 * 高 * 每个像素点4字节
                                PROT_READ|PROT_WRITE,  //映射区域可被读取 映射区域可被写入
                                MAP_SHARED,  //对应射区域的写入数据会复制回文件内
                                fd_lcd,      //  LCD的描述符
                                0) ;         // 不需要偏移
    if (MAP_FAILED == p_lcd)
    {
        perror("内存映射失败 ！！ \n");
        return -1 ;
    }
    

    //2.   打开图片文件
     int fd_bmp = open( arr , O_RDONLY );
    if ( -1 == fd_bmp )
    {
        perror("open  bmp file error");
        return -1 ;
    }
    // 偏移读写位置
    lseek(fd_bmp , 54 , SEEK_SET);

    // 3.   读取图片的数据
    char buf_bmp [ BMP_SIZE ] ;
    bzero(buf_bmp , BMP_SIZE ); // 清空数组
    int ret = read(fd_bmp, buf_bmp , BMP_SIZE);

printf("1111");
    //4.   把24位的图像数据转换为32为LCD数据
    int buf_lcd [ LCD_SIZE ] ;
    int y , x ;

    for ( y = 0; y < HEIGHT ; y++)
    {
        for ( x = 0; x < WIDTH  ; x++)
        {
            buf_lcd [x+(479-y)*800]  =      buf_bmp[(x+y*800)*3 + 0] << 0 |         // B
                                            buf_bmp[(x+y*800)*3 + 1] << 8 |         // G
                                            buf_bmp[(x+y*800)*3 + 2] << 16 ;        // R
                                        
        }
    }

    //5.   把转换后的数据显示到屏幕中
   for ( y = 0; y < HEIGHT ; y++)
    {
        for ( x = 0; x < WIDTH  ; x++)
        {
            *(p_lcd+x+y*800) =  buf_lcd [x+y*800] ;
        }
    }


    //关闭文件
    close(fd_lcd);
    close(fd_bmp);
    return 0;


}


int Ts_init( void  )
{

     // 1. 打开触摸屏的设备文件
    int fd_ts = open( "/dev/input/event0" , O_RDONLY );
    if ( -1 == fd_ts )
    {
        perror("打开触摸平文件失败 ！！ ");
        return -1 ;
    }
    return fd_ts ;
}

//初始化LCD 
void init_lcd()
{
	int fd=open("/dev/fb0",O_RDWR);   //打开lcd
	if(fd==-1)
	{
		perror("");	
		return;
	}
	printf("open lcd ok\n");
	//映射lcd设备
	lcd=mmap(NULL,800*480*4,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	if(lcd==MAP_FAILED)
	{
		perror("");
		return;
	}
	printf("mmap ok\n");	
}


//时钟功能
void shizhong()
{
	  //获取秒数
	time_t tloc;
	time(&tloc);	
	//转换时间 
	char *t=ctime(&tloc);
	//返回 时间结构体
	struct tm *t1=localtime(&tloc);  
	char buf[1024];
	char buff[1024];
	sprintf(buf,"%02d  :  %02d  :  %02d\n",t1->tm_hour,t1->tm_min,t1->tm_sec);
	sprintf(buff,"%d  /  %d  /  %d\n",t1->tm_year+1900,t1->tm_mon+1,t1->tm_mday);
        

	//1.初始化字库
	font *tp = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	font *tp_2 = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	//设置字体的大小
	fontSetSize(tp,90);
	fontSetSize(tp_2,30);
	//2.创建一个字体的输出框  
	//bitmap *bp=createBitmap(100,100,4);   //白色的字体板  
	                            //getColor(a, b, c, d) A B   G R  
	bitmap *bp=createBitmapWithInit(800,300,4,getColor(0,255,255,255));  //指定颜色
	//把字体填充到输出框中 
	fontPrint(tp,bp,200,70,buf,getColor(0,0,0,0),0);
	fontPrint(tp_2,bp,520,230,buff,getColor(0,0,0,0),0);
	
	//把字体框输出到 LCD 设备中
	show_font_to_lcd(lcd,0,90,bp);
	//关闭字体，关闭画板
	fontUnload(tp);
	fontUnload(tp_2);
	destroyBitmap(bp);
}

void sig(int fd_ts)
{
	int i = 1 ;
	int x_1 , y_1 ;
	while(1)
    {
		
		//获取秒数
		time_t tloc;
		time(&tloc);	
		//转换时间 
		char *t=ctime(&tloc);
		//返回 时间结构体
		struct tm *t1=localtime(&tloc);  
		char buf[1024];
		char buff[1024];
		sprintf(buf,"%02d  :  %02d  :  %02d\n",t1->tm_hour,t1->tm_min,t1->tm_sec);
		sprintf(buff,"%d  /  %d  /  %d\n",t1->tm_year+1900,t1->tm_mon+1,t1->tm_mday);



        

        if(i ==  1)
            {
				shizhong();  //显示时钟
                sleep(1);
            }
		ts_get(&x_1, &y_1 , fd_ts );
        if( 660<= x_1 && x_1 <= 800 &&  420 < y_1 && y_1 < 480 )
        {
            char  arr[]= {"./1.bmp"}; i = 0;
			xianshi(arr);
        }
       
	   



        else if( 420<= x_1 && x_1 <= 740 &&  350< y_1 && y_1 < 410 )
        {
               system("poweroff");  //退出
               close(fd_ts);   
        }
        
        
    }
  

        


	




}



int main()
{   
	
	char  arr[]= {"./zhubeijin.bmp"};
	xianshi(arr);


	//1.注册一个时钟信号 
	signal(14,sig);
	//映射LCD  
	init_lcd();
	 //打开触摸屏
    int fd_ts = Ts_init();
    if ( -1 == fd_ts)
    {
        perror("open CMP file error");
        return -1 ;
    }
	sig(fd_ts);

 
	while(1);
	
}