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



//发送数据给组播 
void *write_clien(void *arg)
{
    int tcp_socket  = *(int *)arg ;

    char buf[1024];
    while (1)
    {
        bzero(buf,1024);
        printf("向组播中发送数据\n");
        scanf("%s",buf);
        sendto(tcp_socket,buf,sizeof(buf),0,NULL,NULL);
    }
    
}

int main()
{
    int  tcp_socket  = socket(AF_INET , SOCK_DGRAM , 0);
    if (tcp_socket <  0)
    {
        perror("创建失败！\n");
        return 0;
    }
    else 
    {
        printf("创建成功！\n");

    }


    struct ip_mreq  mreq ;
    bzero(&mreq,sizeof(mreq));


    mreq. imr_multiaddr.s_addr = inet_addr("226.81.9.9");
    mreq. imr_interface.s_addr = inet_addr("192.168.71.86");
    int ret = setsockopt(tcp_socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
    if (ret < 0)
    {
        perror("加入组播失败！\n");
        return 0;
    }
    else
    {
        printf("加入组播成功！\n");
    }
    
    
    
    struct sockaddr_in  server_adddr ;
    server_adddr.sin_family  =  AF_INET ;
    server_adddr.sin_port    =  htons(6666);
    server_adddr.sin_addr.s_addr  = INADDR_ANY;
    ret = bind(tcp_socket,(struct sockaddr *)&server_adddr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("绑定失败！\n");
        return 0;
    }
    else
    {
        printf("绑定成功！\n");
    }



  //创建一个线程给客户端发送数据  
   pthread_t tid;
   pthread_create(&tid,NULL,write_clien,&tcp_socket);


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
        printf("向组播中发送数据\n");
    }
    


}