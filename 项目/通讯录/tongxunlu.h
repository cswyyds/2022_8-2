#ifndef _TONGXUNLU_H_
#define _TONGXUNLU_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//保存联系人信息结构体
typedef struct AddressBook{
    char name[100];
    char tel[100];
    char addr[100];
}AddressBook;
//宏定义通讯录的长度为
#define SIZE 100  

AddressBook addrbook[SIZE];

FILE *fp;  //创建文件流指针

//通讯录界面
int menu();
//添加联系人
int add_new_person(int n);
//查看全部联系人
void look_all_person(int n);
//搜索联系人
void search_person(int n);
//修改联系人信息
void change_person_data(int n);
//删除联系人
int delete_person(int n);
//删除全部联系人
int delete_all_person(int n);
//把联系人信息写入文件
void write_file(int n);
//把联系人的信息从文件读到结构体数组内
int read_file(int n);

#endif