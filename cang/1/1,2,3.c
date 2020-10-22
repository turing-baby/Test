#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <errno.h>


int main()
{    //打开文件
    char * path  = "./test.txt"; 
    char * path_1  = "./test_1.txt";       //定义path的位置  文件夹位置 
    char * path_2  = "./test_1.txt";                       
    int fd = open(path , O_RDWR | O_CREAT);    //
    if (-1 == fd)
    {
        perror("open file erroe");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
     printf("%d \n ",fd); 
     close(fd);
    int fp = open(path_1 , O_RDWR | O_CREAT);    //
    if (-1 == fp)
    {
        perror("open file erroe");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
    char arr[99] = "0123456789";
    int i = write(fp , arr ,  10);
    if (-1 == i)
    {
        perror("open file erroe");
        printf("1111r:%s \n ",strerror(errno));
        return  -1;
    }
    close(fp);
    printf("%c \n", arr[0]);
    int fg = open(path_2 , O_RDONLY | O_EXCL | O_CREAT);    //
    if (-1 == fp)
    {
        perror("open file erroe");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
    char add[] = {};
    lseek(fg , 3 ,SEEK_SET);
    read(fg , add , 3);
    //lseek(fg ,3, SEEK_SET );
    printf("%c\n", arr[0]);
    close(fg);

    return 0 ;
    

}