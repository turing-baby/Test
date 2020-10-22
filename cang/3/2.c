#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
//#include<conio.h>
#include<ctype.h>
#define Esc 27
#define NULL 0
#define LEN sizeof(struct student)
struct student
{
	char class_0[20];					//班级
	int num;							//学号
	char name[20];						//姓名
	float c_prog;						//c语言程序设计
	float eng;							//大学英语
	float math;							//高等数学
	float avg;							//平均成绩
	int order;							//名次
	struct student *next;
};
int n;									//全局变量，统计学生记录的个数
//函数声明
struct student *create();
void Output(struct student *head);
struct student *Delete(struct student *head, int num);
struct student *insert(struct student *head, struct student *stud);
struct student *Lookup(struct student *head, int num);
struct student *Modify(struct student *head, int num);
float Statistic(struct student *p);
struct student *order(struct student *head);
void start();
void goon();
void exit();
//创建链表，输入学生信息
struct student *create()
{
	struct student *head;
	struct student *pl;
	n=0;
	pl=(struct student*) malloc(LEN);
	printf("\n\n\t请输入学生的信息:  \n");
	printf("\t输入格式为：(每输入一项回车，以学号为0退出!)\n");
	printf("\t请输入学生学号:");
	scanf("%d",&pl->num );
	printf("\t请输入学生姓名:");
	scanf("%s",&pl->name );
	printf("\t请输入学生班级:");
	scanf("%s",&pl->class_0 );
	printf("\n");
	printf("\t请输入程序设计成绩:");
	scanf("%f",&pl->c_prog );
	printf("\t请输入大学英语成绩:");
	scanf("%f",&pl->eng );
	printf("\t请输入高等数学成绩:");
	scanf("%f",&pl->math );
	printf("\n");
	head=NULL;
	while(pl->num !=0)
	{
		pl->avg =Statistic(pl);			//求pl的平均值
		head=insert(head,pl);			//创建链表
		pl=(struct student*)malloc(LEN);
		printf("\n\t如果你想结束输入，请输入 0 !\n\n");
		printf("\t请输入学号:");
		scanf("%d",&pl->num );
		if(pl->num ==NULL)				//控制是否退出
		continue;
		printf("\t请输入学生姓名:");
		scanf("%s",&pl->name );
		printf("\t请输入学生班级:");
		scanf("%s",&pl->class_0 );
		printf("\n");
		printf("\t请输入程序设计成绩:");
		scanf("%f",&pl->c_prog );
		printf("\t请输入大学英语成绩:");
		scanf("%f",&pl->eng );
		printf("\t请输入高等数学成绩:");
		scanf("%f",&pl->math );
		printf("\n");
}
	return (head);
}
//输出链表
void Output(struct student *head)
{
	struct student *p;
	printf("\n\t现在有%d个记录是:\n",n);
	p=head;
	printf("\n\n\n****************学生成绩信息表****************\n\n");
	printf("\n学号\t姓名\t班级\t程序设计  大学英语  高等数学  平均分  学生名次\n");
	if(head!=NULL)  
		do
		{
			printf("%-10d",p->num );
			printf("%-10s",p->name );
			printf("%-8s",p->class_0 );
			printf("%-8.2f",p->c_prog );
			printf("\t%-8.2f",p->eng );
			printf("%-8.2f",p->math );
			printf("%-8.2f",p->avg );
			printf("\t%-3d\n",p->order  );
			p=p->next ;
		} while(p!=NULL);
}
//根据学号来删除学生信息
struct student *Delete(struct student *head, int num)
{
	struct student *p1,*p2;
	if(head==NULL)			//空链表时返回
	{
		printf("\n\t链表为空!\n");
		return(head);
	}
	p1=head;
	while(num!=p1->num &&p1->next!=NULL)					
	{
		p2=p1;
		p1=p1->next ;									
	}
	if(num==p1->num )									//链表找到相应的学号
	{
		if(p1==head)
			head=p1->next ;
		else											//表中和表尾删除
			p2->next =p1->next ;
		printf("\n\t你删除的学生信息为: \n");
		printf("\t学号为：%d\t",p1->num );
		printf("\t姓名为：%s\t",p1->name );
		printf("\t班级为：%s\t",p1->class_0 );
		printf("\n");
		printf("\t程序设计成绩为：%.2f\t",p1->c_prog );
		printf("\n");
		printf("\t\t大学英语成绩为：%.2f\t",p1->eng );
		printf("\t\t高等数学成绩为：%.2f\t",p1->math );
		printf("\n");
		printf("\t\t平均成绩为：%.2f\t",p1->avg );
		printf("\t学生名次为：%d\t",p1->order );
		printf("\n");
		n=n-1;
		free(p1);
	}
	else				//找不到学号
		printf("\t学号%d没有找到!\n",num);
	return(head);
}
//插入学生信息
struct student *insert(struct student *head, struct student *stud)
{
	struct student *p0,*p1,*p2;
	p1=head;
	p0=stud;
	if(head==NULL)				//空链表时返回
	{
		head=p0;
		p0->next =NULL;
	}
	else
	{
		while((p0->avg <p1->avg   )&&(p1->next  !=NULL))
		{
			p2=p1;
			p1=p1->next;
		}
		if(p0->avg >=p1->avg )
		{
			if(p1==head)				//链表头插入
			{
				p0->next =p1;
				head->next =p0;
			}
			else						//链表中插入
			{
				p2->next =p0;
				p0->next =p1;
			}
		}
		else							//链表尾部插入
		{
			p1->next =p0;
			p0->next =NULL;
		}
	}
		n=n+1;
		p0->order=0;		//初始化名次
		return(head);	
}
//根据学生学号来查找学生信息
struct student *Lookup(struct student *head, int num)
{
	struct student *p1,*p2;
	p1=head;
	if(head==NULL)		//空链表时返回
	{
		printf("\n\t链表为空！\n");
		return(head);
	}
	else
	{
		while(num!=p1->num &&p1->next !=NULL)
		{
			p2=p1;
			p1=p1->next ;
		}
		if(num==p1->num )							//找到相应的学号则显示
		{
			printf("\t你查找的学生信息为：\n");
			printf("\t学号为：%d\n",p1->num );
			printf("\t姓名为：%s\n",p1->name );
			printf("\t班级为：%s\n",p1->class_0 );
			printf("\t程程序设计成绩为：%.2f\n",p1->c_prog );
			printf("\t大学英语成绩为：%.2f\n",p1->eng );
			printf("\t高等数学成绩为：%.2f\n",p1->math );
			printf("\t平均成绩为：%.2f\n",p1->avg );
			printf("\t学生名次为：%d\n",p1->order );
			
		}
		else						//	学号不在链表内
			printf("\t你输入的学号不在链表内！\n");
	}
	return(head);
}
//根据学生学号来修改学生的信息
struct student *Modify(struct student *head, int num)
{
	struct student *p1,*p2;
	struct student *stude;
	stude=(struct student*)malloc(LEN);
	p1=head;
	if(head==NULL)				//链表为空时不能改变信息
	{
		printf("\n\t链表为空！\n");
		return(head);
 
	}
	else
	{
		while(num!=p1->num &&p1->next !=NULL)
		{
			p2=p1;
			p1=p1->next;
		}
		if(num==p1->num )						//	找到相应的学号时
		{
			if(p1==head)						//表头删除
				head=p1->next ;
			else								//表中和表尾删除
				p2->next =p1->next ;
			printf("\t你要修改的学号为%d的先前信息为：\n",num);
			printf("\t学号为：%d\n",p1->num );
			printf("\t姓名为：%s\n",p1->name );
			printf("\t班级为：%s\n",p1->class_0 );
			printf("\n");
			printf("\t程序设计成绩为：%.2f\n",p1->c_prog );
			printf("\t大学英语成绩为：%.2f\n",p1->eng );
			printf("\t高等数学成绩为：%.2f\n",p1->math );
			printf("\t平均成绩为：%.2f\n",p1->avg );
			printf("\t学生名次为：%d\n",p1->order );
			printf("\n");
			printf("\t请输入你要改变的成绩：（格式为：每输入一次回车）\n");
			printf("\t请输入程序设计成绩：");
			scanf("%f",&p1->c_prog );
			printf("\t请输入大学英语成绩：");
			scanf("%f",&p1->eng );
			printf("\t请输入高等数学成绩：");
			scanf("%f",&p1->math );
			p1->c_prog =stude->c_prog ;
			p1->eng =stude->eng ;
			p1->math =stude->math ;
			p1->avg =Statistic(p1);
			head=insert(head,p1);
			n=n-1;
		}
		else					//找不到学号时
			printf("\t你要修改的学号不在链表内！\n");
	}
	return(head);
}
//根据学生平均分来排名
struct student *order(struct student *head)
{
	struct student *p;
	int i=1;
	p=head;
	while(p!=NULL)
	{
		p->order =i;
		i++;
		p=p->next ;
	}
	return(head);
}
//根据输入的各科成绩来计算平均分
float Statistic(struct student *p)
{
	p->avg =(p->c_prog +p->eng +p->math )/3;
		return p->avg ;
}
void menu()		//系统主界面
{
	printf("\n\n\t\t	= = =欢迎使用班级成绩管理系统= = =");
	printf("\n\n\t\t		1.新建学生成绩表	");
	printf("\n\n\t\t		2.输出学生成绩表		");
	printf("\n\n\t\t		3.学生成绩排序		   ");
	printf("\n\n\t\t		4.按学号查找学生成绩信息");
	printf("\n\n\t\t		5.按姓名修改学生成绩信息  ");
	printf("\n\n\t\t		6.添加学生成绩信息 ");
	printf("\n\n\t\t		7.按学号删除学生信息	");
	printf("\n\n\t\t		ESC.退出系统    ");
	printf("\n\n\t\t	------------------------------------");
	printf("\n\n\t\t	  ********请您选择操作选项********	");
	printf("\n\t\t请输入您的选择(1-7)：");
}
void goon()
{
	printf("按任意键继续！！！");
	getch();
}

