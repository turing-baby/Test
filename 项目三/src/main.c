#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <linux/input.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include "lcd.h"
#include "font.h"
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/select.h>
#include  <cJSON.h>
#include<stdbool.h>

int ziku_tianqi();
int ziku();


int kk = 1 ;

struct app
{
	char t1[1024]; //日期
	char t2[1024]; //天气
	char t3[1024]; //星期几
	char t4[1024];  //高温
	char t5[1024];	//低温

};

struct aff
{
	struct app  kk[5]; //未来五天天气
	char cc1[1024]; // 城市
	char cc2[1024]; //时间

}ops;




//可以保存100个客户端
int  clien_arry[100]={0};  

int  people=0; //当前在线的人数 



int shuju(int p_1)
{
	printf("这是数据处理！\n");

	//1.打开文件获取json 数据 
	FILE *fp  = fopen("./2.txt","r+");
		 if(fp == NULL)
		 {
			 perror("");
			 return 0;
		 }
	 
	//读取文本中的数据 
	 char  json_buf[1024000]={0};
	 fread(json_buf,1,p_1,fp);
	 fclose(fp);  
	 char *p=strstr(json_buf,"{\"message\"");
	 
	//1.判断是否为 json 数据  
	  cJSON *obj=cJSON_Parse(p);
			if(obj == NULL)
			{
				char lkk[1024]="哦，天啊，非常抱歉 ，查询失败，请您稍后再次查询！";
				ziku(lkk);
				printf("不是JSO数据\n"); 
				return 0;
			}
	
		//取出城市
		cJSON *value=cJSON_GetObjectItem(obj,"cityInfo");
		cJSON *value1=cJSON_GetObjectItem(value,"city");
		printf("城市 ： %s \n",cJSON_Print(value1));
		strcpy(ops.cc1 , strtok(cJSON_Print(value1),"\""));
		cJSON *value2=cJSON_GetObjectItem(obj,"time");
		printf("时间 ： %s \n",cJSON_Print(value2));
		strcpy(ops.cc2 , strtok(cJSON_Print(value2),"\""));
		//获取数组
		cJSON *arry=cJSON_GetObjectItem(obj,"data");
		cJSON *arry1=cJSON_GetObjectItem(arry,"forecast");

		//cJSON *arry=cJSON_GetObjectItem(arry2,"forecast");
		if(arry == NULL)
		{
			printf("数组取出失败\n");
			return 0;
		}
	
	    //求出数组的大小 
		int size=cJSON_GetArraySize(arry1);
		//printf("当前数组的大小 %d\n",size);
		//遍历数组
		for(int i=0;i<5;i++)
		{
			//通过数组的下标获取对象  
			cJSON *new_obj=cJSON_GetArrayItem(arry1,i);
			if(new_obj == NULL)
			{
				printf("获取对象失败\n");
				return 0;
			}
			//处理每一项中的数据 
			cJSON *value0=cJSON_GetObjectItem(new_obj,"ymd"); //日期
			cJSON *value1=cJSON_GetObjectItem(new_obj,"type");//天气
			cJSON *value2=cJSON_GetObjectItem(new_obj,"week");//星期几
			cJSON *value3=cJSON_GetObjectItem(new_obj,"low");//低温
			cJSON *value4=cJSON_GetObjectItem(new_obj,"high");//高温
			printf("日期 ： %s \n",cJSON_Print(value0));
			strcpy(ops.kk[i].t1 ,strtok(cJSON_Print(value0),"\"") );
			printf("天气 ： %s \n",cJSON_Print(value1));
			strcpy(ops.kk[i].t2 ,strtok(cJSON_Print(value1),"\""));
			printf("    %s \n",cJSON_Print(value2));
			strcpy(ops.kk[i].t3 ,strtok(cJSON_Print(value2),"\""));

			printf("    %s \n",cJSON_Print(value3));
			strcpy(ops.kk[i].t4 ,strtok(cJSON_Print(value3),"\"") );
			printf("    %s \n",cJSON_Print(value4));
			strcpy(ops.kk[i].t5 ,strtok(cJSON_Print(value4),"\"") );
			printf(" \n");			
		}
	
	ziku_tianqi();

}







  


//初始化Lcd
struct LcdDevice *init_lcd(const char *device)
{
	//申请空间
	struct LcdDevice* lcd = malloc(sizeof(struct LcdDevice));
	if(lcd == NULL)
	{
		return NULL;
	} 

