#include "server.h"

int main(int argc, char **argv)
{
    printf("正在启动服务器\n");
    sleep(1);
    system("clear");
    int sockfd;
    int ret;

    //打开或者创建数据库
    sqlite3 *ppdb;
    ret = sqlite3_open("stu.db",&ppdb);
    if(ret != SQLITE_OK)
    {
        printf("sqlite3 open: %s\n",sqlite3_errmsg(ppdb));
        exit(-1);
    }

    printf("SOCKET3 INIT SUCCESS!\n");

    //创建表(用于保存注册用户的信息)
    CreatTable(ppdb);
    //创建第二张表用于保存聊天记录
    CreatTable2(ppdb);

    //遍历用户注册信息
    Id(ppdb);

    OnlineLinkList *head;      //id cfd next
    OnlineLinkList *new_node;

    thread_node node; //cfd head id pdb

    CreateLink(&head);

    //结构体指向链表
    node.head = head;
    //结构体指向数据库
    node.ppdb = ppdb;

     sockfd = socket(AF_INET,SOCK_STREAM,0);  //IPV4   流式套接字  具体协议类型
    if(-1 == sockfd)
    {
        perror("socket");
        exit(-1);
    }
    
    printf("SOCKET INIT SUCCESS!\n"); 

    

    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));  //设置地址可以被重复绑定
    struct sockaddr_in server_addr;          //保存服务器信息
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET; // 地址族  IPV4 
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);  //ip地址
    server_addr.sin_port = 8888;  //网络字节序端口
    //绑定信息
    ret = bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(-1 == ret)
    {
        perror("bind");
        exit(-1);
    }
    //设置监听序列
    ret = listen(sockfd,10);
    if(-1 == ret)
    {
        perror("listen");
        return -1;
    }

    printf("等待客户端进行连接.........\n");


    
    struct sockaddr_in Message_addr; //用于保存客户端信息
    int length = sizeof(Message_addr);

    while(1)
    {
        //阻塞等待客服端连接
        int ret = accept(sockfd,(struct sockaddr *)&Message_addr, &length);
        if(-1 == ret)
        {
            perror("accept");
            exit(-1);
        }
       
        printf("接收到客户端的连接 %d\n",ret);

        node.cfd = ret;  //cfd head id

        //为每一个客户端创建新的线程
        pthread_t tid;

        //创建线程  将cfd与head传给线程函数
        int fd = pthread_create(&tid,NULL,(void *)MyFun,(void *)&node);
        if(fd != 0)
        {
            perror("pthread_create");
            return -1;
        }
    }

    close(sockfd);

    return 0;
}