//主函数
 
 
main()
{
	struct student *head,*stu;
	int Delete_num;
	int lookup_num;
	int Modify_num;
	char ckey='a';
	int istate=0;
	do
	{
		system("cls");				//vc++清屏函数，包含在#include<stdlib.h>中
		menu();
		ckey=getch();
		if(ckey=='1')				//创建
		{
			system("cls");
			head=create();
			head=order(head);
			Output(head);
			istate=1;				//记录链表是否有数据
			goon();
		}
		else if((istate==0)&&(ckey!=Esc))
		{
			printf("\n\t错误：你必须先输入学生信息！！！");
			goon();
		}
		else if(ckey=='2')					///显示
		{
			system("cls");
			Output(head);
			goon();
		}
		else if(ckey=='3')					//排序
		{
			system("cls");
			order(head);
			Output(head);
			goon();
		}
		else if(ckey=='4')					//查找
		{
			system("cls");
			printf("  \n\t请输入你要查找的学生学号：");
			scanf("%d",&lookup_num);
			head=Lookup(head,lookup_num);
			head=order(head);
			goon();
		}
		else if(ckey=='5')				//修改
		{
			system("cls");
			printf("  \n\t请输入你要修改学生信息的学号：");
			scanf("%d",&Modify_num);
			head=Modify(head,Modify_num);
			head=order(head);
			goon();
		}
		else if(ckey=='6')				//插入
		{
			system("cls");
			printf("  \n\t请输入你要插入的学生信息：\n\n");
			stu=(struct student*)malloc(LEN);
			printf("\t请输入学生信息，格式为：（每输入一项回车）\n");
			printf("\t请输入学生学号：");
			scanf("%d",&stu->num );
			printf("\t请输入学生姓名：");
			scanf("%s",&stu->name );
			printf("\t请输入学生班级：");
			scanf("%s",&stu->class_0 );
			printf("\n");
			printf("\t请输入程序设计成绩：");
			scanf("%f",&stu->c_prog );
			printf("\t请输入大学英语成绩：");
			scanf("%f",&stu->eng );
			printf("\t请输入高等数学成绩：");
			scanf("%f",&stu->math );
			printf("\n");
			stu->avg=Statistic(stu);				//求stu 的平均值
			head=insert(head,stu);
			head=order(head);
			goon();
		}
		else if(ckey=='7')				//删除
		{
			system("cls");
			printf("\n\t请输入你要删除的学生学号");
			scanf("%d",&Delete_num);
			head=Delete(head,Delete_num);
			head=order(head);
			goon();
		}
	}
	while(ckey!=Esc);				//按键盘上的Esc键退出！！！
		
}