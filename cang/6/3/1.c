#include<stdio.h>
#include <pthread.h>
#include<unistd.h>
#include<stdlib.h>


typedef struct arr
{
    char j[1024] ; 
    int  c ;

}arr , *P_arr ;


void * TS_THREAD(void * arg )
{
    P_arr b = arg ;
    while(1)
    {
        printf("b = %d\n",b->c);
        sleep(3);
        static int count = 10086;
        pthread_exit((void*)(&count));

    }


}



int main()
{
    int a = 0;
    int b = 0;
    P_arr  car = malloc(sizeof(arr));
    car->c = 100;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr,99999*5);
    pthread_t TID ;
    pthread_create(&TID, &attr ,TS_THREAD, car );
    void * stackaddr = NULL;
    size_t stacksize = 0;
    int red = pthread_attr_getstack(&attr,&stackaddr,&stacksize);
    pthread_attr_getstacksize(&attr,99999*5);

    
    while(1)
    {
        printf("a = %d\n",a++);
        sleep(3);
        int * ret;
        pthread_join(TID,(void**)&ret);
        printf("线程 退出 %d \n",*