#include  <stdio.h>
#include  <cJSON.h>  //用户配置的 JSON 库  

int main()
{
	
	FILE *p  = fopen("./json.txt","r+");
	if (p == 0)
	{
		printf("1\n");
		return 0;
	}
	
	char json_buf[4096]={0};

	fread(json_buf,1,4096,p);
	fclose(p);
	
     //1.判断该字符串是否为 JSON 数据
      cJSON  *obj = cJSON_Parse(json_buf);
			 if(obj == NULL)
			 {
				printf("JSON 数据有误\n");
				return 0;				
			 }
			 else
			 {
				 printf("正确的json数据\n");
			 }

	//2.通过 key 获取value  
	cJSON *value_obj=cJSON_GetObjectItem(obj,"data");
	cJSON *value1=cJSON_GetObjectItem(value_obj,"city");
	cJSON *value_obj_1=cJSON_GetObjectItem(obj,"new");
	cJSON *value2=cJSON_GetObjectItem(value_obj_1,"xinwen");
	cJSON *value3=cJSON_GetObjectItem(value_obj_1,"dianji");
	

	//3.把value 转换成字符串输出
	char *str1=cJSON_Print(value1);
		printf("str1=%s\n",str1);
	char *str2=cJSON_Print(value2);
		printf("str2=%s\n",str2);
	char *str3=cJSON_Print(value3);
		printf("str2=%s\n",str3);


}

