#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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
	 
	 int on=1; //1就是开启的意思

	 //   setsockopt()函数
     setsockopt(tcp_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); //开启地址服用
	 
	 
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

	 int new_socket=0;
	while(1)
	{
	//4.接收客户端的链接请求  
	     new_socket=accept(tcp_socket,NULL,NULL);   
		 if(new_socket < 0)
		 {
			perror("");
			return 0;
		 }
		 else
		 {
			printf("有客户端链接进来  %d\n",new_socket);
		 }
	   
	}
	
	 close(new_socket);
	 close(tcp_socket); 
	   
}