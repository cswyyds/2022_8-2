#include "server.h"

//管理员身份标志   等于1的时候是管理员身份    等于0的时候不是管理员身份
int root;

//创建一个空链表(保存在线用户)
void CreateLink(OnlineLinkList **head)
{
    CreateNode(head);
    (*head)->next = NULL;  //初始化头指针指向空
}
//创建新的结点
void CreateNode(OnlineLinkList ** new_node)
{
    //定义新的结点，在堆区开辟空间
    *new_node = (OnlineLinkList *)malloc(sizeof(OnlineLinkList));
    //判断新结点开辟新空间是否成功
    if(NULL == new_node)
    {
        printf("malloc error!\n");  //打印错误信息
        exit(-1);  //异常退出
    }
}  
//头插法插入数据
void InsertNodeHead(OnlineLinkList *head, OnlineLinkList * new_node)
{
    //新的结点指向第一个结点的地址
    new_node->next = head->next;
    //头结点指向第一个结点
    head->next = new_node;

}

//客户端下线将cfd从链表中删除
void DeleteNode(thread_node *node)
{
    OnlineLinkList *p1 = NULL;
    OnlineLinkList *p2 = NULL;
    OnlineLinkList *head = NULL;
    int cfd;

    head = node->head;
    cfd = node->cfd;

    p1 = head->next;
    p2 = head;

    if(p1 == NULL)
    {
        
    }
    else
    {
        //cfd判断下线
        while(p1 != NULL && p1->cfd != cfd)
        {
            p2 = p1;
            p1 = p1->next;
        }

        if(p1 == NULL)
        {
            printf("no such Message!\n");
        }
        else
        {
            p2->next = p1->next;
            free(p1);
        }
    }
}

//线程函数
void *MyFun(void *arg)
{
    thread_node node;

    node = *((struct thread_node *)arg);

    
    while(1)
    {
        
         //收发消息
        MsgSendRecv(&node);
    }

    //关闭当前通信接口
    close(node.cfd);

    return NULL;
}

//查找id是否存在
int FindId(sqlite3 * ppdb , Message *data)  
{ 
    char sq1[128] ={0};
    sprintf(sq1,"select *from mytable;");
    char **result;
    int row,column;
    int flag = 0;

    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        return 1;
    }
    int Index = column;
    for(int i = 0; i < row ; i++)
    {
        for(int j = 0; j < column; j++)
        {
            if(Index%column == 0)
            {
                if(strcmp(result[Index] , data->id) == 0)
                {
                   flag = 1;
                   break;
                }
            }
            Index++;
        }
        if(flag == 1)
        {
            break;
        }
        
    }
    if(flag == 1)
    {
        sqlite3_free_table(result);
        return -1;
    }
    sqlite3_free_table(result);  
}

//注册
void Register(thread_node * node,Message *data)
{
    //记录cfd 传回注册成功
    data->cfd = node->cfd;
    //注册前查找是否被注册
    if(-1 == FindId(node->ppdb , data))
    {
        char arr[128] = {"账号已经存在,请重新注册"};
        send(data->cfd,arr,strlen(arr),0);
        return ;
    }
    else
    {
        //像数据库中插入数据
        InsertData(node->ppdb,data);
    }
}
    
//向数据库中插入数据
void InsertData(sqlite3 *ppdb,Message *data)  
{
    char str[128];
    char *sql = str;
    char *errmsg = NULL;

    sprintf(sql,"insert into mytable(id,name,passwd,secret) values('%s','%s','%s','%s');",
                                        data->id,data->name,data->passwd,data->secret);
    
     if(SQLITE_OK != sqlite3_exec(ppdb,sql,NULL,NULL,&errmsg))
    {
        printf("insert record fail! %s \n",errmsg);
        sqlite3_close(ppdb);
        exit(-1);
    }

    char arr[100] = {"账号注册成功"};
    send(data->cfd,arr,strlen(arr),0);
}

