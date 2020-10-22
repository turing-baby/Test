#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
int main()
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
	
	
	//练习：尝试往组中 发送数据 ，看看刚刚加入组中的成员是否能接收到数据
	
}