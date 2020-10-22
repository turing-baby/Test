#include <stdio.h>	
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <errno.h>
#include <dirent.h>

#ifndef MYHEAD_H
#define MYHEAD_H

int dst;  														//显示屏驱动

//打开显示屏
int display_open()
{
	dst = open("/dev/fb0",O_RDWR);								//打开显示屏驱动
	if(dst == -1)
	{
		printf("显示屏打开失败！\n");
		return -1;
	}
}

//关闭显示屏
int display_close()
{
	close(dst);
}

//显示任意位置大小缩放的BMP图片
bool showbmp(char *bmppath, int n)	
{
	int w = 0;
	int h = 0;
	int src = open(bmppath , O_RDWR);
	
	lseek(src, 18 ,SEEK_SET);									//获取BMP图片的宽w信息
	read(src, &w, 4);
	lseek(src, 22 ,SEEK_SET);									//获取BMP图片的高h信息
	read(src, &h, 4);
	
	char bmpbuf[w*h*3];
	int  lcdbuf[w*h];
	int  tempbuf[w*h];
	
	int  lcdbuf1[] = {0};

	lseek(src, 54, SEEK_SET);									//跳过BMP图片头信息字节
	
	int rubbish = (4-(w*3)%4)%4;								//BMP图片字节不能被4整除时，加入的垃圾字节数
	for(int i = 0; i < h; i++)
	{
		read(src, &bmpbuf[w*i*3],w*3);
		lseek(src, rubbish, SEEK_CUR);							//在字节读入时，跳过垃圾字节
	}
		
	for(int i = 0; i < w*h; i++)								//将RGB转换成BGR
	{
		lcdbuf[i] = 0x00<<24 | bmpbuf[i*3+2]<<16 | bmpbuf[i*3+1]<<8 | bmpbuf[i*3];
	}
	
	

	for(int i = 0; i < w; i++)
	{
		for(int j = 0; j < h; j++)
		{
			tempbuf[(h-1-j)*w+i] = lcdbuf[j*w+i];				//BMP像素点上下反转
		}
	}

	
	//内存映射
	int *mmap_bmp = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, dst, 0);

	pid_t id1;
	int status;
	id1 = fork();
	if(id1 > 0)
	{
		for(int i=0; i<h/2/n; i++) 								//图片的高度h / 缩放倍数n
		{
			for(int j=w*n*i,k=0; j<(w*n*i+w);j+=n,k++) 			//循环决定每行该取的像素点
			{
				//任意位置(480-h/n)/2)、(800-w/n)/2)、缩小倍数n
				*(mmap_bmp+800*(((480-h/n)/2)+i)+((800-w/n)/2)+k) = tempbuf[j];
			}
			sleep(0.1);
		}
		sleep(1);
	}
	
	else if(id1 == 0)
	{
		for(int i=h/2/n-2; i<h/n; i++) 							//图片的高度h / 缩放倍数n
		{
			for(int j=w*n*i,k=0; j<(w*n*i+w);j+=n,k++) 			//循环决定每行该取的像素点
			{
				//任意位置(480-h/n)/2)、(800-w/n)/2)、缩小倍数n
				*(mmap_bmp+800*(((480-h/n)/2)+i)+((800-w/n)/2)+k) = tempbuf[j];
			}
			sleep(0.1);
		}
		exit(1);
	}
	pid_t otherid=waitpid(id1,&status,0);  						//阻塞等待

	//解除内存映射
	munmap(mmap_bmp, 800*480*4);
	close(src);
	
}


//设置节点
struct node
{
	char *data;
	struct node *next;
	struct node *prev;
};

//初始化链表
struct node *list_init(char *newdata)
{
	struct node *head = malloc(sizeof(struct node));
	head->data = newdata;
	head->next = head;
	head->prev = head;
	return head;
}

//创建新节点
struct node *newnode(char *newdata)
{
	struct node *new = malloc(sizeof(struct node));
	new->data = newdata;
	new->next = NULL;
	new->prev = NULL;
}

//加入新节点
int addnode(struct node *new,struct node *list)
{
	struct node *p = list;
	while(p->next != list)
	{
		p = p->next;
	}
	new->prev = list->prev;
	new->next = list;
	list->prev = new;
	new->prev->next = new;
}

#endif
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
main.c部分

#include "myhead.h"

int main()
{
	char buf[30];
	display_open();
	
	struct node *list = list_init("010.bmp");			//初始化双向循环链表
	char *path = "bmp";
	DIR *dp = opendir(path);
	struct dirent *p;
	
	while(p = readdir(dp))								//遍历目录文件
	{
		if(p->d_type == DT_REG)
		{
			if(strstr(p->d_name,".bmp"))				//判断是否为.bmp文件
			{
				struct node *new = newnode(p->d_name); 	//创建新节点
				addnode(new,list);  					//插入新节点
			}				
		}
	}

	struct node *head = list->next;
	display_open();
	while(1)
	{
		head = head->next;
		bzero(buf,20);
		sprintf(buf,"%s/%s",path,head->data);
		showbmp("background.bmp",1);				//背景图刷屏，传参“1”意为原大小显示，“2”即缩小到原来的1/2
		showbmp(buf,1);								//显示BMP图片，传参“1”意为原大小显示，“2”即缩小到原来的1/2
		sleep(1);
	}
	display_close();
}