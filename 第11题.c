#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 20  //姓名最大长度
#define MAX_STUDENT_NUMBER 100  //最大学生人数
#define MAX_SCORE 100  //最大成绩
#define MIN_ID 2025100000  // 学号最小值
#define MAX_ID 2025909999  // 学号最大值
struct grade
{
	float math;
	float english;
	float history;	
};
struct student
{
	char name[MAX_NAME_LENGTH];
	long long id;
	struct grade score;
};

void judge(FILE *file);  //判断文件是否打开成功
void judge_id(struct student *information ,int i);  //判断学号是否合法
void input_name(struct student *stu);  //输入姓名
void clean_buffer();  //清除输入缓冲区
int main()
{
	FILE *file = fopen("student_information.txt", "w");
    judge(file);
    struct student information[MAX_STUDENT_NUMBER];
    printf("请输入学生信息:(q结束)\n");
	char key='0';
	int i=0;
	int j=0;
	/*输入学生信息部分*/
	while (key!='q'&&i<MAX_STUDENT_NUMBER) 
 	{
 		clean_buffer();
        printf("学生%d:\n",i+1);
 		printf("请输入姓名:");
        input_name(information+i);
		if (strcmp(information[i].name, "q") == 0)
        {
            printf("退出输入\n");
            break;
        }
 		printf("\n请输入学号:");
		judge_id(information,i);
		printf("\n请输入成绩:(依次为数学、英语、历史)\n"); 
 		while (scanf("%f",&information[i].score.math)!=1||
        information[i].score.math<0||information[i].score.math>MAX_SCORE)
        {
            printf("输入的第%d个学生的数学成绩格式错误\n",i+1);
            printf("请输入0-%d之间的数学成绩:\n",MAX_SCORE);
            clean_buffer();
        }  //判断数学成绩是否合法
        while (scanf("%f",&information[i].score.english)!=1||
        information[i].score.english<0||information[i].score.english>MAX_SCORE) 
        {
            printf("输入的第%d个学生的英语成绩格式错误\n",i+1);
            printf("请输入0-%d之间的英语成绩:\n",MAX_SCORE);
            clean_buffer();
        }  //判断英语成绩是否合法
        while (scanf("%f",&information[i].score.history)!=1||
        information[i].score.history<0||information[i].score.history>MAX_SCORE)
        {
            printf("输入的第%d个学生的历史成绩格式错误\n",i+1);
            printf("请输入0-%d之间的历史成绩:\n",MAX_SCORE);
            clean_buffer();
        }  //判断历史成绩是否合法
		printf("是否继续输入下一个学生的信息?(q结束,其他键继续)");
		scanf(" %c",&key);
		i++;
        clean_buffer();
	}
	if (i==0)
	{
		printf("未输入任何学生信息\n");
		fclose(file);
		return 0;
	}
	if (i==MAX_STUDENT_NUMBER)
	{
		printf("输入的学生人数已达%d人\n",MAX_STUDENT_NUMBER);
	}
	for(j=0;j<i;j++)
    {
        fprintf(file, "第%d个学生:\n",j+1);
        fprintf(file, "姓名:%s\n学号:%lld\n",information[j].name,information[j].id);
        fprintf(file, "成绩:\n数学:%.2f\n英语:%.2f\n历史:%.2f\n",
                information[j].score.math,
                information[j].score.english,
                information[j].score.history);
    }
	printf("学生信息已保存至student_information.txt文件\n");
	fclose(file);
    /*数据文件写入部分*/
    FILE *file_data = fopen("student_information_data.dat", "wb+");
    judge(file_data);
    size_t count_write = fwrite(information, sizeof(struct student), i, file_data);
    if (count_write != i)  //判断写入的学生人数是否与输入的学生人数一致
    {
        printf("写入的学生人数为%zu人\n",count_write);
        fclose(file_data);
        perror("文件写入失败");
        exit(EXIT_FAILURE);
    }
    printf("学生信息已保存至student_information_data.dat文件\n");
    rewind(file_data);  //将文件指针定位到文件开头，准备读取
    struct student read_information[MAX_STUDENT_NUMBER];
    size_t count_read = fread(read_information, sizeof(struct student), i, file_data);
    if (count_read != i)  //判断读取的学生人数是否与输入的学生人数一致
    {
        printf("读取的学生人数为%zu人\n",count_read);
        fclose(file_data);
        perror("文件读取失败");
        exit(EXIT_FAILURE);
    }
    /*序号查询部分*/
    printf("请输入要查询的学生序号(1-%d):", i);
    int query_serial=0;
    while (1)
    {
        if (scanf("%d",&query_serial)!=1)
        {
            printf("输入的序号格式错误\n");
            clean_buffer();
        }
        else if (query_serial<1||query_serial>i)
        {
            printf("输入的序号超出范围(1-%d)\n",i);
            clean_buffer();
        }
        else
        {
            clean_buffer();
            break;
        }
    }  //判断输入的序号是否合法
    int index = query_serial - 1;  //将序号转换为数组索引
    long offset = index * sizeof(struct student);  //计算偏移量
    if (fseek(file_data, offset, SEEK_SET) != 0)  //定位到指定偏移量，成功则返回0
    {
        perror("fseek定位失败");
        fclose(file_data);
        exit(EXIT_FAILURE);
    }
    struct student found_student;
    if (fread(&found_student, sizeof(struct student), 1, file_data) != 1)  //读取学生信息
    {
        perror("读取学生信息失败");
        fclose(file_data);
        exit(EXIT_FAILURE);
    }
    printf("\n查询结果(第%d个学生):\n", query_serial);
    printf("姓名:%s\n学号:%lld\n", found_student.name, found_student.id);
    printf("成绩:\n数学:%.2f\n英语:%.2f\n历史:%.2f\n",
           found_student.score.math,
           found_student.score.english,
           found_student.score.history);
    fclose(file_data);
    return 0;
}

void judge(FILE *file)
{
	if (file == NULL)
    {
        perror("文件打开失败");
        exit(EXIT_FAILURE);
    }
}

void input_name(struct student *stu)
{
    fgets(stu->name, MAX_NAME_LENGTH , stdin);
    int len=strlen(stu->name);
    if (len>0&&stu->name[len-1]=='\n')  //如果输入的姓名以换行符结尾，则将换行符替换为字符串结束符
    {
        stu->name[len-1]='\0';
        len--;
    }
}

void clean_buffer()
{
    int c;
    while ((c=getchar())!='\n'&&c!=EOF)
    {
        continue;
    }
}

void judge_id(struct student *information ,int i)
{
    while (1)
        {
            if (scanf("%lld",&information[i].id)!=1)
            {
                printf("输入的学号格式错误\n");
                clean_buffer();
                printf("请重新输入学号:\n");
            }
            else if (information[i].id<MIN_ID||information[i].id>MAX_ID)
            {
                printf("学号不能小于%lld或大于%lld\n",MIN_ID,MAX_ID);
                clean_buffer();
                printf("请重新输入学号:\n");
            }
            else if (i>0)
            {
                int j=0;
                for(j=0;j<i;j++)
                {
                    if (information[i].id==information[j].id)
                    {
                        printf("学号已存在\n");
                        clean_buffer();
                        printf("请重新输入学号:\n");
                        break;
                    }
                }
                if (j==i)
                {
                    break;
                }
            }
            else
            {
                clean_buffer();
                break;
            }
        }
}