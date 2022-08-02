#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <sqlite3.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <arpa/inet.h>
#include <time.h>
#include<signal.h>


int sockfd;  //客户端套接字socket

enum
{
    REG,     //注册
    LOGIN,   //登录
    FORGET,  //忘记密码
    LOOKUSERS, //查看在线用户
    PRIVATE, //私聊
    GROUP,   //群聊
    ANONPRIVATE,//私聊(匿名聊天)
    ANONGROUP,  //群聊(匿名聊天)
    REPLY,   //回复
    EXCUSE,   //禁言
    ADMINISTRATOR,   //申请管理员
    OUTADMINISTRATOR,  //取消管理员
    WORLD,          //解除禁言
    KICK,  //踢人
    KILLUSER, //注销账户
};

//保存信息结构体
typedef struct Message
{
    char id[32];      //账号
    char name[32];    //昵称
    char myid[32];   //用于保存自己的id
    char passwd[32];   //密码
    char secret[32];   //密保
    char cmd[32];    //聊天的方式
    int cfd;        //聊天的对象
    char msg[128]; //聊天内容
    int  root;     //管理员标志
}Message;


//聊天室功能能选择界面
void menu();
//常用语功能界面
void menu1();
//常用语
char *PhrasalVerbs(int *select);
//写线程 
void *write_thread(void * arg);
//读线程 
void *read_thread(void * arg);
//修改退出聊天室的方式
void Close(int signum);

#endif


