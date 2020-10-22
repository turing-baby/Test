#include <stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/errno.h>
#include <sys/shm.h>


int main()
{
    key_t  key = ftok("/home/gec/yyj" ,10086);
    if (key == -1)
    {
       printf("1111");
       perror("");
    }
    
    int shm_id = shmget(key ,1024, IPC_CREAT|0777);
    if (shm_id == -1)
    {
       printf("2222");
    }
   
}