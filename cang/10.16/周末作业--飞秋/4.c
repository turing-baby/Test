#include <sys/types.h>          
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>


//信息结构体
struct massage
{
    char name[20];   //名字
    char phone[20];  //电话
    char ip[50];     //ip
};

//定义结构体数组保存好友人数
struct massage all[500];
//当前在线的人数
int people=1;


//广播发一条信息
int broadcast()
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
    int ret=setsockopt(udp_socket,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on)); //开启组播功能
    if(ret < 0)
    {
        perror("");
        return 0;
    }

    //3.设置udp的广播地址
    struct sockaddr_in BROAD_addr;
	BROAD_addr.sin_family = AF_INET;     //IPV4
	BROAD_addr.sin_port = htons(4444);    //端口号
	BROAD_addr.sin_addr.s_addr = inet_addr("192.168.71.255");//设置广播地址
   
    char buf1[50]={0};
    char buf2[50]={0};
    sprintf(buf1,"name:%s",all[0].name);
    sprintf(buf2,"phone:%s",all[0].phone);
    //4.发送广播数据
    sendto(udp_socket,buf1,sizeof(buf1),0,(struct sockaddr *)&BROAD_addr,sizeof(BROAD_addr));
    sendto(udp_socket,buf2,sizeof(buf2),0,(struct sockaddr *)&BROAD_addr,sizeof(BROAD_addr));

}


