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
        read(tcp_socket,buf,1024);
        printf("\n");
        printf("----------------------\n");
        printf("接收到服务器消息 -> %s\n",buf);
        printf("----------------------\n");
        printf("\n");
        printf("向服务器发送的数据： \n");
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
    server_adddr.sin_addr.s_addr  =  inet_addr("192.168.71.86");
    int ret = connect(tcp_socket,(struct sockaddr *)&server_adddr,sizeof(struct sockaddr));
    if (ret < 0)
    {
        perror("链接失败！\n");
        return 0;
    }
    printf("连接成功！\n");
    
    pthread_t TID ;
    pthread_create(&TID, NULL ,fuck, &tcp_socket );



    while (1)
    {
        char add[1024]; 
        char aee[1024];
        int a ;
        bzero(add,1024);
        printf("1.向服务器发送数据\n");
        printf("2.向服务器发送文件\n");
        scanf("%d",&a);
        printf("\n");
        switch(a)
            {
                case 1: 
                {
                    printf("\n");   
                    printf("向服务器发送的数据： \n");
                    scanf("%s",add);
                    write(tcp_socket,add, strlen (add));
                }
                break;

                case 2:
                {
                    printf("请输入要传输的文件名：\n");
	                scanf("%s",aee);
                    strcpy(add,"kbgiueslgbijgnlsdkugnldfjngdnfdnkgnfdjnbkd");
                    strcat(add,aee); 
                    write(tcp_socket,add, strlen (add));
                    //打开目标文件 主函数传参  打开需要传输的文件
                    int fd = open( aee , O_RDONLY);
                    if (fd <0 )
                    {
                        perror("");
                        return -1;
                    }
                    char buff[1024] ;
                    while(1)
                        {
                            bzero(buff,1024);
                            //读取目标文件
                            
                            int size = read(fd,buff,sizeof(buff));
                           //写入目标文件
                            write(tcp_socket,buff,size);
                            if(size <= 0)
                            {
                                close(fd);
                                printf("文件发送完毕\n");
                                break;
                            }
                            
                        }
                        
                }
                break;


                default :printf("输入错误请从新选择！\n");break;
            }


    }
    

    close(tcp_socket);

}