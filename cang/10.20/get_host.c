#include <stdio.h>
#include <netdb.h>

#if 0
      struct hostent {
               char  *h_name;            /*域名*/
               char **h_aliases;         /*域名的别名列表list */
               int    h_addrtype;        /*域名的类型*/
               int    h_length;          /* 地址的长度*/
               char **h_addr_list;       /*域名的IP地址列表*/
           }
#endif  
	
//练习：利用gethostbyname 把  百度的其他信息全部输出！！ 
int main()
{
	//通过域名获取对方的IP地址信息 
	struct hostent *addr=gethostbyname("www.baidu.com");
					if(addr == NULL)
					{
						perror("");
						return 0;
					}
					
	//输出获取到的信息 
	printf("addr->h_name=%s\n",addr->h_name);
	
	int i=0;
	while(1)
	{
		if(addr->h_aliases[i] == NULL)
		{
			break;
		}
		
		printf("addr->h_aliases[i]=%s\n",addr->h_aliases[i]);
		i++;
	}
	printf("addr->h_addrtype=%d\n",addr-> h_addrtype);
	printf("addr->h_length=%d\n",addr->h_length);
	
	int a=0;
	while(1)
	{
		if(addr->h_addr_list[a] == NULL)
		{
			break;
		}
		
		printf("addr->h_addr_list[%d]=%s\n",a,inet_ntop(addr->h_addr_list[a]));//转换数据出错！！
		//需要需要转换数据类型才能打印出来
		a++;
	}

}