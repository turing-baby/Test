
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

 
char buf[9];

int  len;
int main(int argc, char const *argv[])
{

	int fd,fg;
	fd = open( argv[1] , O_RDWR | O_CREAT , 0777 );
     if (-1 == fd)
    {
        perror("open file erroe111");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
	fg = open(argv[2] , O_RDWR | O_CREAT ,0777 );
     if (-1 == fg)
    {
        perror("open file erroe2222");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
	while(len = read(fd,buf,9))
	{   
        if (-1 == len)
    {
        perror("open file erroe333");
        printf("open error:%s \n ",strerror(errno));
        return  -1;
    }
		write(fg,buf,len);
	}
	return 0;
}