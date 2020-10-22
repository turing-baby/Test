#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



struct  msgbuf
{
    long msgtyp;
    char msginfo[1024];
};


int main(int argc,char *argv[])
{
    key_t key;
    key=ftok("/home/gec",10086);
    if(key<0)
    {
        perror("");
        return -1;
    }
    else
    {
        printf("获得key值成功\n");
    }


    int msg_id =msgget(key,IPC_CREAT|0777);
    if(msg_id<0)
    {
        perror("");
        return -1;
    }
    else 
    {
        printf("消息队列创建成功！\n");
    }
    printf("我的pid为：%d\n",getpid());
    pid_t pid=fork();
    
    if(pid<0)
    {
        perror("");
        return -1;
    }
    if(pid==0)  //子进程发送
    {   
        while(1)
        {
            struct msgbuf buf;
            char p[20]={0};
            printf("请输入你想传给谁：\n");
            scanf("%s",p);           
            bzero(buf.msginfo,1024);
            printf("请输入你要传输的数据：\n");
            scanf("%s",buf.msginfo);
            buf.msgtyp=atoi(p);
            msgsnd(msg_id,&buf,strlen(buf.msginfo),0);
            if(strstr(buf.msginfo,"exit"))
            {
                kill(getppid(),9);
                msgctl(msg_id,IPC_RMID,NULL);
                exit(0);
            }
        }
    }
    else  //父进程收
    {
        //atexit(queue_exit);
        while(1)
        {
            struct msgbuf newbuf;
            bzero(newbuf.msginfo,1024);
            msgrcv(msg_id,&newbuf,1024,getpid(),0);
            printf("from PID:%ld:%s\n",newbuf.msgtyp,newbuf.msginfo);
            if(strstr(newbuf.msginfo,"exit"))
            {
                kill(pid,9);
                msgctl(msg_id,IPC_RMID,NULL);
                exit(0);
            }


        }
    }
    return 0;
}