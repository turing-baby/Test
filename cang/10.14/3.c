#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include <pthread.h>


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
    server_adddr.sin_port    =  htons(2005);
    server_adddr.sin_addr.s_addr  =  INADDR_ANY;
    int ret = bind(tcp_socket,(struct sockaddr *)&server_adddr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("绑定失败！\n");
        return 0;
    }
    printf("绑定成功！\n");

    ret = listen(tcp_socket , 5);
   if (ret < 0)
    {
        perror("设置监听失败！\n");
        return 0;
    }
    printf("设置监听成功！\n");
    int arr;
    //客户端的信息结构体 
    struct sockaddr_in   clien={0};
    int len = sizeof(clien);

    while(1)
    {
        arr = accept(tcp_socket ,(struct sockaddr *)&clien,&len);
        if (arr  == -1)
        {
            perror("接收链接请求失败！\n");
            return 0;
        }
        printf("接收链接请求成功！\n");
        char buf[1024];
        read(arr,buf,1024);
        //把结构体中的信息取出来
        printf("____________________________________\n");
        printf("接收到IP%s的数据：\n",inet_ntoa(clien.sin_addr));
        printf("____________________________________\n");
        printf("端口号为:%d\n",ntohs(clien.sin_port));
        printf("____________________________________\n");
        printf("->%s\n",buf);
        printf("____________________________________\n");

        
    }
    
     close(arr);


    
    close(tcp_socket);

}