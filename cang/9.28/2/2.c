#include  <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
 /*
													 (当前有多少个进程映射这块共享内存)
键        shmid      拥有者      权限        字节     连接数       状态      
0x00000000 9          gec        600        524288     2           目标   

*/ 
	   

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
		 
	
    //2.开启共享内存  
    int shm_id = shmget(key,1024,IPC_CREAT|0777);	
		if(shm_id < 0)
		{
			
			perror("创建共享内存失败");
			return 0;
		}	
		else
		{
			printf("创建共享内存成功\n");
		}

  
    //3.把共享内存映射到用户空间中  
	char *p=shmat(shm_id,NULL,0); //映射
    char  arr[1024];
    sprintf(arr,"%d",getpid());
    strncat(p,arr,sizeof(arr));
   // strcpy(p,arr); //往共享内存放一个字符串


		printf("%s\n",p);
   
    

    while(1);

    //解除映射 
	shmdt(p); //解除映射，销毁之前必须解除映射
	
	//销毁共享内存  
    shmctl(shm_id,IPC_RMID,NULL);

}