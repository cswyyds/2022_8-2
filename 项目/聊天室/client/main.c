#include "client.h"

int main(int argc, char const *argv[])
{
    printf("正在启动聊天室\n");
    sleep(1);

    int ret;

    //注册信号函数
    //进行信号捕捉，将SIGINT信号的处理方式改成自己的处理方式
    //去执行我自己的功能
    if(signal(SIGINT,Close) == SIG_ERR)   
    {
        perror("signal");
        return -1;
    }

    pthread_t tid_read;
    pthread_t tid_write;

    //创建套接字
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sockfd)
    {
        perror("socket");
        return -1;
    }
    //向服务器发起连接
    struct sockaddr_in server_addr; //保存服务器信息
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;  //地址族 ipv4
    server_addr.sin_port = 8888; //网络字节序端口
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //ip地址
    ret = connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(-1 == ret)
    {
        perror("connect");
        return -1;
    }
    

    

    ret = pthread_create(&tid_read,NULL,(void *)read_thread,(void *)&sockfd);

    ret = pthread_create(&tid_write,NULL,(void *)write_thread,(void *)&sockfd);


    //阻塞等待回收指定线程
    pthread_join(tid_read,NULL);
    pthread_join(tid_write,NULL);


    close(sockfd);
   


    
    return 0;
}
