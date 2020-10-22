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

struct add
{
    int socket;
    struct sockaddr_in  qq;

};

//发送数据
void *write_clien(void *arg)
{
   //1.创建UDP socket  
	   int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
		  if(udp_socket < 0)
		  {
			  perror("");
			  return 0;
		  }
	
	   
	   //2.设置目标的IP地址信息       
	 struct sockaddr_in  dest_addr; 
     dest_addr.sin_family  =  AF_INET;	 //IPV4 协议
     dest_addr.sin_port    =   htons(2000);  //端口号
     dest_addr.sin_addr.s_addr =  inet_addr("192.168.71.19");
    char  buf[100];
	while(1)
	{
	  //发送数据
		bzero(buf,1024);
        printf("发送数据->\n");
        scanf("%s",buf);
		int size=sendto(udp_socket,buf,strlen(buf),0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
		printf("size =%d\n",size);
	}
    close(udp_socket);
}



int main(int argc ,char * argv[])
{ 
	int i;
	printf("选择需要的功能\n");
	printf("1.单播\n");
	printf("2.组播\n");
	printf("3.广播\n");
	scanf("%d",&i);
	switch (i)
	{
	case 1:
		{   
			
		}
		break;
	
	default:
		break;
	}


    //创建一个线程给客户端发送数据  
   pthread_t tid;
   pthread_create(&tid,NULL,write_clien,NULL);
     //1.创建UDP socket  
	   int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
		  if(udp_socket < 0)
		  {
			  perror("");
			  return 0;
		  }
	
     //2.绑定服务器通信socket     
	 struct sockaddr_in  server_addr;  //服务器地址信息
     server_addr.sin_family  =  AF_INET;	 //IPV4 协议
     server_addr.sin_port    =   htons(6666);  //端口号
     server_addr.sin_addr.s_addr =   INADDR_ANY;//自动绑定本地网卡地址
     int ret  = bind(udp_socket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
		if(ret < 0)
		{
			perror("");
			return 0;
		}
	 
	 while(1)
	 {
		 //3.接收数据
		char  buf[1024]={0};
		struct sockaddr_in  send_addr;
		int len  = sizeof(send_addr);
		int size = recvfrom(udp_socket,buf,1024,0,(struct sockaddr *)&send_addr,&len);
        printf("发送者 %s 端口 %d  size %d 数据 %s\n",inet_ntoa(send_addr.sin_addr),ntohs(send_addr.sin_port),size,buf);
	 }
    
    close(udp_socket);

}