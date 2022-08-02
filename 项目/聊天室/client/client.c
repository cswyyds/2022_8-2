#include "client.h"

//管理员身份标志   等于1的时候是管理员身份    等于0的时候不是管理员身份
int root = 0;

//禁言标志   等于1是被禁言  等于0的时候没有被禁言
int forbid_flag = 0;

//修改退出聊天室的方式
void Close(int signum)
{
    printf("请正确退出聊天室\n");
}

//聊天室功能能选择界面
void menu()
{
    printf("***********聊*************天*************室**************\n\n");
    printf("                          1.登录                       \n\n");
    printf("                          2.注册                       \n\n");
    printf("                          3.匿名聊天                    \n\n");
    printf("                          4.私聊                       \n\n");
    printf("                          5.群聊                       \n\n");
    printf("                          6.踢人                       \n\n");
    printf("                          7.禁言                       \n\n");
    printf("                          8.解除禁言                    \n\n");
    printf("                          9.忘记密码                    \n\n");
    printf("                         10.查看在线用户                 \n\n");
    printf("                         11.请求管理员身份               \n\n");
    printf("                         12.撤销管理员身份               \n\n");    
    printf("                         13.注销账户                    \n\n");
    printf("                         14.查看聊天记录                 \n\n"); 
    printf("                         00.退出聊天室                   \n\n");
    printf("*********************************************************\n");
}

//常用语功能界面
void menu1()
{
    printf("*******常***********用************语*******\n\n");  
    printf("            1.中国人不骗中国人           \n\n");
    printf("            2.花有重开日,人无再少年       \n\n");
    printf("            3.YYDS                    \n\n");
    printf("            4.大哥，别卷了              \n\n");
    printf("            5.我躺好了                  \n\n");
    printf("            6.摆烂了                    \n\n");
    printf("            7.干饭人                    \n\n");
    printf("            8.哒咩                    \n\n");
    printf("            9.芭比Q了                   \n\n");
    printf("           10.人生无常，大肠包小肠                   \n\n");
    printf("           11.gg                      \n\n");
    printf("           12.emo了                   \n\n");
    printf("           13.破大防了，家银们          \n\n");
    printf("           14.杀伤力不大，侮辱性极强     \n\n");
    printf("           15.啊，对对对对             \n\n");
    printf("           16.牛蛙兄弟                 \n\n");
    printf("           17.人家想要贴贴             \n\n");
    printf("           18.普信女             \n\n");
    printf("           19.你个老六             \n\n");
    printf("           20.这里有老六             \n\n");
    printf("     输入其他编号切换手动输入信息           \n\n");
    printf("********************************************\n");
    
}

//常用语
char *PhrasalVerbs(int *select)
{
    char *world;
    world = (char *)malloc(32);
    if(*select == 1)
    {
        strcpy(world,"中国人不骗中国人");
        return world;
    }
    if(*select == 2)
    {
        strcpy(world,"花有重开日,人无再少年");
        return world;
    }
    if(*select == 3)
    {
        strcpy(world,"YYDS");
        return world;
    }
    if(*select == 4)
    {
        strcpy(world,"大哥，别卷了");
        return world;
    }
    if(*select == 5)
    {
        strcpy(world,"我躺好了");
        return world;
    }
    if(*select == 6)
    {
        strcpy(world,"摆烂了");
        return world;
    }
    if(*select == 7)
    {
        strcpy(world,"干饭人");
        return world;
    }
    if(*select == 8)
    {
        strcpy(world,"哒咩");
        return world;
    }
    if(*select == 9)
    {
        strcpy(world,"芭比Q了");
        return world;
    }
    if(*select == 10)
    {
        strcpy(world,"人生无常，大肠包小肠");
        return world;
    }
    if(*select == 11)
    {
        strcpy(world,"gg");
        return world;
    }
    if(*select == 12)
    {
        strcpy(world,"emo了");
        return world;
    }
    if(*select == 13)
    {
        strcpy(world,"破大防了，家银们");
        return world;
    }
    if(*select == 14)
    {
        strcpy(world,"杀伤力不大，侮辱性极强");
        return world;
    }
    if(*select == 15)
    {
        strcpy(world,"啊，对对对对");
        return world;
    }
    if(*select == 16)
    {
        strcpy(world,"牛蛙兄弟");
        return world;
    }
    if(*select == 17)
    {
        strcpy(world,"人家想要贴贴");
        return world;
    }
    if(*select == 18)
    {
        strcpy(world,"普信女");
        return world;
    }
    if(*select == 19)
    {
        strcpy(world,"你个老六 ");
        return world;
    }
    if(*select == 20)
    {
        strcpy(world,"这里有老六");
        return world;
    }
    else
    {
        strcpy(world,"NO");
        return world; 
    }
}

