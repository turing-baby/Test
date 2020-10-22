#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/mman.h>
#include <linux/input.h>
#include <stdbool.h>




typedef struct arr
{
    char j[100] ; 
    int  c ;

}arr , * P_arr ;



void * TS_THREAD(void * arg )
{
    P_arr b = arg ;
    int fd = open( b->j ,O_RDONLY );  //打开当前文件夹下普通文件
    if(fd  == 0)
    {
        perror("open CMP file error");
       pthread_exit(0);
    }
    char  bufff[1024];
    sprintf(bufff , "./3/%s",b->j);   //指定目录为当前目录中  3  这个文件夹
    int fp = open( bufff , O_WRONLY | O_CREAT ); //在指定目录中打开普通文件，如r果没有则新建
    if(fp  == 0)
    {
        perror("open CMP file error");
       pthread_exit(0);
    }
    while(1)
    {
        c