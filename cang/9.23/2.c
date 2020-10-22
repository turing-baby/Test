#include<stdio.h>
#include<string.h>
#include <stdio.h>
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
#include <pthread.h>


int main(int argc, char const *argv[])
{
     DIR * fd_dir = opendir("./");
    if(NULL == fd_dir)
    {
        // perror("open dir error!!");
        printf("open %s error %s \n" , "./", strerror(errno));
        return -1 ;
    
    }
    DIR * fd_dir_1 = opendir("./");
    if(NULL == fd_dir_1)
    {
        // perror("open dir error!!");
        printf("open %s error %s \n" , "./1/", strerror(errno));
        return -1 ;
    
    }
    FILE  * fd_txt = fopen( "./1.txt" , "w+");
    if(fd_txt  == NULL)
    {
        perror("open CMP file error");
        return -1 ;
    }
    
    char buf[1024];

    while(1)
    {
          struct dirent * dir_info  = readdir(fd_dir);

        if (NULL == dir_info )
        {
            perror("read dir error ");
            break;

        }
        printf("dir_info->d_name=%s\n",dir_info->d_name);
      if (dir_info->d_type== 8 )
        {
            fprintf(fd_txt,"./%s\n", dir_info->d_name);
            fflush(fd_txt);
            sprintf(buf,"cp  ./%s  ./1/ ",dir_info->d_name);
            system(buf); ///system应该也是建进程吧。。。。。
        }
        
    }
    fclose(fd_txt);
    closedir(fd_dir);
    closedir(fd_dir_1);
    return 0;
}