	//1打开设备
	lcd->fd = open(device, O_RDWR);
	if(lcd->fd < 0)
	{
		perror("open lcd fail");
		free(lcd);
		return NULL;
	}
	
	//映射
	lcd->mp = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd->fd,0);

	return lcd;
}


void * TS_THREAD(void * arg )
{
		/* 在x=0,y=0,显示图片 */
	char buf[1024] = {0};
	/* 打开LCD设备 */
		lcd_open();
	for( int i = 1 ; i < 46 ;i++)
	{	
		if (i == 45)
		{
			i = 1 ;
		}

		if( kk == 1)
		{
			sprintf(buf ,"./yasuotu/%d.jpg" , i);
			lcd_draw_jpg(0,0,buf);
		}
		
		
		sleep(0.4);
		
	}	
	/* 关闭LCD设备 */
		lcd_close();
}







int ziku(char buf[1024])
{
	
    //初始化Lcd
	struct LcdDevice* lcd = init_lcd("/dev/fb0");
			
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	  
	//字体大小的设置
	fontSetSize(f,24);
	
	
	//创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(800,100,4,getColor(0,49,31,14)); //也可使用createBitmapWithInit函数，改变画板颜色
	//bitmap *bm = createBitmap(288, 100, 4);
	
	
	//将字体写到点阵图上
	fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	
	

	//把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp,0,380,bm);

	
	//关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);
	
}



