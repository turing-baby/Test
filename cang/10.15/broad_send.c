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
		 
       sendto(udp_socket,"Help me",7,0,(struct sockaddr *)&BROAD_addr,sizeof(BROAD_addr));
	   sleep(1);
	   
	 }
	 
	 //练习：自己尝试发送广播数据
}