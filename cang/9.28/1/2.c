#include  <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
//定义消息结构体
 struct msgbuf {
               long mtype;       /* 消息的类型, must be > 0 */
               char mtext[1024];    /*消息数据,数据的长度由用户自定义*/
};

int main()
{
	
	//1.获取一把钥匙  
	key_t  key=ftok("/home/gec",10086); 
		 if(key < 0)
		 {
			perror("");
			return 0; 
		 }
	     else
		 {
			printf("获取密钥成功\n");
		 }
	
	
	
	
	 //2.开启消息队列 
	 int msg_id=msgget(key,IPC_CREAT|0777);
	      if(msg_id < 0)
		  {
			  perror("创建失败");
			  return 0;
		  }
		  else
		  {
			  printf("创建消息队列成功\n");
		  }

		pid_t pid = fork();
    
		if (pid > 0)
		{
			while (1)
			{
				struct msgbuf  buf; int  i;char arr[100];
				printf("请选择要发送的对象为1 ，2 ，3。\n");
				scanf("%d \n",&i);//设置消息的类型
				buf.mtype = (long) i ;
				if(buf.mtype !=  1 || 2  || 3) 
				{
					printf("内置发送对象为1 ，2 ，3。请重新选择：\n");
					break ;
				}
				//buf.mtype =   100;
				scanf("%s",buf.mtext);
				//strcpy(buf.mtext,arr);  //3.发送数据到消息队列中 
				msgsnd(msg_id,&buf,strlen(buf.mtext),0); 
			}
		}



		if (pid == 0)
		{
			
			while (1)
			{
				struct msgbuf  new_buf; 
				msgrcv(msg_id,&new_buf,1024,2,0);//数据类型内置为 3
				printf("接收到的数据 %s\n",new_buf.mtext);
			}
    
		}

       //5.销毁消息队列 
	    msgctl(msg_id,IPC_RMID,0);
	
}