#include "tongxunlu.h"

//通讯录界面
int menu()
{
    printf("**********************************************\n\n");
    printf("\t\t1.添加联系人                      \n\n");
    printf("\t\t2.查看所有联系人                   \n\n");
    printf("\t\t3.搜索联系人                      \n\n");
    printf("\t\t4.修改联系人信息                   \n\n");
    printf("\t\t5.删除联系人                      \n\n");
    printf("\t\t6.删除全部联系人                   \n\n");
    printf("\t\t0.退出通讯录                      \n\n");
    printf("**********************************************\n");
    int choice;
    scanf("%d",&choice);
    getchar();
    return choice;
}
//添加联系人
int add_new_person(int n)
{
    int a = 1;

    
    printf("现在开始添加联系人信息\n");
    while(a)
    {
        a = 0;
        printf("姓名\n");
        scanf("%s",addrbook[n].name);
        getchar();
        for(int i = 0; i < n ; i++)
        {
            if(strcmp(addrbook[n].name ,addrbook[i].name) == 0)
            {
                a = 1;
                printf("该联系人已存在，请重新输入\n");
            }
        }
    }
    
    printf("电话号码\n");
    scanf("%s",addrbook[n].tel);
    getchar();
    printf("家庭住址\n");
    scanf("%s",addrbook[n].addr);
    getchar();
    printf("联系人信息添加成功\n");
    write_file(n); //把联系人信息写入文件
    n++;
    return n;
} 
//查看全部联系人
void look_all_person(int n)
{
    if(n == 0)
    {
        printf("通讯录为空,快去添加联系人吧\n");
        return;
    }
    AddressBook temp;
    for(int i = 0; i < n ; i++)
    {
        for(int j = i + 1 ; j < n; j++)
        {
            if(strcmp(addrbook[i].name , addrbook[j].name) > 0)
            {
                temp = addrbook[i];
                addrbook[i] = addrbook[j];
                addrbook[j] = temp;
            }
        }
    }
    printf("根据首字母进行排序\n");
    putchar(10);
    printf("姓名\t电话号码\t家庭住址\n");
    for(int i = 0; i < n; i++)
    {   
        printf("%s\t%s\t%s\n",addrbook[i].name,addrbook[i].tel,addrbook[i].addr);
    }
    putchar(10);
    write_file(n);
    printf("共打印%d条联系人信息\n",n);
    return;
}
//搜索联系人
void search_person(int n)
{
    if(n == 0)
    {
        printf("通讯录为空,快去添加联系人吧\n");
        return;
    }

    int flag = 0;  
    char name[100];
    printf("请输入要查询联系人的姓名\n");
    scanf("%s",name);
    getchar();
    for(int i = 0; i < n; i++)
    {
        if(strcmp(name , addrbook[i].name) == 0)
        {
            flag = 1;
            printf("联系人查找成功\n");
            putchar(10);
            printf(" 姓名\t电话号码\t家庭住址\n");
            printf("%s\t%s\t%s\n",addrbook[i].name,addrbook[i].tel,addrbook[i].addr);
            return;
        }
    }
    if(flag == 0)
    {
        printf("查询失败，通讯录中没有该联系人\n");
    }
}
//修改联系人信息
void change_person_data(int n)
{
    if(n == 0)
    {
        printf("通讯录为空\n");
        return;
    }

    int flag = 0;  
    char name[100];
    printf("请输入要查询联系人的姓名\n");
    scanf("%s",name);
    getchar();
    for(int i = 0; i < n; i++)
    {
        if(strcmp(name , addrbook[i].name) == 0)
        {
            flag = 1;
            printf("请输入新的联系人信息\n");
            printf("新姓名\n");
            scanf("%s",addrbook[i].name);
            getchar();
            printf("新电话号码\n");
            scanf("%s",addrbook[i].tel);
            getchar();
            printf("新家庭住址\n");
            scanf("%s",addrbook[i].addr);
            getchar();
            printf("联系人信息修改成功\n");
        }
    }
    write_file(n); //更新文件内联系人信息
    if(flag == 0)
    {
        printf("通讯录中没有该联系人\n");
    }    
}
//删除联系人
int delete_person(int n)
{
    int flag = 0;
    int a;  
    char name[100];
    printf("请输入要删除联系人的姓名\n");
    scanf("%s",name);
    if(n == 0)
    {
        printf("通讯录还没保存联系人呢\n");
        return 0;
    }
    for(int i = 0; i < n; i++)
    {
        if(strcmp(name , addrbook[i].name) == 0)
        {
            flag = 1;
            printf("确定要删除%s吗？\n",name);
            printf("1.确定   2返回菜单\n");
            scanf("%d",&a);
            if(a == 1)
            {
                for(int j = i; j < n; j++)
                {
                    addrbook[j] = addrbook[j + 1];
                    sleep(1);
                }
                
            }
            printf("删除联系人成功\n");
            if(a == 2)
            {
                return n;
            }
            
        }
    }
    
    if(flag == 0)
    {
        printf("查询失败，通讯录中没有该联系人\n");
        return n;
    }
    n--;
     write_file(n); //更新文件内联系人信息
    return n;
}
//删除全部联系人
int delete_all_person(int n)
{
    if(n == 0)
    {
        printf("通讯录为空\n");
        return 0;
    }

    int a;
    printf("确定要全部删除吗？\n");
    printf("1.确定  2.返回菜单\n");
    scanf("%d",&a);
    if(a == 1)
    {
        write_file(0); //更新文件内联系人信息
        sleep(2);
        printf("联系人已清空\n");
        return 0;
        
    }
    if(a == 2)
    {
        return n;
    }
}
//把联系人信息写入文件
void write_file(int n)
{
    fp = fopen("PersonChat.txt","w"); //以写的方式打开文件PersonChat.txt
    if(fp == NULL)                    //每次写都是清空文件写入
    {
        perror("open");  //判断文件是否出错
        return;
    }
    else
    {       //
        fwrite(&n , sizeof(int), 1 , fp); //以二进制的形式写入文件
    }                          
    for(int i = 0; i < n ; i++)
    {       //写入数据的首地址   对象的大小        个数  目标文件流指针
        fwrite(&addrbook[i],sizeof(AddressBook),1 , fp);
    }
    printf("\n");

    fclose(fp); //关闭文件描述符
}
//把联系人的信息从文件读到结构体数组内
int  read_file(int n)
{
    fp = fopen("PersonChat.txt","r"); //以只读的方式打开文件PersonChat.txt
    if(fp == NULL)                    //每次读都是从开头开始读
    {
        perror("open");
        return n;
    }
    fread(&n,sizeof(int),1,fp);
    for(int i = 0; i < n; i++)
    {
        fread(&addrbook[i],sizeof(AddressBook),1,fp);        
    }
    fclose(fp);
    return n; 
}
