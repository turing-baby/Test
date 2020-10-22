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


void * TS_THREAD(void * arg )
{




}



int main()
{
    DIR * fd_dir = opendir("/");

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