int ziku_tianqi()
{
	printf("%d",kk);
	kk = 0 ;
	printf("%d",kk);
	printf("这里是显示天气\n");
    //初始化Lcd
	struct LcdDevice* lcd = init_lcd("/dev/fb0");
		
	//打开字体	
	font *f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	 
	//字体大小的设置
	fontSetSize(f,24);
	
	
	
	//创建一个画板（点阵图）
	bitmap *bm = createBitmapWithInit(800,480,4,getColor(0,49,31,14)); //也可使用createBitmapWithInit函数，改变画板颜色
	//bitmap *bm = createBitmap(288, 100, 4);

	//将字体写到点阵图上



	//显示时间
	fontPrint(f,bm,30,70,ops.cc2,getColor(0,255,255,255),0);
	//显示星期几
	fontPrint(f,bm,80,120,ops.kk[0].t3,getColor(0,255,255,255),0);
	fontPrint(f,bm,220,120,ops.kk[1].t3,getColor(0,255,255,255),0);
	fontPrint(f,bm,360,120,ops.kk[2].t3,getColor(0,255,255,255),0);
	fontPrint(f,bm,500,120,ops.kk[3].t3,getColor(0,255,255,255),0);
	fontPrint(f,bm,640,120,ops.kk[4].t3,getColor(0,255,255,255),0);

	//显示日期
	fontPrint(f,bm,60,150,ops.kk[0].t1,getColor(0,255,255,255),0);
	fontPrint(f,bm,200,150,ops.kk[1].t1,getColor(0,255,255,255),0);
	fontPrint(f,bm,340,150,ops.kk[2].t1,getColor(0,255,255,255),0);
	fontPrint(f,bm,480,150,ops.kk[3].t1,getColor(0,255,255,255),0);
	fontPrint(f,bm,620,150,ops.kk[4].t1,getColor(0,255,255,255),0);
	
	//显示天气
	fontPrint(f,bm,100,320,ops.kk[0].t2,getColor(0,255,255,255),0);
	fontPrint(f,bm,240,320,ops.kk[1].t2,getColor(0,255,255,255),0);
	fontPrint(f,bm,380,320,ops.kk[2].t2,getColor(0,255,255,255),0);
	fontPrint(f,bm,520,320,ops.kk[3].t2,getColor(0,255,255,255),0);
	fontPrint(f,bm,660,320,ops.kk[4].t2,getColor(0,255,255,255),0);
	
	
	//显示温度
	fontPrint(f,bm,70,370,ops.kk[0].t4,getColor(0,255,255,255),0);
	fontPrint(f,bm,210,370,ops.kk[1].t4,getColor(0,255,255,255),0);
	fontPrint(f,bm,350,370,ops.kk[2].t4,getColor(0,255,255,255),0);
	fontPrint(f,bm,490,370,ops.kk[3].t4,getColor(0,255,255,255),0);
	fontPrint(f,bm,630,370,ops.kk[4].t4,getColor(0,255,255,255),0);

	fontPrint(f,bm,70,420,ops.kk[0].t5,getColor(0,255,255,255),0);
	fontPrint(f,bm,210,420,ops.kk[1].t5,getColor(0,255,255,255),0);
	fontPrint(f,bm,350,420,ops.kk[2].t5,getColor(0,255,255,255),0);
	fontPrint(f,bm,490,420,ops.kk[3].t5,getColor(0,255,255,255),0);
	fontPrint(f,bm,630,420,ops.kk[4].t5,getColor(0,255,255,255),0);




	//显示城市
	fontSetSize(f,30);
	fontPrint(f,bm,80,30,ops.cc1,getColor(0,255,255,255),0);
	printf("%s",ops.cc1);
	
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);
	//fontPrint(f,bm,50,20,buf,getColor(0,255,255,255),0);

	
	

	//把字体框输出到LCD屏幕上
	show_font_to_lcd(lcd->mp,0,0,bm);
	//关闭字体，关闭画板
	fontUnload(f);
	destroyBitmap(bm);


	int uio =  0 ;int iop = 0 ;
	for(iop ; iop < 5 ; iop ++)
	{
		printf("这里是打印图片 %d\n", iop);
		if(! strcmp( ops.kk[iop].t2  , "晴" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/qing.jpg");
			/* 关闭LCD设备 */
		//	lcd_close();

		}
		if(! strcmp( ops.kk[iop].t2  , "阴" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/yin.jpg");
			/* 关闭LCD设备 */
			//lcd_close();
		}
		if(! strcmp( ops.kk[iop].t2  , "多云" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/duoyun.jpg");
			/* 关闭LCD设备 */
			//lcd_close();
		}
		if(! strcmp( ops.kk[iop].t2  , "小雨" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/yu.jpg");
			/* 关闭LCD设备 */
			//lcd_close();
		}
		if(! strcmp( ops.kk[iop].t2  , "暴雨" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/baoyu.jpg");
			/* 关闭LCD设备 */
			//lcd_close();
		}
		if(! strcmp( ops.kk[iop].t2  , "阵雨" ))
		{
			/* 打开LCD设备 */
			//lcd_open();
			/* 在x=0,y=0,显示图片 */
			lcd_draw_jpg(60+uio,195,"./weather/zhenyu.jpg");
			/* 关闭LCD设备 */
			//lcd_close();
		}

		uio=uio+140 ;

	}

	sleep(10);
	kk = 1 ;
	char kku[1024] = "看清楚了吗？？ 哈哈哈";
	ziku(kku);
}




int tcp_server(char buf[1024])
{
	//1.创建TCP socket    
	int socket_1= socket(AF_INET, SOCK_STREAM, 0);
	printf("客户端的数据 :%s\n",buf);
    // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addre;
    server_addre.sin_family = AF_INET;
    server_addre.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addre.sin_addr.s_addr = inet_addr("47.107.155.132"); //服务器的IP地址
											//图片的地址 

  // 3.链接 
  int ret = connect(socket_1,(struct sockaddr *)&server_addre,sizeof(server_addre));
	  if(ret < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }

	
	//制定 HTTP 请求协议   重点！！！ 
	char  http_head[1024*3];
	sprintf(http_head ,"GET /api.php?key=free&appid=0&msg=%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n",buf);
	printf("%s",buf);												

	//发HTTTP 请求协议  
	write(socket_1,http_head,strlen(http_head));

	char  head[1024]={0}; 
	int size1 = read(socket_1,head,1024);
	printf("size1 = %d\n",size1);
	printf("size1 = %s\n",head);		

	char *p=strstr(head,"{");
	
	 
	//1.判断是否为 json 数据  
	  cJSON *obj=cJSON_Parse(p);
			if(obj == NULL)
			{
				char att[1024]="输入错了！！";
				ziku(att);
				printf("不是JSO数据\n");close(socket_1);
				return 0;
			}
			
	cJSON *value=cJSON_GetObjectItem(obj,"content");
	close(socket_1);
	strcpy(head ,strtok(cJSON_Print(value),"\""));
	ziku(head);
	

}




int tcp_server_1(char buf[1024])
{
	//1.创建TCP socket    
	int socket_1= socket(AF_INET, SOCK_STREAM, 0);
	printf("客户端的数据 :%s\n",buf);
    // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addre;
    server_addre.sin_family = AF_INET;
    server_addre.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addre.sin_addr.s_addr = inet_addr("47.107.155.132"); //服务器的IP地址
											//图片的地址 

  // 3.链接 
  int ret = connect(socket_1,(struct sockaddr *)&server_addre,sizeof(server_addre));
	  if(ret < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }

	
	//制定 HTTP 请求协议   重点！！！ 
	char  http_head[1024*3];
	sprintf(http_head ,"GET /api.php?key=free&appid=0&msg=翻译%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n",buf);
	printf("%s",buf);												

	//发HTTTP 请求协议  
	write(socket_1,http_head,strlen(http_head));

	char  head[1024]={0}; 
	int size1 = read(socket_1,head,1024);
	printf("size1 = %d\n",size1);
	printf("size1 = %s\n",head);		

	char *p=strstr(head,"{");
	
	 
	//1.判断是否为 json 数据  
	  cJSON *obj=cJSON_Parse(p);
			if(obj == NULL)
			{
				char att[1024]="输入错了！！";
				ziku(att);
				printf("不是JSO数据\n");close(socket_1);
				return 0;
			}
			
	cJSON *value=cJSON_GetObjectItem(obj,"content");
	close(socket_1);
	
	strcpy(head ,strtok(cJSON_Print(value),"\""));
	//char tmp[1024] ;  char tmi[1024]; char head_1[1024] ;
	//sscanf(head ,"翻译：%s{br}结果：%s" ,tmp , tmi);
	//sprintf(head_1 ,"翻译：%s    结果：%s",buf ,tmi );
	
	//strcpy(tmp ,strtok(head,"\"{br}\n"));
	//ziku(head_1);
	//sleep(3);
	ziku(head);

	

}






int tianqi(char buf[1024])
{
	kk = 0 ;
	//1.创建TCP socket    
	int socket_2= socket(AF_INET, SOCK_STREAM, 0);
	printf("客户端的数据 :%s\n",buf);
/*
	 int fd1 = open("./1.txt", O_RDONLY) ;
	char bhh[4039*3];
	int yu = read ( fd1 ,bhh ,4039*3 );
	printf("%d",yu); 
	
	//1.判断是否为 json 数据  
	  cJSON *obj=cJSON_Parse(bhh);
			if(obj == NULL)
			{
				printf("不是JSO数据\n"); 
				return 0;
			}
		//取出城市
	cJSON *value=cJSON_GetObjectItem(obj,buf);
	printf("城市代码 ： %s \n",cJSON_Print(value));
*/
    // 2.设置服务器的IP地址并链接    
    struct sockaddr_in server_addre;
    server_addre.sin_family = AF_INET;
    server_addre.sin_port = htons(80); //HTTP 协议默认的端口就是 80 
    server_addre.sin_addr.s_addr = inet_addr("1.81.5.190"); //服务器的IP地址
											//图片的地址 

  // 3.链接 
  int ret = connect(socket_2,(struct sockaddr *)&server_addre,sizeof(server_addre));
	  if(ret < 0)
	  {
		printf("链接失败\n");
		return 0;
	  }
	  else
	  {
		  printf("链接成功\n");
	  }


	
	//制定 HTTP 请求协议   重点！！！ 
	char  http_head[1024*3]={0};
	sprintf(http_head,"GET /api/weather/city/%s HTTP/1.1\r\nHost:t.weather.itboy.net\r\n\r\n",buf);
													

	//发HTTTP 请求协议  
	write(socket_2,http_head,strlen(http_head));
	//新建一个文件  
		int fd=open("2.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
				if(fd < 0)
				{
					perror("");
					return -1;
				}
				int size1 = 0 ; int p_1 = 0 ;
	while(1)
	{
		char  head[1024]={0}; 
	   	size1 = read(socket_2,head,1024);printf("size1 = %s\n",head);
		p_1+=size1 ;
		write(fd,head,1024);	
		if(size1 < 1024 )
		{
			close(socket_2);
			close(fd);printf("222222222222222\n");
			break;
		}
		
	}
	
	printf("2222444444444444222\n");
	shuju(p_1);
}




int xiazai()
{

	printf("这里是显示图片\n");
			 
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
	lcd_draw_jpg_suofang(0,0,"1.jpg");


	//练习： 实现文件下载图片，文件中的回文头数据给删除掉！！！ 
	close(fd); 
	close(tcp_socket);


}



int main()
{
	//开启一个线程，循环打印GIF图片
	pthread_t TID ;
    pthread_create(&TID, NULL ,TS_THREAD, 0 );



	char afffff[1024]="你好！！";
	ziku(afffff);
	sleep(4);
	char affffff[1024]="请问有什么能帮到你吗？";
	ziku(affffff);



		//与pc链接接收数据  获取屏幕坐标
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
	 
	int on=1; //1就是开启的意思
    setsockopt(tcp_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)); //开启地址服用
	//2.绑定服务器通信socket     
	 struct sockaddr_in  server_addr;  //服务器地址信息
     server_addr.sin_family  =  AF_INET;	 //IPV4 协议
     server_addr.sin_port    =   htons(8888);  //端口号
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
	
	
	//设计多路复用的模型 
	while(1)
	{
		//1.清空集合
		fd_set set;
		FD_ZERO(&set);
		
		//2.把需要监听的描述符添加到集合中   
		FD_SET(0,&set);
		FD_SET(tcp_socket,&set);
		
		//3.添加所有的客户端文件描述符 
		int i=0;
		for(i=0;i<people;i++)
		{
			FD_SET(clien_arry[i],&set);
		}

		//4.开始监听集合 
		int ret = 0;
		if(people == 0)
		{
		   ret = select(tcp_socket+1,&set,NULL,NULL,NULL);
		}
		else
		{
			ret = select(clien_arry[people-1]+1,&set,NULL,NULL,NULL);
		}
		
		//处理活跃的描述符 
		if(ret < 0)
		{
			perror("");
			return 0; 
		}
		
		if(ret > 0)
		{
			
			//1.服务器发送数据
			if(FD_ISSET(0,&set))
			{
				
			}
			//有新的客户端链接请求
			if(FD_ISSET(tcp_socket,&set))
			{
				printf("接收客户端的请求\n");
				
				//接收客户端的链接请求 
				int new_socket=accept(tcp_socket,NULL,NULL);
					if(new_socket < 0)
					{
						perror("");
						return 0;
					}
					else
					{
						clien_arry[people] = new_socket; //保存描述符  
						
						people++; //人数增加
						
					}

				
				
			}
			
			//客户端有数据发送过来
			int i=0;
			for(i=0;i<people;i++)
			{
				if(FD_ISSET(clien_arry[i],&set))
				{
					printf("客户端 %d发送数据过来\n",clien_arry[i]);
				
					//接收客户端发送过来的数据
					char buf[1024]={0};
					int size=read(clien_arry[i],buf,1024);


					if(size <= 0)  //客户端掉线了
						{
							close(clien_arry[i]); 
							
							clien_arry[i]=0;
							printf("客户端掉线了 \n");
							continue;
							
						}


					
						if(strstr(buf,"tq"))  ///用 In 判断是否要开启天气功能 
						{
							char ppt[1024] ;
							strcpy(ppt , (strrchr(buf, '-')+1));
							printf("%s",ppt);
							printf("这是天气里面 \n");
							char juu[1024] = "请稍等 ， 正在为您查询 ! " ;
							ziku(juu);
							sleep(1);
							tianqi(ppt);
						}
						else if (strstr(buf,"exit"))
						{
							kk = 1 ;
							char ruu[1024] = "正在退出，请稍等 ! " ;
							ziku(ruu);
							sleep(2);
							char guu[1024] = "你好！！ " ;
							ziku(guu);
						}
						else if (strstr(buf,"tp"))
						{
							kk = 0 ;
							printf("这是下载图片里面 \n");
							char jeu[1024] = "请稍等 ， 正在为您下载 ！! " ;
							ziku(jeu);
							sleep(3);
							xiazai();
							sleep(10);
							char jet[1024] = "好看吗？ 嘻嘻 " ;
							ziku(jet);
							kk = 1 ;
						}
						else if (strstr(buf,"fy-"))
						{
							
							char ppr[1024] ;
							strcpy(ppr , (strrchr(buf, '-')+1));
							printf("这是切割后的数据：  %s\n",ppr);
							printf("这是翻译  \n");
							char jew[1024] = "请稍等 ， 正在为您翻译 ！! " ;
							ziku(jew);
							sleep(1);
							tcp_server_1(ppr);
							sleep(10);
							char jeq[1024] = "你好，请问还有什么能帮到你的吗？" ;
							ziku(jeq);
							
						}
						else
						{
							
							printf("这是来聊天");
							printf("客户端的数据 :%s\n",buf);
							tcp_server(buf);
							
						}
						
						
				}
				
			}
			
			
			
			
		}
		
		
		
	}
	
	


	



	return 0;	
}	