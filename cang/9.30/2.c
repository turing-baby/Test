#include<stdio.h>
#include"thread_pool.h"


struct arr
{
    int a ;
};
void *fuck(void *arg)
{
    
    int a = *(int *)arg ;printf("this is fuck :");
    printf("%d\n", a );

}


void *fuck1(void *arg)
{
    
    char * a = (char *)arg ; printf("this is fuck1 :");
    printf("%s\n", a );


}



void *fuck2(void *arg)
{


}


int main()
{
    thread_pool *pool = malloc(sizeof(thread_pool));


    init_pool(pool,3);
    int k = 80 ;
    struct arr *  p = NULL;
    add_task(pool , fuck , &k );
    add_task(pool , fuck1, "90" );
    add_task(pool , fuck2, p );



    pause();
}