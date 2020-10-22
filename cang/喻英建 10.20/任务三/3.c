#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
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
#include <stdlib.h>


//客户端信息结构体 
struct  clien
{
	char ip[50];  
	int  port;  
	int  clien_socket;
};  


//定义结构体数组保存客户端的链接人数  
struct  clien   all[50]; 

int  people=0; //当前在线的人数 


int main(int argc,char *argv[])
{
    //1.创建服务器通信对象 
   int tcp_socket = socket(AF_INET, SOCK_STREAM, 0); //创建TCP 通信 socket 
	   if(tcp_socket < 0)
	   {
		   perror("");
		   return -1; 
	   }
	   else
	   {
		    printf("创建服务器socket成功\n");
	   }
	 
	 
	//2.绑定服务器通信socket     
	 struct sockaddr_in  server_addr;  //服务器地址信息
     server_addr.sin_family  =  AF_INET;	 //IPV4 协议
     server_addr.sin_port    =   htons(atoi(argv[1]));  //端口号
     server_addr.sin_addr.s_addr =   INADDR_ANY;//自动绑定本地网卡地址
    int ret  = bind(tcp_socket,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
    	if(ret < 0)
    	{
    	    perror("");
    	    return 0; 
    	}
    	else
    	{
    	    printf("bind ok\n");
    	}   
	
	//3.设置服务器为监听模式 
	ret = listen(tcp_socket,5);
		if(ret < 0)
    	{
    	    perror("");
    	    return 0; 
    	}
    	else
    	{
    	    printf("listen ok\n");
    	} 


	while(1)
	{	
		//客户端的信息结构体 
		 struct sockaddr_in   clien={0};
		 int len = sizeof(clien);
			

        //清空集合  
        fd_set fdset;
        FD_ZERO(&fdset);
        
        //1.把标准文件描述符加入到集合中   （抓犯人）
        FD_SET(tcp_socket,&fdset);
        FD_SET(0,&fdset);


        //2.让 select 去监听这个集合    （管理犯人）
	    int ret=select(tcp_socket+1+people,&fdset,NULL,NULL,NULL);
		 if(ret < 0)
		 {
			perror("");
			return 0; 
		 }
		 else if(ret == 0)
		 {
			printf("超时\n");
		 }
		 else if(ret > 0)
		{
			if(FD_ISSET(0,&fdset))
			{
				printf("输入设备：\n");
				char aee[1024]={0};
				read(0,aee,1024);
				printf("输入的数据： %s\n",aee);
                if (strcmp(aee , "1"))
                {
                    printf("_____________________\n");
                    for(int i=0;i<people;i++)
                    {
                        printf("ip:%s,prot:%d,socket:%d\n",all[i].ip,all[i].port,all[i].clien_socket);
                    }
                    
                    printf("_____________________\n");
                }
                if(aee == "2")
                {
                    FD_CLR(0,&fdset); 
                    printf("请选择需要发送的客户端\n");
                    int c=0;
                    scanf("%d",&c);
                    
                    printf("输入发送的数据\n");
                    char buf[1024]={0}; 
                    scanf("%s",buf);
                    //发送
                    write(all[c].clien_socket,buf,strlen(buf));
                    FD_SET(0,&fdset);
                }
                else
                {
                    printf("输入错误请重新输入：\n");
                }
                
                

			}
			if(FD_ISSET(tcp_socket,&fdset))
			{
				int new_socket=accept(tcp_socket,(struct sockaddr *)&clien,&len);   
                if(new_socket < 0)
                {
                    perror("");
                    return 0;
                }
                else
                {
                    printf("有客户端链接进来  %d\n",new_socket);
                    //把结构体中的信息取出来
                    printf("ip:%s\n",inet_ntoa(clien.sin_addr));
                    printf("port:%d\n",ntohs(clien.sin_port));
                    printf("____________________________________\n");
                    
                    //保存客户端的信息  
                    strcpy(all[people].ip,inet_ntoa(clien.sin_addr));  
                    all[people].port = ntohs(clien.sin_port);  
                    all[people].clien_socket = new_socket;

                    FD_SET(new_socket,&fdset); //将新的用于通信的描述符加入监听
                    people++;  

                    printf("1.显示所有客户端信息  2.给客户端发送数据\n");
		        }
            }
            else 
            {

                for (int  y = 0; y < people; y++)
                {
                    char  buf[1024]={0};
                    int size =read(all[y].clien_socket,buf,1024);
                    if(size > 0)
                    { 
                        printf("客户 %s 收到 %s\n",all[y].ip,buf);
                       
                    }

                    printf("1.显示所有客户端信息  2.给客户端发送数据\n");
                 
                }
                
               
            }
            

        }
			
		
	   
	}
	
   
}