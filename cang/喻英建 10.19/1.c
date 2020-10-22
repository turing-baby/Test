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


int main()
{
    //1.创建TCP socket  
       
	int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addr.sin_addr.s_addr = inet_addr("112.67.243.36"); //服务器的IP地址
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
	char  *http_head = "GET  /WeatherApi?city=广州 HTTP/1.1\r\nHost:http://wthrcdn.etouch.cn/\r\n\r\n";
    //发HTTTP 请求协议  
	write(tcp_socket,http_head,strlen(http_head));
    char buf[4096*3];
    int size=read(tcp_socket,buf,4096);
    printf("buf=%s\n",buf);
    close(tcp_socket);
}