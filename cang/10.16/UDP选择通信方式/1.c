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
void *write_clien(void *arg)
{
		 //1.创建UDP socket  
	   int udp_socket_1 = socket(AF_INET, SOCK_DGRAM, 0);
		  if(udp_socket_1 < 0)
		  {
			  perror("");
			  return 0;
		  }
	
	   
	   //2.设置目标的IP地址信息       
	 struct sockaddr_in  dest_addr; 
     dest_addr.sin_family  =  AF_INET;	 //IPV4 协议
     dest_addr.sin_port    =   htons(6666);  //端口号
     dest_addr.sin_addr.s_addr =  inet_addr("192.168.71.52");//自动绑定本地网卡地址

	while(1)
	{
	  //发送数据
		char  buf[100];
		scanf("%s",buf);
		int size=sendto(udp_socket_1,buf,5,0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
		printf("size =%d\n",size);
		
	}


}


int main()
{
	int i ;
	printf("选择需要的功能\n");
	printf("1.单播\n");
	printf("2.组播\n");
	printf("3.广播\n");
	scanf("%d",&i);
	switch (i)
	{
	case 1:
		{   pthread_t tid;
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
				printf("发送者< %s >端口< %d >大小< %d >数据 -> %s\n",inet_ntoa(send_addr.sin_addr),ntohs(send_addr.sin_port),size,buf);
			}	
			close(udp_socket);
		}
		break;
	case 2:
		{
				 //1.创建UDP socket  
			int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
				if(udp_socket < 0)
				{
					perror("");
					return 0;
				}
			
			//2.把UDP加入到组播中  
			struct  ip_mreq  mreq; //创建一个组播结构体
			bzero(&mreq, sizeof(mreq));

			mreq.imr_multiaddr.s_addr = inet_addr("226.81.9.8"); //设置一个组 
			mreq.imr_interface.s_addr = inet_addr("192.168.71.52") ; //把该IP地址加入到组播中
			int ret=setsockopt(udp_socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)); //开启组播功能
					if(ret < 0)
					{
						perror("");
						return 0;
					}
					else
					{
						printf("加入组播成\n");
					}
			
			
			//3.绑定UDP 的地址信息  
			struct sockaddr_in  server_addr;  //服务器地址信息
			server_addr.sin_family  =  AF_INET;	 //IPV4 协议
			server_addr.sin_port    =   htons(2425);  //端口号
			server_addr.sin_addr.s_addr =   INADDR_ANY;//自动绑定本地网卡地址
			ret  = bind(udp_socket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
				if(ret < 0)
				{
					perror("");
					return 0; 
				}
				else
				{
					printf("bind ok\n");
				}   
			
			//4.接收 单播或者是组播数据   （只有有人往组播地址 或者是本机地址发送数据都可以收到）
			while(1)
			{
				char  buf[1024]={0};
				int size = recvfrom(udp_socket,buf,1024,0,NULL,NULL);
				printf("size %d,  %s\n",size,buf);
			}
		}
		break;
	case 3:
		{
			//1.创建UDP socket  
			int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
				if(udp_socket < 0)
				{
					perror("");
					return 0;
				}
			
			//2.开启广播功能  
			int on=1;  
			int ret = setsockopt(udp_socket,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
				if(ret < 0)
				{
					perror("");
					return 0;
				}
			
			//设置广播地址  
			struct sockaddr_in  BROAD_addr;  //设置广播地址
			BROAD_addr.sin_family  =  AF_INET;	 //IPV4 协议
			BROAD_addr.sin_port    =   htons(6666);  //端口号
			BROAD_addr.sin_addr.s_addr =   inet_addr("192.168.71.255");//设置广播地址 
			
			//发送广播数据
			while(1)
			{
				char v[1024];
				scanf("%s",v);
				sendto(udp_socket,v,strlen(v),0,(struct sockaddr *)&BROAD_addr,sizeof(BROAD_addr));
			}
		}
		break;	
	default:
		break;
	}
}