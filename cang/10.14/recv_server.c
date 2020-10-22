#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[])
{
	//参数  ./recv    port   file  
	if(argc != 3)
	{
		printf("请输入参数  ./recv    port   file  \n");
		return 0;
	}
	
	
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
     server_addr.sin_port    =   htons(atoi(argv[1]));  //端口号
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

	
	//4.接收客户端的链接请求  
	    int  new_socket=accept(tcp_socket,NULL,NULL);   
		 if(new_socket < 0)
		 {
			perror("");
			return 0;
		 }
		 else
		 {
			printf("有客户端链接进来  %d\n",new_socket);
		 }
	   
	    //新建文件 
		int fd = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("");
				return 0;
			}
		
		//接收文件的大小  
		char cmd[1024]={0};
		int file_size=0;
		read(new_socket,cmd,1024);    //file:110540\n
		if(strstr(cmd,"file:"))
		{
			//获取文件大小
			sscanf(cmd,"file:%d\n",&file_size);
			printf("对方的文件大小为 %d\n",file_size);
			
			//应答
			write(new_socket,"recv_ok",strlen("recv_ok"));
		}
		else
		{
			printf("接收错误\n");
			return 0;
		}
		
		int recv_size=0;
		while(1)
		{
			char buf[1024]={0};  
			int size = read(new_socket,buf,1024);  //读取网络数据
				recv_size+=size; 
				if( recv_size == file_size)
				{
					write(fd,buf,size);
					printf("接收完毕\n");
					
					//应答
					write(new_socket,"recv_end",strlen("recv_end"));
					
					break;
				}
			
			printf("recv_size:%d\n",recv_size);
			//写入到本地文件中 
			write(fd,buf,size);
			
			
		}
		
	
	 close(fd);
	 close(new_socket);
	 close(tcp_socket); 
	   
}