//创建表(用于保存注册用户的信息)
void CreatTable(sqlite3 *ppdb)
{
   //创建表
    char sq1[128] = {0};
    sprintf(sq1,"create table if not exists mytable(id char,name char,passwd char,secret char);");
    int ret = sqlite3_exec(ppdb,sq1,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }
}

//忘记密码
void ForgetSecret(thread_node * node,Message *data)
{
   //记录cfd 传回注册成功
    data->cfd = node->cfd;
    //更改密码前查询账号和密保是否有误
    if(-1 == FindSecret(node->ppdb , data))
    {
        //更新数据库
        UpdateData(node->ppdb,data);
    }
    else
    {
        char arr[128] = {"账号或密保错误\n"};
        send(data->cfd,arr,strlen(arr),0);
        return ;
        
    } 
}

//查找id和密保是否存在
int FindSecret(sqlite3 * ppdb , Message *data)
{
    char sq1[128] ={0};
    sprintf(sq1,"select *from mytable;");
    char **result;
    int row,column;
    int flag = 0;
    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        return -1;
    }
    int Index = column;
    for(int i = 0; i < row ; i++)
    {
        for(int j = 0; j < column; j++)
        {
            if(Index%column == 0)
            {
                int ret1 = strcmp(result[Index] , data->id);
                int ret2 = strcmp(result[Index+3] , data->secret);
                if( ret1== 0  && ret2 == 0)
                {
                    flag = 1;
                    break;
                }
            }
            Index++;
        }
        if(flag == 1)
        {
            break;
        }
        
    }
    if(flag == 1)
    {
        sqlite3_free_table(result);
        return -1;
    }
    sqlite3_free_table(result);  

}

//更新数据库(更改密码)
void UpdateData(sqlite3 *ppdb,Message *data)
{
    char sq1[128] ={0};
    sprintf(sq1,"update mytable set passwd='%s' where id= '%s'  ;",data->passwd, data->id);
    char **result;
    int row,column;
    int flag = 0;

    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }

    char arr[100] = {"密码更改成功"};
    send(data->cfd,arr,strlen(arr),0);
}

//检查账号是否重复登录
int RepeatLogin(thread_node *node , Message *data)
{
    OnlineLinkList *head = NULL;
    head = node->head;
    OnlineLinkList *p= NULL;
    p = head->next;

    if(p == NULL)
    {
        //无用户在线
        return 0;
    }
    
    while(p != NULL && strcmp(p->id,data->id) != 0)
    {
        p = p->next;
    }

    if(p == NULL)
    {
        //该id不在线
        return 0;
    }
    else
    {
        //已经登录
        return -1;
    }
}

//登录
void Login(thread_node *node , Message *data)
{
    if(-1 == RepeatLogin(node , data))
    {
        char arr[128] = {"您已经在线，无需重复登录"};
        send(node->cfd,arr,strlen(arr),0);
        return;
    }

    OnlineLinkList *new_node;
    OnlineLinkList *head = node->head;  
    //记录cfd 传回成功
    data->cfd = node->cfd;
    

    if(-1 != FindId(node->ppdb,data))
    {
        char arr[128] = {"该账号不存在，请重新登录"};
        send(data->cfd,arr,strlen(arr),0);
        return;
    }
    else
    {
        //登录前查找账号跟密码是否正确
        int  ret = VerifyIdPassword(node->ppdb, data);
        if(ret == -1) 
        {
            char arr[128] = {"登录成功"};
            send(data->cfd,arr,strlen(arr),0);

            //创建新的节点
            CreateNode(&new_node);
            /*
            //根据id查找name
            char sq1[128] ={0};
            sprintf(sq1,"select *from mytable;");
            char **result;
            int row,column;
            int flag = 0;

            int ret = sqlite3_get_table(node->ppdb,sq1,&result,&row,&column,NULL);
            if(ret != SQLITE_OK)
            {
                printf("sqlite3_get_table: %s\n",sqlite3_errmsg(node->ppdb));
                return ;
            }
            int Index = column;
            char aa[128];
            for(int i = 0; i < row ; i++)
            {
                for(int j = 0; j < column; j++)
                {
                    if(Index%column == 0)
                    {
                        if(strcmp(result[Index] , data->id) == 0)
                        {
                            strcpy(aa,result[Index + 1]);
                            flag = 1;
                            break;
                        }
                    }
                    Index++;
                }
                if(flag == 1)
                {
                    break;
                }
        
            } 
            */
            //把该账户的id复制的链表里 
            strcpy(new_node->id,data->id);


            new_node->cfd = data->cfd;
            new_node->root = 0;
            new_node->forbid_flag = 0;

            //头插法插入新的数据
            InsertNodeHead(head,new_node);
        }
        else
        {
            char arr[128] = {"账号或密码错误"};
            send(data->cfd,arr,strlen(arr),0);
            
        }
    }
}

