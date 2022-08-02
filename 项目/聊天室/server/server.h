#ifndef _SERVER_H_
#define _SERVER_H_
#include<sqlite3.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<time.h>
#include<signal.h>
#include<arpa/inet.h>


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
    EXCUSE,   //禁言
    ADMINISTRATOR,   //申请管理员
    OUTADMINISTRATOR,  //取消管理员
    WORLD,          //解除禁言
    KICK,  //踢人
    KILLUSER, //注销账户
    LOOKCHATRECORD, //聊天记录
};

//在线用户链表
typedef struct OnlineLinkList
{
    char id[100];
    char name[32];
    int cfd;
    int root;     
    int forbid_flag;  
    struct OnlineLinkList *next; //指针域，为了能够操作后面结点
                                 //所以指针的类型为当前结构体的类型
}OnlineLinkList;

typedef struct thread_node
{
    int cfd;
    OnlineLinkList *head;
    sqlite3 *ppdb;
}thread_node;

//保存信息结构体
typedef struct Message
{
    char id[32];     //账号
    char myid[32];   //用于保存自己的id
    char name[32];   //昵称
    char passwd[32];  //密码
    char secret[32];   //密保
    char cmd[32];      //聊天方式
    int cfd;          //聊天对象
    char msg[128];   //聊天内容
    int  root;      //管理员标志
    char chat[1000];  //聊天记录
}Message;




//创建一个空链表
void CreateLink(OnlineLinkList **head);
//创建新的结点
void CreateNode(OnlineLinkList ** new_node);
//头插法插入数据
void InsertNodeHead(OnlineLinkList *head, OnlineLinkList * new_node);
//遍历用户注册信息
void Id(sqlite3 * ppdb ) ;
//线程函数
void *MyFun(void *arg);
//查找id是否存在
int FindId(sqlite3 * ppdb , Message *data);
//注册
void Register(thread_node * node , Message *data); 
//向数据库中插入数据
void InsertData(sqlite3 *ppdb , Message *data);
//创建表
void CreatTable(sqlite3 *ppdb);
//收发消息
void MsgSendRecv(thread_node * cfd_node);
//查找账号和密保是否存在
int FindSecret(sqlite3 * ppdb , Message *data);
//更新数据库(更改密码)
void UpdateData(sqlite3 *ppdb , Message *data);
//登录
void Login(thread_node *cfd_node , Message *data);
//登录验证
int VerifyIdPassword(sqlite3 *ppdb , Message *data);
//群聊
void GroupChat(sqlite3 *ppdb , OnlineLinkList *head , Message *data);
//私聊
void PrivateChat(thread_node *node , Message *data);
//群聊(匿名聊天)
void AnonGroupChat(OnlineLinkList *head , Message *data , sqlite3 *ppdb);
//私聊(匿名聊天)
void AnonPrivateChat(thread_node *node , Message *data);
//查看在线用户
int LookOnlineUsers(thread_node * node);
//检查自己是否在线
int InspectOwnOnline(thread_node *node);
//检查账号是否重复登录
int RepeatLogin(thread_node *node , Message *data);
//申请管理员身份
void DaministerUser(thread_node *node);
//撤销管理员身份
void OutDaministerUser(thread_node *node);
//踢人
void KickUser(thread_node *node , char *ID);
//禁言
int ForbidWorld(thread_node *cfd_node,char *ID);
//解禁
int CancelForbidWorld(thread_node *cfd_node,char *ID);
//注销用户
void CancelUser(thread_node * node,Message *data);
//删除注册信息
void DeleteData(sqlite3 *ppdb,Message *data);
//删除注销用户在链表内的信息
void KillLinkListUser(thread_node *node,char * ID); 
//创建第二张表用于保存聊天记录
void CreatTable2(sqlite3 *ppdb);
//向第二张表中插入聊天记录
void InsertChatData(sqlite3 *ppdb , char *chat);
//遍历聊天记录
void  PrintChatRecord(sqlite3 *ppdb , thread_node *node);

#endif