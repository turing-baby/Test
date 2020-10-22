#include  <stdio.h>
#include  <cJSON.h>  //用户配置的 JSON 库  


int main()
{
	//1.打开文件获取json 数据 
	FILE *fp  = fopen("./json.txt","r+");
		 if(fp == NULL)
		 {
			 perror("");
			 return 0;
		 }
	 
	//读取文本中的数据 
	 char  json_buf[4096]={0};
	 fread(json_buf,1,4096,fp);
	 fclose(fp);  
	 
	//1.判断是否为 json 数据  
	  cJSON *obj=cJSON_Parse(json_buf);
			if(obj == NULL)
			{
				printf("不是JSO数据\n");
				return 0;
			}
	
		//取出数组
		cJSON *value=cJSON_GetObjectItem(obj,"data");
		cJSON *arry1=cJSON_GetObjectItem(value,"city");
		cJSON *arry2=cJSON_GetObjectItem(value,"wendu");
		cJSON *arry3=cJSON_GetObjectItem(value,"ganmao");
		printf("城市 ： %s \n",cJSON_Print(arry1));
		printf("今日温度 ： %s \n",cJSON_Print(arry2));
		printf("提示 ： %s \n",cJSON_Print(arry3));
        printf(" \n");


		cJSON *arry=cJSON_GetObjectItem(value,"forecast");
		if(arry == NULL)
		{
			printf("数组取出失败\n");
			return 0;
		}
	
	    //求出数组的大小 
		int size=cJSON_GetArraySize(arry);
		//printf("当前数组的大小 %d\n",size);
		
		
		//遍历数组
		for(int i=0;i<size;i++)
		{
			//通过数组的下标获取对象  
			cJSON *new_obj=cJSON_GetArrayItem(arry,i);
				if(new_obj == NULL)
				{
					printf("获取对象失败\n");
					return 0;
				}
			
			//处理每一项中的数据 
			cJSON *value0=cJSON_GetObjectItem(new_obj,"fengxiang");
			cJSON *value1=cJSON_GetObjectItem(new_obj,"fengli");
			cJSON *value2=cJSON_GetObjectItem(new_obj,"high");
			cJSON *value3=cJSON_GetObjectItem(new_obj,"type");
			cJSON *value4=cJSON_GetObjectItem(new_obj,"low");
			cJSON *value5=cJSON_GetObjectItem(new_obj,"date");
			printf("日期 ： %s \n",cJSON_Print(value5));printf("天气 ： %s \n",cJSON_Print(value3));
			printf("最高温度 ：  %s \n",cJSON_Print(value2));
			printf("最低温度 ：%s \n",cJSON_Print(value4));
			printf("风向 ： %s \n",cJSON_Print(value0));printf("风力 ： %s \n",cJSON_Print(value1));	
			printf(" \n");

			
			
		}
	
	
	
}