//验证登录的账号和密码
int VerifyIdPassword(sqlite3 *ppdb , Message *data)
{
    char sq1[128] ={0};
    sprintf(sq1,"select *from mytable;");
    char **result;
    int row,column;
    int flag = 0;

    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        return -1;
    }
    int Index = column;
    for(int i = 0; i < row ; i++)
    {
        for(int j = 0; j < column; j++)
        {
            if(Index%column == 0)
            {

                int ret1 = strcmp(result[Index] , data->id);
                int ret2 = strcmp(result[Index + 2] , data->passwd);

                if( ret1 == 0  && ret2 == 0 )
                {
                    flag = 1;
                    break;
                }
            }
            Index++;
        }
        if(flag == 1)
        {
            break;
        }
        
    }
    
    
    if(flag == 1)
    {
        sqlite3_free_table(result);
        return -1;
    }
    sqlite3_free_table(result);  
}

//收发消息
void MsgSendRecv(thread_node * node)
{
    int ret;

    Message RecvInfo;

    ret = recv(node->cfd,&RecvInfo,sizeof(RecvInfo),0);
    
    if(ret == 0)
    {
        DeleteNode(node);
        pthread_exit(NULL);
    }  
    else
    {  
        //注册
        if(strcmp(RecvInfo.cmd,"REG") == 0)
        {
            Register(node,&RecvInfo);
        }
        //忘记密码
        else if(strcmp(RecvInfo.cmd,"FORGET") == 0)
        {
            ForgetSecret(node,&RecvInfo);
        }
        //登录
        else if(strcmp(RecvInfo.cmd,"LOGIN") == 0)
        {
            Login(node,&RecvInfo);
        }
        //查看在线用户
        else if(strcmp(RecvInfo.cmd,"LOOKUSERS") == 0)
        {
        
            if(-1 == InspectOwnOnline(node))
            {   
                char arr[128] ={"你未在线，不能查看在线用户，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {
                LookOnlineUsers(node);
            }
        } 
        //群聊
        else if(strcmp(RecvInfo.cmd,"GROUP") == 0)
        {
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"你未在线，不能群发消息，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {     
                GroupChat(node->ppdb ,node->head,&RecvInfo);
            }  
        }
        //私聊
        else if(strcmp(RecvInfo.cmd,"PRIVATE") == 0)
        {      
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"你未在线，不能私发消息，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {
                PrivateChat(node,&RecvInfo);
            }  
        }
        //群聊(匿名聊天)
        else if(strcmp(RecvInfo.cmd,"ANONGROUP") == 0)
        {
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"你未在线，不能群发消息，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {     
                AnonGroupChat(node->head,&RecvInfo,node->ppdb);
            }  
        }
        //私聊(匿名聊天)
        else if(strcmp(RecvInfo.cmd,"ANONPRIVATE") == 0)
        {      
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"你未在线，不能私发消息，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {
                AnonPrivateChat(node,&RecvInfo);
            }  
        }
        //申请管理员
        else if(strcmp(RecvInfo.cmd,"ADMINISTRATOR") == 0)
        {
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            
            int i;
            printf("账号%s正在申请管理员身份,是否同意\n",RecvInfo.id);
            printf("1: 是 2: 否\n");
            scanf("%d",&i);
            if(i == 1)
            {
                char arr[100] = {"管理员身份申请成功"};
                send(node->cfd,arr,strlen(arr),0);
                //将管理员标志置位1  cfd查找
                DaministerUser(node);
            }
            if(i == 2)
            {
                char arr[100] = {"不同意管理员身份申请"};
                send(node->cfd,arr,strlen(arr),0);
            }
        }
        //撤销管理员身份
        else if(strcmp(RecvInfo.cmd,"OUTADMINISTRATOR") == 0)
        {
            if(-1 == InspectOwnOnline(node))
            {
                char arr[128] ={"请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            int i;
            printf("账号%s正在申请取消管理员身份,是否同意\n",RecvInfo.id);
            printf("1: 是 2: 否\n");
            scanf("%d",&i);
            if(i == 1)
            {
                char arr[100] = {"撤销管理员身份成功"};
                send(node->cfd,arr,strlen(arr),0);
                //将管理员标志置位1  cfd查找
                OutDaministerUser(node);
            }
            if(i == 2)
            {
                char arr[100] = {"不同意撤销管理员身份申请"};
                send(node->cfd,arr,strlen(arr),0);
            }
        }
        //踢人
        else if (strcmp(RecvInfo.cmd,"KICK") == 0)
        {
            KickUser(node,RecvInfo.id);          
        }
        //禁言
        else if(strcmp(RecvInfo.cmd,"EXCUSE") == 0)
        {
            int ret = ForbidWorld(node,RecvInfo.id);
            if(0 == ret)
            {
                char arr[100] = {"该id用户不在线"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else if(-1 == ret)
            {
                usleep(1);
                char arr[100] = {"禁言成功"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {
                char arr[100] = {"对方是管理员，不可禁言"};
                send(node->cfd,arr,strlen(arr),0);
            }
        }
        //解禁
        else if(strcmp(RecvInfo.cmd,"WORLD") == 0)
        {
            int ret = CancelForbidWorld(node,RecvInfo.id);
            if(0 == ret)
            {
                char arr[100] = {"该id用户不在线"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else if( 1 == ret)
            {
                char arr[100] = {"解禁成功"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            {
                char arr[100] = {"该用户不需要解禁"};
                send(node->cfd,arr,strlen(arr),0);
            }
        }
        //注销用户
        else if(strcmp(RecvInfo.cmd,"KILLUSER") == 0)
        {
            CancelUser(node,&RecvInfo);
        }
        //查看聊天记录
        else if(strcmp(RecvInfo.cmd,"LOOKCHATRECORD") == 0)
        {
        
            if(-1 == InspectOwnOnline(node))
            {   
                char arr[128] ={"你未在线，不能查看在线用户，请先登录"};
                send(node->cfd,arr,strlen(arr),0);
            }
            else
            PrintChatRecord(node->ppdb , node);
            
        }
    }
    
}

//遍历用户注册信息
void Id(sqlite3 * ppdb )  
{
    char sq1[128] ={0};
    sprintf(sq1,"select *from mytable;");
    char **result;
    int row,column;
    int flag = 0;

    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }
    int Index = column;
    for(int i = 0; i < row ; i++)
    {
        for(int j = 0; j < column; j++)
        {
                printf("%s = %s  ",result[j] , result[Index]);
            
            Index++;
        }
        putchar(10); 
    
    }  
}
//群聊
void GroupChat(sqlite3 *ppdb , OnlineLinkList *head , Message *data)
{
    int length;
    OnlineLinkList *p = NULL;
    p = head->next;
    length = strlen(data->msg);
    char aa[100];
    int len;
    memset(aa,0,sizeof(aa));

    char chat[1000] = {0};
    strcpy(chat ,data->name);
    strcat(chat ,":");
    strcat(chat , data->msg);

    InsertChatData(ppdb,chat);
    
    while(p != NULL)
    {
        //发给每一个客户端
        strcpy(aa,data->name);
        strcat(aa,":");
        len = strlen(aa);
        send(p->cfd,aa,len,0);
        send(p->cfd,data->msg,length,0);
        p = p->next;
    }
}

//私聊
void PrivateChat(thread_node *node , Message *data)
{
    int length;
     
    OnlineLinkList *p = NULL;
    
    p = node->head->next;

    length = strlen(data->msg);

    char aa[100];
    int len;
    memset(aa,0,sizeof(aa));
    //寻找在线用户链表中的cfd与私聊中的cfd是否一致
    while(p != NULL && strcmp(p->id , data->id) != 0)
    {
        p = p->next;
    }

    if(p == NULL)
    {
        char arr[100] = {"该用户不在线"};
        send(node->cfd,arr,strlen(arr),0);
    }
    else
    {
        strcpy(aa,data->name);
        strcat(aa,":");
        len = strlen(aa); 
        send(p->cfd,aa,len,0);
        send(p->cfd,data->msg,length,0);

        char chat[1000] = {0};
        strcpy(chat ,data->name);
        strcat(chat ,":");
        strcat(chat , data->msg);
        InsertChatData(node->ppdb,chat);
    }
    
}

//群聊(匿名聊天)
void AnonGroupChat(OnlineLinkList *head , Message *data , sqlite3 *ppdb)
{
    int length;
    OnlineLinkList *p = NULL;
    p = head->next;
    length = strlen(data->msg);

    char aa[100];
    int len;
    memset(aa,0,sizeof(aa));

    char chat[1000] = {0};
    strcpy(chat ,"匿名消息");
    strcat(chat ,":");
    strcat(chat , data->msg);

    InsertChatData(ppdb,chat);
    
    
    while(p != NULL)
    {
        //发给每一个客户端
        strcpy(aa,"匿名消息");
        strcat(aa,":");
        len = strlen(aa);
        send(p->cfd,aa,len,0);
        send(p->cfd,data->msg,length,0);
        p = p->next;
    }
}

//私聊(匿名聊天)
void AnonPrivateChat(thread_node *node , Message *data )
{
    int length;
     
    OnlineLinkList *p = NULL;
    
    p = node->head->next;

    length = strlen(data->msg);

    char aa[100];
    int len;
    memset(aa,0,sizeof(aa));

    //寻找在线用户链表中的cfd与私聊中的cfd是否一致
    while(p != NULL && strcmp(p->id , data->id) != 0)
    {
        p = p->next;
    }

    if(p == NULL)
    {
        char arr[100] = {"该用户不在线"};
        send(node->cfd,arr,strlen(arr),0);
    }
    else
    {
        strcpy(aa,"匿名消息");
        strcat(aa,":");
        len = strlen(aa); 
        send(p->cfd,aa,len,0);
        send(p->cfd,data->msg,length,0);

        char chat[1000] = {0};
        strcpy(chat ,"匿名消息");
        strcat(chat ,":");
        strcat(chat , data->msg);
        InsertChatData(node->ppdb,chat);

    }
    
}

//查看在线用户
int LookOnlineUsers(thread_node * node)
{
    OnlineLinkList *head = NULL;
    head = node->head;
    OnlineLinkList *p = NULL;
    p = head->next;

    char bb[128];

    if(p == NULL)
    {  
        return 0;   //当前无用户在线
    }

    while(p != NULL)
    {
        memset(bb,0,sizeof(bb));
        strcpy(bb,p->id);
        send(node->cfd,bb,strlen(bb),0);
        p = p->next;
        usleep(1);
    }

    return 1;
}

//检查自己是否在线
int InspectOwnOnline(thread_node *node)
{
    OnlineLinkList *head= NULL;
    head = node->head;
    OnlineLinkList *p= NULL;
    p = head->next;

    if(p == NULL)
    {
        //无用户在线
        return -1;
    }

    while(p != NULL && p->cfd != node->cfd)
    {
        p = p->next;
    }

    if(p == NULL)
    {
        //自己不在线
        return -1;
    }
    else
    {
        //自己已经登录
        return 0;
    }

}

//申请管理员身份
void DaministerUser(thread_node *node)
{
    OnlineLinkList *p = NULL;
    OnlineLinkList *head = NULL;

    head = node->head;

    p =head->next;

    if(p == NULL)
    {
        return;
    }
    else
    {
        while(p != NULL && p->cfd != node->cfd)
        {
            p = p->next;
        }
        if(p == NULL)
        {
            return;
        }
        else
        {
            p->root = 1;
            return;
        }
        
    }
}

//撤销管理员身份
void OutDaministerUser(thread_node *node)
{
    OnlineLinkList *p = NULL;
    OnlineLinkList *head = NULL;

    head = node->head;

    p =head->next;

    if(p == NULL)
    {
        return;
    }
    else
    {
        while(p != NULL && p->cfd != node->cfd)
        {
            p = p->next;
        }
        if(p == NULL)
        {
            return;
        }
        else
        {
            p->root = 0;
            return;
        }
        
    }

}

//踢人
void KickUser(thread_node *node,char * ID)   
{
    OnlineLinkList *p1 = NULL;
    OnlineLinkList *p2 = NULL;
    OnlineLinkList *head = NULL;  
    char *id = ID;

    head = node->head;

    p1 = head->next;
    p2 = head;

    if(p1 == NULL)
    {
        char arr[100] = {"该用户不在群组，踢人失败"};
        send(node->cfd,arr,strlen(arr),0);
    }
    else
    {
        //cfd判断下线
        while(p1 != NULL && strcmp(p1->id,id) != 0)
        {
            p2 = p1;
            p1 = p1->next;
        }

        if(p1 == NULL)
        {
            char arr[100] = {"该用户不存在"};
            send(node->cfd,arr,strlen(arr),0);
        }
        else
        {
            if(p1->root != 1)
            {
                p2->next = p1->next;
                //踢人
                free(p1);
                char arr[100] = {"对方已被强制下线"};
                send(node->cfd,arr,strlen(arr),0);
                memset(arr,0,sizeof(arr));
                char *ar;
                ar = "你已经被管理员强制下线";
                send(p1->cfd,ar,strlen(ar),0);

            }
            else
            {
                char arr[100] = {"对方是管理员，无权强制对方下线"};
                send(node->cfd,arr,strlen(arr),0);
            }
        }
    }
}

//禁言
int ForbidWorld(thread_node *cfd_node,char *ID)
{
    OnlineLinkList *head = NULL;
    head = cfd_node->head;
    OnlineLinkList *p = NULL;
    p = head->next;

    if(p == NULL)
    {
        //无用户在线
        return 0;
    }

    while(p != NULL && strcmp(p->id,ID) !=0 )
    {
        p = p->next;
    }

    if(p == NULL)
    {   
        //该用户不在线
        return 0;
    }
    else
    {
        if(p->root == 1)
        {
            //对方是管理员不可禁言
            return 1;
        }
        else
        {
            p->root = 1;
            char arr[100] = {"你已经被管理员禁言"};
            send(p->cfd,arr,strlen(arr),0);
            return -1;
        }
        
        
    }
}

//解禁
int CancelForbidWorld(thread_node *cfd_node,char *ID)
{
    OnlineLinkList *head = NULL;
    head = cfd_node->head;
    OnlineLinkList *p = NULL;
    p = head->next;

    if(p == NULL)
    {
        //无用户在线
        return 0;
    }

    while(p != NULL && strcmp(p->id,ID) !=0 )
    {
        p = p->next;
    }

    if(p == NULL)
    {
        //该用户不在线
        return 0;
    }
    else
    {
        char arr[100] = {"你已经被管理员解禁"};
        p->forbid_flag = 0;
        send(p->cfd,arr,strlen(arr),0);
        usleep(1);      
    }
}

//注销用户
void CancelUser(thread_node * node,Message *data)
{
   //记录cfd 传回注册成功
    data->cfd = node->cfd;
    //注销账户前查询账号和密保是否有误
    if(-1 == FindSecret(node->ppdb , data))
    {
        //删除注册信息
        DeleteData(node->ppdb,data);
        KillLinkListUser(node,data->id);
    }
    else
    {
        char arr[128] = {"账号或密保错误\n"};
        send(data->cfd,arr,strlen(arr),0);
        return ;
        
    } 
}

//删除注册信息
void DeleteData(sqlite3 *ppdb,Message *data)
{
    char sq1[128] ={0};
    sprintf(sq1,"delete from mytable where id = '%s';",data->id);
    char **result;
    int row,column;

    int ret = sqlite3_get_table(ppdb,sq1,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }

    char arr[100] = {"注销成功"};
    send(data->cfd,arr,strlen(arr),0);
}

//删除注销用户在链表内的信息
void KillLinkListUser(thread_node *node,char * ID)  
{
    OnlineLinkList *p1 = NULL;
    OnlineLinkList *p2 = NULL;
    OnlineLinkList *head = NULL;  
    char *id = ID;

    head = node->head;

    p1 = head->next;
    p2 = head;

    if(p1 == NULL)
    {
        
    }
    else
    {
        //cfd判断下线
        while(p1 != NULL && strcmp(p1->id,id) != 0)
        {
            p2 = p1;
            p1 = p1->next;
        }

        if(p1 == NULL)
        {
            char arr[100] = {"该用户不存在"};
            send(node->cfd,arr,strlen(arr),0);
        }
        else
        {          
                p2->next = p1->next;              
                free(p1);
        }
    }
}

//创建第二张表用于保存聊天记录
void CreatTable2(sqlite3 *ppdb)
{
    //创建表
    char sql[128] = {0};
    sprintf(sql , "create table if not exists chat(chat char);");
    int ret = sqlite3_exec(ppdb,sql,NULL,NULL,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_exec: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }
}

//向第二张表中插入聊天记录
void InsertChatData(sqlite3 *ppdb , char *chat)
{
    char str[128];
    char *sql = str;
    char *errmsg = NULL;

    sprintf(sql,"insert into chat(chat) values('%s');", chat);
    
    if(SQLITE_OK != sqlite3_exec(ppdb,sql,NULL,NULL,&errmsg))
    {
        printf("insert record fail! %s \n",errmsg);
        sqlite3_close(ppdb);
        exit(-1);
    }


    sprintf(sql,"insert into chat(chat) values('%s');", "\n");
    if(SQLITE_OK != sqlite3_exec(ppdb,sql,NULL,NULL,&errmsg))
    {
        printf("insert record fail! %s \n",errmsg);
        sqlite3_close(ppdb);
        exit(-1);
    }
}

//遍历聊天记录
void  PrintChatRecord(sqlite3 *ppdb , thread_node *node)
{
    OnlineLinkList *head = NULL;
    head = node->head;
    OnlineLinkList *p = NULL;
    p = head->next;

    char sql[128] = {0};
    //sprintf(sql , "selete *from mytable;");
    sprintf(sql,"select *from chat");
    char **result;
    int row,column;

    int ret = sqlite3_get_table(ppdb,sql,&result,&row,&column,NULL);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3_get_table: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }

    int Index = column;
    char chat[1000] = {0};  

    for(int i = 0; i < row ; i++)
    {
        for(int j = 0; j < column; j++)
        {
            strcpy(chat , result[Index]);
            send(node->cfd , chat ,strlen(chat), 0);
            Index++;
        }
    }
    if(row == 0) //当聊天记录为0行的时候聊天记录为空
    {
        strcpy(chat , "当前还没有聊天记录");
        send(node->cfd , chat ,strlen(chat), 0);
        return;
    }
    else
    {
        return; 
    }
}