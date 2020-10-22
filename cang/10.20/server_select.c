#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


//解析对方的IP地址信息 
void  peer_ip(int  *socket,int people)
{
	
	int i=0;
	for(i=0;i<people;i++)
	{
		
		 struct sockaddr_in  addr={0};
		 int addrlen=sizeof(addr); 
		   
		   //通过socket  解析 IP  
		getpeername(socket[i],(struct sockaddr *)&addr,&addrlen);
		
		
		//把结构体中的信息取出来
		printf("ip:%s\n",inet_ntoa(addr.sin_addr));
		printf("port:%d\n",ntohs(addr.sin_port));
		
		
	}
	
	
	printf("____________________________________\n");
	
	
	
	
}





int main()
{
   //1.创建服务器通信对象 
   int tcp_socket = socket(AF_INET, SOCK_STREAM, 0); //创建TCP 通信 socket 
	   if(tcp_socket < 0)
	   {
		   perror("");
		   return -1; 
	   }
	   else
	   {
		    printf("创建服务器socket成功\n");
	   }
	 
	 
	//2.绑定服务器通信socket     
	 struct sockaddr_in  server_addr;  //服务器地址信息
     server_addr.sin_family  =  AF_INET;	 //IPV4 协议
     server_addr.sin_port    =   htons(8888);  //端口号
     server_addr.sin_addr.s_addr =   INADDR_ANY;//自动绑定本地网卡地址
    int ret  = bind(tcp_socket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
    	if(ret < 0)
    	{
    	    perror("");
    	    return 0; 
    	}
    	else
    	{
    	    printf("bind ok\n");
    	}   
	
	//3.设置服务器为监听模式 
	ret = listen(tcp_socket,5);
		if(ret < 0)
    	{
    	    perror("");
    	    return 0; 
    	}
    	else
    	{
    	    printf("listen ok\n");
    	} 


	int  arry[100];
	int  people=0;
	
	while(1)
	{
	//清空集合  
	fd_set fdset;
	FD_ZERO(&fdset);
	
	//1.把标准文件描述符加入到集合中   （抓犯人）
    FD_SET(0,&fdset);
	
	FD_SET(tcp_socket,&fdset);  //把服务器 socket 加入到集合中 
	
	//2.让 select 去监听这个集合    （管理犯人）
	 int ret=select(tcp_socket+1,&fdset,NULL,NULL,NULL);
		 if(ret < 0)
		 {
			perror("");
			return 0; 
		 }
		 else if(ret == 0)
		 {
			printf("超时\n");
		 }
		 else if(ret > 0)
		 {
			printf("有犯人不听话\n"); 
			
			//看看是谁不听话
			if(FD_ISSET(0,&fdset))
			{
				printf("处理输入设备\n");
				//读取标准输入设备中的数据
				char buf[1024]={0};
				read(0,buf,1024);
				
				printf("处理后的数据 %s\n",buf);
			}
			 
			if(FD_ISSET(tcp_socket,&fdset))  //判断服务器socket 是否活跃
			{
				printf("处理服务器\n");
				
				int new_socket = accept(tcp_socket,NULL,NULL); //接收链接请求  
					if(new_socket > 0)
					{
						printf("链接成功\n");
						
					}
			}
			
			
		 }
	}	
	
	 

	   
}