//读线程
void *read_thread(void * arg)
{  
    char receive[128];
    int length;

    sockfd = *((int *)arg);

    while(1)
    {
        memset(receive, 0, sizeof(receive));
        length = recv(sockfd,receive,100,0);
        if(length == 0)
        {
            pthread_exit(NULL);
        }

        receive[length] = '\0';

        if(strcmp(receive,"管理员身份申请成功") == 0)
        {
            printf("%s\n",receive);

            root = 1;
        }
        else if(strcmp(receive,"撤销管理员身份成功") == 0)
        {
           printf("%s\n",receive); 

           root = 0;
        }
        else if(strcmp(receive,"你已经被管理员禁言") == 0)
        {
            printf("%s\n",receive); 
            forbid_flag = 1;
        }
        else if(strcmp(receive,"你已经被管理员解禁") == 0)
        {
            printf("%s\n",receive);
            forbid_flag = 0;
           
        }
        else
        {
            printf("%s\n" , receive);
        }       
    }
    pthread_exit(NULL);
}

//写线程 
void *write_thread(void * arg)
{
    system("clear");
    char sendline[128];
    Message message;
    int a;  //判断是否注销账户
    int a1;  //判断是否匿名聊天方式
    int a2; //判断是否使用常用语
    int select;

    sockfd = *((int *)arg);
   
    while(1)
    {        
        menu();     
        scanf("%d",&select);
        getchar();

        switch (select)
        {
        
        //登录
        case 1:   //登录
            printf("账号:\n");
            scanf("%s",message.id);
            getchar();
            strcpy(message.myid,message.id);
            printf("密码:\n");
            scanf("%s",message.passwd);
            getchar();
            strcpy(message.cmd,"LOGIN");
            printf("正在登录，请稍后......\n");
            sleep(1);
            send(sockfd,&message,sizeof(message),0);
            sleep(1);
            system("clear");
            break;
        //注册    
        case 2:    
            printf("id:\n");
            scanf("%s",message.id);
            getchar();
            printf("昵称:\n");
            scanf("%s",message.name);
            getchar();
            printf("密码:\n");
            scanf("%s",message.passwd);
            getchar();
            printf("请输入密保:\n");
            scanf("%s",message.secret);
            getchar();
            strcpy(message.cmd,"REG");
            printf("正在注册，请稍后......\n");
            sleep(1);
            send(sockfd,&message,sizeof(message),0);
            sleep(1);
            system("clear");
            break;
        //匿名聊天
        case 3:
            if(forbid_flag == 0)
            {
                printf("请选择匿名聊天的聊天方式\n"); 
                printf("1 : 匿名私聊  2 : 匿名群聊 \n");
                scanf("%d", &a1);
                getchar();
                //匿名私聊 
                if(a1 == 1)
                {
                    printf("是否需要使用聊天常用语快捷输入\n");
                    printf("1 : 是  2 : 否 \n");
                    scanf("%d", &a2);
                    getchar();
                    if(a2 == 1)
                    {
                        int select1;
                        system("clear");
                        sleep(1);
                        menu1();
                        printf("请输入编号\n");
                        scanf("%d",&select1);
                        getchar();
                        char *world;
                        world = PhrasalVerbs(&select1);

                        if(strcmp(world,"NO") != 0)
                        {
                            strcpy(message.cmd,"ANONPRIVATE");
                            printf("请输入对方的昵称:\n");
                            scanf("%s",message.id);
                            strcpy(message.msg,world);
                            send(sockfd,&message,sizeof(message),0);
                            free(world);      //记住一定要用free释放，否则会造成内存泄露
                            break;
                        }
                        a2 =2;
                    }
                    if (a2 == 2)
                    {
                        printf("请输入消息:\n");
                        memset(sendline,0,sizeof(sendline));
                        fgets(sendline,128,stdin);
                        strcpy(message.cmd,"ANONPRIVATE");
                        printf("请输入对方的昵称:\n");
                        scanf("%s",message.id);
                        strcpy(message.msg,sendline);
                        send(sockfd,&message,sizeof(message),0);
                        break;
                    }
                    else
                    {
                        printf("输入错误，正在跳转功能页面\n");
                        sleep(1);
                        break;
                    }             
                }
                //匿名群聊
                if(a1 == 2)
                {
                    printf("是否需要使用聊天常用语快捷输入\n");
                    printf("1 : 是  2 : 否 \n");
                    scanf("%d", &a2);
                    getchar();
                    if(a2 == 1)
                    {
                        int select1;
                        system("clear");
                        sleep(1);
                        menu1();
                        printf("请输入编号\n");
                        scanf("%d",&select1);
                        getchar();
                        char *world;
                        world = PhrasalVerbs(&select1);

                        if(strcmp(world,"NO") != 0)
                        {
                            strcpy(message.cmd,"ANONGROUP");
                            strcpy(message.msg,world);
                            send(sockfd,&message,sizeof(message),0);
                            free(world);      //记住一定要用free释放，否则会造成内存泄露
                            break;
                        }
                        a2 =2;

                    }
                    if(a2 == 2)
                    {
                        printf("请输入消息:\n");
                        memset(sendline, 0, sizeof(sendline));
                        fgets(sendline,100,stdin);
                        strcpy(message.cmd,"ANONGROUP");
                        strcpy(message.msg,sendline);
                        send(sockfd,&message,sizeof(message),0);
                        break;
                    }
                    else
                    {
                        printf("输入错误，正在跳转功能页面\n");
                        sleep(1);
                        break;
                    }                 
                }
                else
                {
                    printf("输入错误，正在跳转功能页面\n");
                    sleep(1);
                    break;
                }

            }
            else
            {
                printf("你已经被管理员禁言\n");
            }
            break;   
        //私聊
        case 4:
            if(forbid_flag == 0)
            {
                printf("是否需要使用聊天常用语快捷输入\n");
                printf("1 : 是  2 : 否 \n");
                scanf("%d", &a2);
                getchar();
                if(a2 == 1)
                {
                    int select1;
                    system("clear");
                    sleep(1);
                    menu1();
                    printf("请输入编号:");
                    scanf("%d",&select1);
                    getchar();
                    char *world;
                    world = PhrasalVerbs(&select1);

                    if(strcmp(world,"NO") != 0)
                    {
                         strcpy(message.cmd,"PRIVATE");
                        printf("请输入对方的昵称:\n");
                        scanf("%s",message.id);
                        strcpy(message.name,message.myid);
                        strcpy(message.msg,world);
                        send(sockfd,&message,sizeof(message),0);
                        free(world);  //记住一定要用free释放，否则会造成内存泄露  
                        break;                     
                    }
                    a2 =2;

                }
                if(a2 == 2)
                {
                    printf("请输入消息:\n");
                    memset(sendline,0,sizeof(sendline));
                    fgets(sendline,128,stdin);
                    strcpy(message.cmd,"PRIVATE");
                    printf("请输入对方的昵称:\n");
                    scanf("%s",message.id);
                    strcpy(message.name,message.myid);
                    strcpy(message.msg,sendline);
                    send(sockfd,&message,sizeof(message),0);
                    break;
                }
                else
                {
                    printf("输入错误，正在跳转功能页面\n");
                    sleep(1);
                }                
            }
            else
            {
                printf("你已经被管理员禁言\n");
            }
            break;
        //群聊
        case 5:
            if(forbid_flag == 0)
            {
                printf("是否需要使用聊天常用语快捷输入\n");
                printf("1 : 是  2 : 否 \n");
                scanf("%d", &a2);
                getchar();
                if(a2 == 1)
                {
                    int select1;
                    system("clear");
                    sleep(1);
                    menu1();
                    printf("请输入编号\n");
                    scanf("%d",&select1);
                    getchar();
                    char *world;
                    world = PhrasalVerbs(&select1);

                    if(strcmp(world,"NO") != 0)
                    {
                        strcpy(message.cmd,"GROUP");
                        strcpy(message.name,message.myid);
                        strcpy(message.msg,world);
                        send(sockfd,&message,sizeof(message),0);
                        free(world);      //记住一定要用free释放，否则会造成内存泄露
                        break;
                    }
                    a2 =2;

                }
                if(a2 == 2)
                {
                    printf("请输入消息:\n");
                    memset(sendline, 0, sizeof(sendline));
                    fgets(sendline,100,stdin);
                    strcpy(message.name,message.myid);
                    strcpy(message.cmd,"GROUP");
                    strcpy(message.msg,sendline);
                    send(sockfd,&message,sizeof(message),0);
                    break;
                }    
            }
            else
            {
                printf("你已经被管理员禁言\n");
            }
            break;
        //踢人
        case 6:
            if(root == 1)
            {
                printf("请输入你要强制下线用户的id\n");
                scanf("%s",message.id);
                strcpy(message.cmd,"KICK");
                send(sockfd,&message,sizeof(message),0);
            }
            else
            {
                printf("你还不是管理员身份，不能使用该功能\n");
            }
            break;
        //禁言
        case 7:
            if(root == 1)
            {
                //msg_text.admin_flag = admin_flag;
                
                printf("需要禁言用户的id\n");
                scanf("%s",message.id);
                strcpy(message.cmd,"EXCUSE");
                send(sockfd,&message,sizeof(message),0);
            }
            else
            {
                printf("你还不是管理员身份，不能使用该功能\n");
            }
            break;
        //解除禁言
        case 8:
            if(root == 1)
            {
                printf("需要解禁言用户的id\n");
                scanf("%s",message.id);
                strcpy(message.cmd,"WORLD");
                send(sockfd,&message,sizeof(message),0);
            }
            else
            {
                printf("你还不是管理员身份，不能使用该功能\n");
            }
            break;
        //忘记密码
        case 9:
            printf("请输入id:\n");
            scanf("%s",message.id); 
            getchar();  
            printf("请输入密保:\n");
            scanf("%s",message.secret);
            getchar();
            printf("请输入新密码:\n");
            scanf("%s",message.passwd);
            getchar();
            strcpy(message.cmd,"FORGET");
            printf("正在更改密码，请稍后......\n");
            sleep(1);
            send(sockfd,&message,sizeof(message),0);
            sleep(1);
            system("clear");
            break;
        //查看在线人数
        case 10:
            strcpy(message.cmd,"LOOKUSERS");
            send(sockfd,&message,sizeof(message),0); 
            sleep(1);         
            break;
        //申请管理员身份
        case 11:
            if(root == 1)
            {
                printf("你已经是管理员\n");
            } 
            else
            {
                strcpy(message.cmd,"ADMINISTRATOR");
                send(sockfd,&message,sizeof(message),0); 
                printf("正在等待服务器响应...........\n"); 
            } 
            sleep(2);
            system("clear"); 
            break;
        //取消管理员身份
        case 12:
            if(root == 0)
            {
                printf("你还不是管理员\n");
            }
            else
            { 
                strcpy(message.cmd,"OUTADMINISTRATOR");
                send(sockfd,&message,sizeof(message),0);
                printf("正在等待服务器响应...........\n"); 
            }
            sleep(1);
            system("clear");  
            break;
        //注销账户
        case 13:
            printf("你确定要注销账户吗？\n"); 
            printf("1 : 确定  2 : 取消 \n");
            scanf("%d", &a); 
            if(a == 1)
            {
                printf("请输入id:\n");
                scanf("%s",message.id);
                getchar();   
                printf("请输入密保:\n");
                scanf("%s",message.secret);
                getchar();
                strcpy(message.cmd,"KILLUSER");
                printf("正在注销账户，请稍后......\n");
                sleep(1);
                send(sockfd,&message,sizeof(message),0);
                sleep(1);
            }
            else
            {
                break;

            } 
            break;
        case 14:
            strcpy(message.cmd,"LOOKCHATRECORD"); 
            send(sockfd,&message,sizeof(message),0);
            break;          
        //退出聊天室
        case 00:
            system("clear"); 
            //sleep(1);
            printf("退出成功!\n");
            printf("欢迎下次使用！\n");
            exit(-1);
            break;
        default:
            printf("选择有误！请重新输入！\n");
            break;
        
        }
    }
}
