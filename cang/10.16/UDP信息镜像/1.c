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
#include <sys/ioctl.h>
struct add
{
    char  buff[1024];
    struct sockaddr_in  qq;

};

//发送数据
void *write_clien(void *arg)
{
    
    struct add arr = *(struct add *)arg;
   //1.创建UDP socket  
	   int udp_socket_1 = socket(AF_INET, SOCK_DGRAM, 0);
		  if(udp_socket_1 < 0)
		  {
			  perror("");
			  return 0;
		  }
	
	  
	   //2.设置目标的IP地址信息       
	 struct sockaddr_in  dest_addr; 
     dest_addr.sin_family  =  AF_INET;	//IPV4 协议
     dest_addr.sin_port    =   htons(ntohs(arr.qq.sin_port)); //端口号
     dest_addr.sin_addr.s_addr =  inet_addr(inet_ntoa(arr.qq.sin_addr));
     char  buf[1024];
       strcpy(buf,arr.buff) ;
	  //发送数据
		
  
        printf("%s\n",buf);
		int size=sendto(udp_socket_1,buf,strlen(buf),0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
		printf("端口 =%d\n",ntohs(arr.qq.sin_port));
		printf("size =%d\n",size);
	
    close(udp_socket_1);
    pthread_exit(0);
}



int main(int argc ,char * argv[])
{ 
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
	 int on = 1;
	 ioctl(udp_socket,FIONBIO,&on);
	 while(1)
	 {

		 //3.接收数据
		char  buf[1024]={0}; printf("1\n");
		struct sockaddr_in  send_addr;printf("2\n");
		int len  = sizeof(send_addr);printf("3\n");
		int size = recvfrom(udp_socket,buf,1024,0,(struct sockaddr *)&send_addr,&len);printf("4\n");
        struct add arr;printf("5\n");
        arr.qq.sin_addr = send_addr.sin_addr;printf("6\n");
        arr.qq.sin_port = send_addr.sin_port;printf("7\n");
        strcpy(arr.buff,buf) ;printf("8\n");
        printf("发送者 %s 端口 %d  size %d 数据 %s\n",inet_ntoa(send_addr.sin_addr),ntohs(send_addr.sin_port),size,buf);printf("9\n");
        pthread_t tid;printf("10\n");
        pthread_create(&tid,NULL,write_clien,&arr);printf("11\n");
        void * o ;printf("12\n");
        pthread_join(tid,&o);printf("13\n");
	 }
    
    close(udp_socket);

}