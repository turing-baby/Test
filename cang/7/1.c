#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<stdlib.h>



void cancel(void * arg)
{

    printf("释放堆空间");
    free(arg);

}



void *add(void * arg)
{
    char *p = malloc(1024);
    pthread_cleanup_push(cancel,NULL);
    while (1)
    {
        printf("你妈没了！！");
        sleep(1);
    }
    pthread_cleanup_pop(1);
    free(p);
    pthread_exit(NULL);


}


int main ()
{
    pthread_t TID;
    pthread_create(&TID , NULL , add ,NULL);
    sleep(5);
    pthread_cancel(TID);

}