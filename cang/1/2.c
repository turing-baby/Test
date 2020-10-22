#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h> 
#include <sys/mman.h>

int show_shapebmp(int x,int y,int w,int h,const char bmpname)
{
int lcdfd;
int bmpfd;
int lcdmem;
int i,j;
char bmpbuf[wh3]; //char占一个字节
int lcdbuf[w*h]; // int 占4字节

//打开液晶屏的驱动
lcdfd = open("/dev/fb0",O_RDWR);
// 出错处理
if(lcdfd == -1)
{
	printf("open lcd failed!\n");
	return -1;
}

// 映射lcd(将lcd在内存中的地址告诉你)
lcdmem = (int *)mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
if(lcdmem == NULL)
{
	printf("map lcd failed!\n");
	return -1;
}
// 打开你要显示的图片
bmpfd = open(bmpname,O_RDONLY);
// 出错处理
if(bmpfd == -1)
{
	printf("open %s failed!\n",bmpname);
	return -1;
}
// 在你读取颜色值之前跳过54字节
lseek(bmpfd,54,SEEK_SET);
// 读出bmp图片的颜色值
read(bmpfd,bmpbuf,w*h*3);

// 将读取的3字节数据---》转换成4字节
for(i=0; i<w*h; i++)
	lcdbuf[i] = bmpbuf[3*i]|bmpbuf[3*i+1]<<8|bmpbuf[3*i+2]<<16|0x00<<24; // 0x00[2][1][0]

// 将转换后的数据直接填充到lcd的地址中去(i,j)--->(x+i,y+j)
// 将转换后的数据直接填充到lcd的地址中去(i,h-1-j)--->(x+i,y+j)
for(i=0; i<w; i++)
	for(j=0; j<h; j++)
		*(lcdmem+(y+j)*800+x+i) = lcdbuf[(h-1-j)*w+i];
// 关闭lcd和bmp
close(bmpfd);
close(lcdfd);
// 解除映射
munmap(lcdmem,800*480*4);
return 0;

