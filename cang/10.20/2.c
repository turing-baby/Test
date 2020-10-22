#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <pthread.h>
#include <sys/types.h>  
#include <sys/stat.h> 
#include <fcntl.h>

//演示：监听 标准输入设备  ： 0   1   2 

int main()
{

	int  tcp_socket  = socket(AF_INET , SOCK_STREAM , 0);
    if (tcp_socket == -1)
    {
        perror("创建失败！\n");
        return 0;
    }
    
    printf("创建成功！\n");

    struct sockaddr_in  server_adddr ;
    server_adddr.sin_family  =  AF_INET ;
    server_adddr.sin_port    =  htons(2006);
    server_adddr.sin_addr.s_addr  =  inet_addr("192.168.71.84");//对服务器的ip及端口
    int ret = connect(tcp_socket,(struct sockaddr *)&server_adddr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("链接失败！\n");
        return 0;
    }
    printf("连接成功！\n");



	while(1)
	{
	//清空集合  
	fd_set fdset;
	FD_ZERO(&fdset);
	
	//1.把标准文件描述符加入到集合中   （抓犯人）
    FD_SET(tcp_socket,&fdset);
	FD_SET(0,&fdset);
	
	
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
			if(FD_ISSET(0,&fdset))
			{
				printf("输入设备：\n");
				char aee[1024]={0};
				read(0,aee,1024);
				printf("输入的数据： %s\n",aee);  
                printf("向服务器发送的数据：%s \n",aee);
                write(tcp_socket,aee, strlen (aee));
			}
			if(FD_ISSET(tcp_socket,&fdset))
			{
				printf("服务器:\n");
				char buf[1024];
				bzero(buf,1024);
				int a = read(tcp_socket,buf,1024);
				if(a <= 0)
				{
					perror("服务器已断开 ！！\n");
					break ;
				}
				printf("\n");
				printf("----------------------\n");
				printf("接收到服务器消息 -> %s\n",buf);
				printf("----------------------\n");
				printf("\n");
			}	 
		 }
	}	
	
	return 0;
}