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

int main(int argc ,char * argv[])
{
    int  tcp_socket  = socket(AF_INET , SOCK_DGRAM , 0);
    if (tcp_socket == -1)
    {
        perror("创建失败！\n");
        return 0;
    }
    else 
    {
        printf("创建成功！\n");

    }
    

    struct sockaddr_in  server_adddr ;
    server_adddr.sin_family  =  AF_INET ;
    server_adddr.sin_port    =  htons(2006);
    server_adddr.sin_addr.s_addr  =  inet_addr("192.168.71.86");
    int ret = bind(tcp_socket,(struct sockaddr *)&server_adddr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("绑定失败！\n");
        return 0;
    }
    else
    {
        printf("绑定成功！\n");
    }
    
    int on=1; //1就是开启的意思

	 //   setsockopt()函数
     setsockopt(tcp_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); //开启地址服用
	 
   

    while (1)
    {
         //客户端的信息结构体 
        struct sockaddr_in   clien={0};
        int len = sizeof(clien);

        char buf[1024]={0};

        int size = recvfrom(tcp_socket,buf,1024,0, (struct sockaddr *)&clien, &len);
         //把结构体中的信息取出来
        printf("\n");
        printf("----------------------\n");
        printf("I  P->%s：\n",inet_ntoa(clien.sin_addr));
       
        printf("端口->%d\n",ntohs(clien.sin_port));
        
        printf("消息->%s\n",buf);
        printf("----------------------\n");
        printf("\n");
    }
    
    
    close(tcp_socket);


}