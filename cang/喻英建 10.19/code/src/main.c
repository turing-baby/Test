#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "lcd.h"


int main(int argc,char **argv)
{
	/* 打开LCD设备 */
	lcd_open();
	//1.创建TCP socket  
       
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addr.sin_addr.s_addr = inet_addr("113.113.67.48"); //服务器的IP地址
											//图片的地址 

  // 3.链接 
  int ret = connect(tcp_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	  if(ret < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }
	//https://i04piccdn.sogoucdn.com /c4503cb41c4f1043
			//i04piccdn.sogoucdn.com /2e816d38495f796e
			
			//http://www.boa.org/boa-0.94.13.tar.gz  
			//https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1603113386416&di=4df16fe7400b6ea6603cfb90115055ca&imgtype=0&src=http%3A%2F%2Fpic1.win4000.com%2Fwallpaper%2F2019-08-26%2F5d63a0b7350c7.jpg
	//制定 HTTP 请求协议   重点！！！ 
	char  *http_head = "GET /timg?image&quality=80&size=b9999_10000&sec=1603113386416&di=4df16fe7400b6ea6603cfb90115055ca&imgtype=0&src=http%3A%2F%2Fpic1.win4000.com%2Fwallpaper%2F2019-08-26%2F5d63a0b7350c7.jpg HTTP/1.1\r\nHost:timgsa.baidu.com\r\n\r\n";
														

	//发HTTTP 请求协议  
	write(tcp_socket,http_head,strlen(http_head));
	
	
	//新建一个文件  


       int fd=open("1.jpg",O_RDWR|O_CREAT|O_TRUNC,0777);
			if(fd < 0)
			{
				perror("");
				return -1;
			}
	
	//读取头数据  
	char  head[1024]={0}; 
	int size1 = read(tcp_socket,head,1024);  
		printf("size1 = %d\n",size1);
	//取出文件的大小 
	int file_size=0;
	//下载的大小 
	int load_size=0;
	
	char *p =  strstr(head,"Content-Length");  //查找关键字
	
	sscanf(p,"Content-Length:%d\r\n",&file_size);
	
	printf("file_size=%d\n",file_size);
	
	
	//求出头数据的长度 
	p = strstr(head,"\r\n\r\n");   //回文末尾  
	p = p+4; 
	
  int head_len = (int)(p - head);
      printf("head_len = %d\n",head_len);
	
	//写入去头的一次数据 
	  int len  =size1-head_len; 
	  write(fd,p,len);
	  load_size =+  len;
	
	
	while(1)
	{
	//回收HTTP 服务器的消息
	char  buf[4096]={0};
	int size=read(tcp_socket,buf,4096);
		load_size += size;
		printf("file_size=%d load_size=%d\n",file_size,load_size);
		if(file_size == load_size)
		{
			printf("文件下载完毕\n");
			write(fd,buf,size);
			
			break;
		}
	     //把数据写入到本地文件中 
		   write(fd,buf,size);
	
		//printf("buf=%s\n",buf);
	}

	
	/* 在x=0,y=0,显示图片 */
	lcd_draw_jpg(0,0,"1.jpg");


	//练习： 实现文件下载图片，文件中的回文头数据给删除掉！！！ 
	close(fd); 
	close(tcp_socket);
	
	
	
	/* 关闭LCD设备 */
	lcd_close();
	
	return 0;	
}

