
int xianshi(char * arr)
{
    
    // 打开文件
    printf("444");
    int fd_lcd = open( LCD_PATH , O_RDWR );
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
printf("2222");
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