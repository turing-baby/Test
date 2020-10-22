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
        char buff[1024]; //拷贝文件的缓冲区，一次读1024
        if(0 != read(fd,buff, 1024))
        {
            write(fp, buff, sizeof(buff));
        }
        
        break ;
    }
     pthread_exit(0);

}


int main()
{
    char buf[1024];

    DIR * fd_dir = opendir("./");

    if(NULL == fd_dir)
    {
        // perror("open dir error!!");
        printf("open %s error %s \n" , "./" , strerror(errno));
        return -1 ;
    
    }

    while(1)
    {
        struct dirent * dir_info  =  readdir(fd_dir);

        if (NULL == dir_info )
        {
            perror("read dir error ");
            break;

        }

        printf("dir_info->d_name=%s\n",dir_info->d_name);

        P_arr  car = malloc(sizeof(arr));

       strcpy(car->j  , dir_info->d_name);

        if (dir_info->d_type== 8 )
        {

            pthread_t TID ;
            
            pthread_create(&TID, NULL ,TS_THREAD, car );

            //pthread_join(TID , NULL);   //多线程等还是不等？

        }
        
    }


}