//给好友发送数据
void *Friend_list_chat(void *arg)
{

    while(1)
    {
        int a=0;
        if(1 != scanf("%d",&a))
{
	while(getchar() != '\n');

}
        if(a == 1)
        {
            
            printf("________________________________________________\n");
            printf("                 好 友 列 表          \n\n");
            for(int i=1;i<=people;i++)
            {
                if(strlen(all[i].ip)==0)
                {
                    break;
                }
                printf("%s   %s  ip:%s\n",all[i].name,all[i].phone,all[i].ip);
                // printf("people:%d\n",people);
                // sleep(2);
            }
            printf("________________________________________________\n");
        }
        if(a == 2)
        {
            int udp_socket=socket(AF_INET, SOCK_DGRAM, 0);
            if(udp_socket == -1)
            {
                printf("创建通信对象失败！\n");
                return 0;
            }
            printf("创建通信对象成功！\n");

            //2.绑定udp的地址信息
            struct sockaddr_in dest_addr;
            dest_addr.sin_family = AF_INET;     //IPV4
            dest_addr.sin_port = htons(4444);    //端口号
            dest_addr.sin_addr.s_addr = inet_addr("192.168.71.97");//ip地址自动获取

            // printf("请选择需要发送的好友\n");
            // int c=0;
            // scanf("%s",&c);
            printf("输入发送的数据\n");
            char buf[1024]={0};
            while(1)
            {
                scanf("%s",buf);
               if(strstr(buf,"exit"))
{break;}
                //发送数据
                sendto(udp_socket, buf, strlen(buf), 0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
            }
            
        }
    }
}


//接收上线信息
void Online_notice()
{
    int udp_socket=socket(AF_INET, SOCK_DGRAM, 0);
    if(udp_socket == -1)
    {
        printf("创建通信对象失败！\n");
        return ;
    }
    printf("创建通信对象成功！\n");

    //2.绑定udp的地址信息
    struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;     //IPV4
	dest_addr.sin_port = htons(2222);    //端口号
	dest_addr.sin_addr.s_addr = INADDR_ANY;//ip地址
    int ret_bind=bind(udp_socket, (struct sockaddr*)&dest_addr,sizeof(struct sockaddr));
    if(ret_bind == -1)
    {
        printf("绑定udp的地址信息失败!\n");
        return ;
    }
    printf("绑定udp的地址信息成功!\n");

    // 设备在运行的时候可能要求数据的写入是连续的,I/O通道进行管理
    int on = 1;
	ioctl(udp_socket,FIONBIO,&on);

    printf("请选择：1.显示好友列表    2.给好友发送数据\n");
	
    while(1)
    {
        sleep(1);
        //客户端的信息结构体
        struct sockaddr_in clien={0};
        int len = sizeof(clien);

        char buf[1024]={0};
        int size=recvfrom(udp_socket,buf,1024,0,(struct sockaddr *)&clien, &len);
        if(size <=0 )
        {
            // printf("暂时没有收到信息！\n");
            continue;
        }
       if(!strstr(buf,"name")&&!strstr(buf,"phone"))
        {
            printf("内容：%s\n",buf);
        }

        for(int i=1;i<=people;i++)
        {
            if(!strcmp(all[i].ip,inet_ntoa(clien.sin_addr)))//相同返回0
            {
                // printf("ip不同:all[i].ip:%s   inet_ntoa(clien.sin_addr):%s\n",all[i].ip,inet_ntoa(clien.sin_addr));
                // printf("ip相同\n");
                break;
            }
            else
            {
                // printf("ip不同:all[i].ip:%s   inet_ntoa(clien.sin_addr):%s\n",all[i].ip,inet_ntoa(clien.sin_addr));
                if(strstr(buf,"name"))
                {
                    printf("\n---------------------------\n");
                    printf("你的好友已上线！\n");
                    printf("%s\n",buf);

                    //保存好友的信息
                    strcpy(all[people].name,buf);//name
                }

                if(strstr(buf,"phone"))
                {
                    printf("%s\n",buf);
                    printf("ip:%s\n",inet_ntoa(clien.sin_addr));//ip
                    printf("---------------------------\n");

                    //保存好友的信息
                    strcpy(all[people].phone,buf);//phone
                    strcpy(all[people].ip,inet_ntoa(clien.sin_addr));//ip

                    char buf1[50]={0};
                    char buf2[50]={0};
                    sprintf(buf1,"name:%s",all[0].name);
                    sprintf(buf2,"phone:%s",all[0].phone);
                    // printf("%s ++++  %s  \n",buf1,buf2);
                    // printf("ip:%s\n",inet_ntoa(clien.sin_addr));//ip
                    //原路返回
                     sendto(udp_socket, buf1, sizeof(buf1), 0,(struct sockaddr *)&clien,len);
                     sendto(udp_socket, buf2, sizeof(buf2), 0,(struct sockaddr *)&clien,len);
                   // broadcast();
                    // sleep(1);
                    // printf("people:%d\n",people);
                    // printf("%s   %s  ip:%s\n",all[1].name,all[1].phone,all[1].ip);

                    

                    people++;//好友数
                }
                break;
            }
            
        }

        // if(strstr(buf,"name"))
        // {
        //     printf("\n---------------------------\n");
        //     printf("你的好友已上线！\n");
        //     printf("%s\n",buf);

        //     //保存好友的信息
        //     strcpy(all[people].name,buf);//name
        // }

        // if(strstr(buf,"phone"))
        // {
        //     printf("%s\n",buf);
        //     printf("ip:%s\n",inet_ntoa(clien.sin_addr));//ip
        //     printf("---------------------------\n");

        //     //保存好友的信息
        //     strcpy(all[people].phone,buf);//phone
        //     strcpy(all[people].ip,inet_ntoa(clien.sin_addr));//ip

        //     char buf1[50]={0};
        //     char buf2[50]={0};
        //     sprintf(buf1,"name:%s",all[0].name);
        //     sprintf(buf2,"phone:%s",all[0].phone);
        //     printf("%s ++++  %s  \n",buf1,buf2);
        //     printf("ip:%s\n",inet_ntoa(clien.sin_addr));//ip
        //     //原路返回
        //     // sendto(udp_socket, buf1, sizeof(buf1), 0,(struct sockaddr *)&clien,len);
        //     // sendto(udp_socket, buf2, sizeof(buf2), 0,(struct sockaddr *)&clien,len);
        //     broadcast();
        //     // sleep(1);
        //     // printf("people:%d\n",people);
        //     // printf("%s   %s  ip:%s\n",all[1].name,all[1].phone,all[1].ip);

            

        //     people++;//好友数
        //}

        // //保存好友的信息
        // strcpy(all[people].ip,inet_ntoa(clien.sin_addr));//ip
        // all[people].port = ntohs(clien.sin_port);//端口
        // all[people].clien_sock = new_socket;//通信socket

        //创建一个线程处理该客户的数据,读取客户端发送过来的数据
        // pthread_t tid1;                      //把客户端的信息传递过去
        // pthread_create(&tid1,NULL,Friend_list_chat,NULL);//&all[people]
        // pthread_detach(tid1);//设置分离属性，防止变成僵尸线程

        // sleep(1);
        // printf("people:%d\n",people);
        // printf("%s   %s  ip:%s\n",all[1].name,all[1].phone,all[1].ip);

        // people++;//好友数
    }
    close(udp_socket);
    return ;
}


int main(int argc,char *argv[])
{
                              //argv[1]  argv[2]
    //输入命令格式   ./main    name     phone
    if(argc != 3)
    {
        printf("请输入 ./main name phone\n");
        return 0;
    }
    
    //登录者信息
    strcpy(all[0].name,argv[1]);
    strcpy(all[0].phone,argv[2]);

    //广播发一条信息
    broadcast();
    sleep(1);
    
    // pthread_t tid1;                      
    // pthread_create(&tid1,NULL,Online_notice,NULL);

    pthread_t tid1;                   
    pthread_create(&tid1,NULL,Friend_list_chat,NULL);
    // pthread_detach(tid1);//设置分离属性，防止变成僵尸线程

    //接收上线信息
    Online_notice();

    

}