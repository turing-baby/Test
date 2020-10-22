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


void *fuck(void * arg)
{
    int tcp_socket = *(int *)arg ;
    char buf[1024];
    while (1)
    {
        bzero(buf,1024);
        printf("向客户端发送的数据： \n");
        scanf("%s",buf);
        write(tcp_socket,buf, strlen (buf));
        printf("\n");
    }
    
}


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


    //客户端的信息结构体 
    struct sockaddr_in   clien={0};
    int len = sizeof(clien);


    int arr = accept(tcp_socket ,(struct sockaddr *)&clien,&len);
    if (arr  == -1)
    {
        perror("接收链接请求失败！\n");
        return 0;
    }
    printf("接收链接请求成功！\n");

    pthread_t TID ;
    pthread_create(&TID, NULL ,fuck, &arr);


    char buf[1024];
    while(1)
    {
        
        bzero(buf,1024);
        read(arr,buf,1024);
        if (strstr(buf, "kbgiueslgbijgnlsdkugnldfjngdnfdnkgnfdjnbkd") != 0)
        {
             char p[982];
             char  q[1024];
                for(int i = 0 ;i < 982 ;i++ )
                {
                    p[i] = buf[i+42];
                }
                sprintf(q , "./Download/%s" ,p );
                int new_fd = open(q, O_WRONLY |O_CREAT);
                if (new_fd < 0)
                {
                    perror("");
                    return -1;
                }
                char buff[1024] ;
                while(1)
                    {
                        //读取件数据
                        
                        bzero(buff,1024);
                       int  size = read(arr , buff , 1024);
                         //写入新文件
                        write(new_fd,buff,size);
                        if(size <= 0)
                        {
                            printf("文件接受完毕\n");
                            close(arr);
                            close(tcp_socket);
                            break;
                        }
                        
                       
                    } 
                    close(new_fd);
        }
        


        //把结构体中的信息取出来
        printf("\n");
        printf("----------------------\n");
        printf("接收到IP%s的数据：\n",inet_ntoa(clien.sin_addr));
        printf("----------------------\n");
        printf("端口号为:%d\n",ntohs(clien.sin_port));
        printf("----------------------\n");
        printf("->%s\n",buf);
        printf("----------------------\n");
        printf("\n");
        printf("向客户端发送的数据： \n");
    }
    
     close(arr);


    
    close(tcp_socket);

}