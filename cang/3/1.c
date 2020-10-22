#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include<stdbool.h>


typedef struct sub
{
    int num ;
    double score ;
    struct sub  * r ;
    struct sub  * l ;

}Sub , * P_Sub;


//初始化
Sub * new_sub(int num  , double score )
{
    P_Sub new = malloc(sizeof(Sub));
    if(NULL == new)
    {
        return NULL ;
    }
    // 初始化 该节点的前趋指针和后继指针都指向自己
    new->r=new->l = new ;
    // 把数据存放到内存中
    new->num = num ;
    new->score = score;

    // 返回堆内存地址
    return new ;

}



bool ins_node_2_front(Sub * node , Sub * new )
{
    // 判断链表是否没有初始化 ，  或者 新节点是否存在
    if ( node == NULL || new == NULL )
    {
        return false ;
    }
    
    // 1. 先把头的next的前趋指针的后继指针
    new->l = node->r->l;  //   new->l = node ;
    // 2. 把头节点的前趋指针的后继指针指向新节点
    node->r->l = new ;
    // 3. 把头节点的前趋指针赋值给新节点
    new->r = node->r ;
    // 4. 把头的前趋指针指向新节点
    node->r = new ;

    return true ;
}


int main(int argc, char const *argv[])
{

    //打开文件
    FILE * fp = fopen("test.txt", "w+");
    if ( fp == NULL )
    {
    //    perror("open file error !! ");
        printf("open XXX error : %s \n" , strerror(errno));
        return -1 ;
    }


    //写入信息
    Sub even ;
    Sub * head = new_sub(0 , 0);
    if (NULL == head)
        {
            printf("初始化失败\n");
            return -1;
        }
    for(int h = 0 ; h < 3 ; h++)
    {
        int num ;
        double score ;
        //memcpy(even.name, data->name , strlen(data->name)+1) ;
        // memcpy(even.name, "xiaoli" , strlen("xiaoli")+1);
        printf("请输入编号：\n");
        scanf("%d",&num);
        //even.num = 7 ;
        printf("请输入成绩：\n");
        scanf("%lf",&score);
        //even.score = 3.1415926;   
        Sub * new = new_sub(num , score); 
         if(!(ins_node_2_front(head , new )))
        {
            printf("添加失败  ！！ \n");
            break ;
        }
           
        int ret = fwrite(&even, sizeof(Sub), 3 , fp );
    }
    //偏移  从头开始
    fseek(fp , 0 , SEEK_SET );
    Sub stu1 ;

    //输出打印
    int red = fread(&stu1, sizeof(Sub), 3  , fp );
    printf(" num:%d score:%.2lf \n " , stu1.num , stu1.score);

    fclose(fp);



    return 0;
}
