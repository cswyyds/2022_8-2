#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>

//宏定义
//每次使用检查日期是否跟今天的不一样
#define NOWYEAR 2022  //今天的年份 
#define MONTH 7       //月
#define DAY 3         //日 


//计算星座
char * Constellation(int a2);
//计算属相
char *ShuXiang(int year);
//最配星座
char *MaxCon(char *con);
//判断那天是星期几
char *Week(int year , int month , int day);
//星座对应的守护星
char *GuardianPlanet(char *con);

//计算天数
int AddDay2(int birthYear, int birthMouth, int birthDay, int nowYear, int nowMouth, int nowDay);
 
int main(int argc, char const *argv[])
{
    system("clear");
    printf("欢迎使用年龄计算器\n");
    sleep(1);
    int now_year = NOWYEAR;  //今天的年份
    int now_month = MONTH;    //今天的月份
    int now_day = DAY;    
    

    int age , birthyear , birthmonth , birthday;
    printf("请输入你的出生年月日\n");
    scanf("%d %d %d", &birthyear,&birthmonth,&birthday);
    
    //周岁虚岁
    int a1 = (now_month * 100) + now_day;
    int a2 = (birthmonth *100) + birthday;
    int xusui;
    //在本人生日到来之前，虚岁=周岁 + 2;
    //在本人生日到来之后，虚岁=周岁 + 1;
    if(a1 >= a2)
    {
        age = (now_year - birthyear);
        xusui = age + 1;
    }
    else if(a1 < a2)
    {
        age = (now_year - birthyear) -1;
        xusui = age + 2;
    }

    

    //星座
    char *con;
    con = Constellation(a2);

    //属相
    char *sx;
    sx = ShuXiang(birthyear);

    //最佳星座
    char *maxcon;
    maxcon = MaxCon(con);

    //计算天数
    int days = AddDay2(birthyear, birthmonth, birthday, now_year, now_month, now_day);
    
    //离下一个生日还有多少天
    int d = 0;
    //判断那天是星期几
    char *week;
    //判断守护星球
    char *planet;
    planet = GuardianPlanet(con);
    //下一个生日年份
    int will_year;
    if(a1 >a2)
    {
        d = AddDay2(now_year, now_month, now_day,now_year + 1, birthmonth, birthday);
        week = Week(now_year + 1, birthmonth, birthday);
        will_year = now_year + 1;
    }
    else if(a1 < a2)
    {
        d = AddDay2(now_year,now_month, now_day,now_year, birthmonth, birthday);
        week = Week(now_year, birthmonth, birthday);
        will_year = now_year;
    }

    printf("**********************查询结果如下***********************\n\n");
    printf("      生肖: %s  (按农历年份计算)\n\n",sx);
    printf("      星座: %s  \n\n",con);
    printf("      周岁：%d  (每过一个公历生日一岁)\n\n", age);
    printf("      虚岁：%d  (出生即一岁，每过一个春节长一岁)\n\n", xusui);
    printf("      生日: %d年%d月%d日\n\n",birthyear,birthmonth,birthday);
    printf("      您在这个世界上生活了:%d天\n\n",days + 1);
    printf("      另一半的最佳星座: %s  \n\n",maxcon);
    //printf("      %s的守护星球: %s  \n\n",con ,planet);
    printf("      下个生日: %d-%d-%d，还有%d天， 那天是 %s，\n\n                那天你就 %d 周岁了\n\n",will_year,birthmonth, birthday,d,week,age +1);
    printf("********************************************************\n\n");

    //让定义的指针指向空，防止成为野指针;
    con = NULL;
    sx = NULL;
    maxcon = NULL;
    week = NULL;
    planet = NULL;
    //free释放，防止内存泄漏
    free(con);
    free(sx);
    free(maxcon);
    free(planet);

    return 0;
}

//计算星座
char *Constellation(int a2)
{

    char *con;
    con = (char *)malloc(32);
    if(a2>=121 && a2<=219)
    {
        strcpy(con,"水瓶座");
        return con;
    }
    if(a2>=220 && a2<=320)
    {
        strcpy(con,"双鱼座");
        return con;
    }
    if(a2>=321 && a2<=420)
    {
        strcpy(con,"白羊座");
        return con;
    }
    if(a2>=421 && a2<=521)
    {
        strcpy(con,"金牛座");
        return con;
    }
    if(a2>=522 && a2<=621)
    {
        strcpy(con,"双子座");
        return con;
    }
    if(a2>=622 && a2<=722)
    {
        strcpy(con,"巨蟹座");
        return con;
    }
    if(a2>=723 && a2<=823)
    {
        strcpy(con,"狮子座");
        return con;
    }
    if(a2>=824 && a2<=923)
    {
        strcpy(con,"处女座");
        return con;
    }
    if(a2>=924 && a2<=1023)
    {
        strcpy(con,"天秤座");
        return con;
    }
    if(a2>=1024 && a2<=1122)
    {
        strcpy(con,"天蝎座");
        return con;
    }
    if(a2>=1123 && a2<=1221)
    {
        strcpy(con,"射手座");
        return con;
    }
    else
    {
        strcpy(con,"摩羯座");
        return con;

    }
}

//计算属相
char *ShuXiang(int year)
{
    int y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12;
    
    y1 = year - 1924;  //1924鼠年
    y2 = year - 1925;
    y3 = year - 1926;
    y4 = year - 1927;
    y5 = year - 1928;
    y6 = year - 1929;
    y7 = year - 1930;
    y8 = year - 1931;
    y9 = year - 1932;
    y10 = year - 1933;
    y11 = year - 1934;
    y12 = year - 1935;  //1935猪年
    

    char *sx;
    sx = (char *)malloc(32);
    if(y1%12 == 0)
    {
        strcpy(sx,"鼠");
        return sx;
    }
    if(y2%12 == 0)
    {
        strcpy(sx,"牛");
        return sx;
    }
    if(y3%12 == 0)
    {
        strcpy(sx,"虎");
        return sx;
    }
    if(y4%12 == 0)
    {
        strcpy(sx,"兔");
        return sx;
    }
    if(y5%12 == 0)
    {
        strcpy(sx,"龙");
        return sx;
    }
    if(y6%12 == 0)
    {
        strcpy(sx,"蛇");
        return sx;
    }
    if(y7%12 == 0)
    {
        strcpy(sx,"马");
        return sx;
    }
    if(y8%12 == 0)
    {
        strcpy(sx,"羊");
        return sx;
    }
    if(y9%12 == 0)
    {
        strcpy(sx,"猴");
        return sx;
    }
    if(y10%12 == 0)
    {
        strcpy(sx,"鸡");
        return sx;
    }
    if(y11%12 == 0)
    {
        strcpy(sx,"狗");
        return sx;
    }
    if(y12%12 == 0)
    {
        strcpy(sx,"猪");
        return sx;
    }  
}

//最配星座
char *MaxCon(char *con)
{
    int y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12;

    y1 = strcmp(con,"白羊座");
    y2 = strcmp(con,"金牛座");
    y3 = strcmp(con,"双子座");
    y4 = strcmp(con,"巨蟹座");
    y5 = strcmp(con,"狮子座");
    y6 = strcmp(con,"处女座");
    y7 = strcmp(con,"天秤座");
    y8 = strcmp(con,"天蝎座");
    y9 = strcmp(con,"射手座");
    y10 = strcmp(con,"摩羯座");
    y11 = strcmp(con,"水瓶座");
    y12 = strcmp(con,"双鱼座");

    
    
    char *cons;
    cons = (char *)malloc(32);
    
    if(y1 == 0)
    {
        strcpy(cons,"狮子座");
        return cons;
    }
    if(y2 == 0)
    {
        strcpy(cons,"摩羯座");
        return cons;
    }
    if(y3 == 0)
    {
        strcpy(cons,"天秤座");
        return cons;
    }
    if(y4 == 0)
    {
        strcpy(cons,"双鱼座");
        return cons;
    }
    if(y5 == 0)
    {
        strcpy(cons,"白羊座");
        return cons;
    }
    if(y6 == 0)
    {
        strcpy(cons,"金牛座");
        return cons;
    }
    if(y7 == 0)
    {
        strcpy(cons,"双子座");
        return cons;
    }
    if(y8 == 0)
    {
        strcpy(cons,"巨蟹座");
        return cons;
    }
    if(y9 == 0)
    {
        strcpy(cons,"白羊座");
        return cons;
    }
    if(y10 == 0)
    {
        strcpy(cons,"金牛座");
        return cons;
    }
    if(y11 == 0)
    {
        strcpy(cons,"双子座");
        return cons;
    }
    if(y12 == 0)
    {
        strcpy(cons,"巨蟹座");
        return cons;
    }

}

//判断是否是闰年，是则返回1，否返回0
int LeapYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//用来计算没有过完的年已经过了的天数
int AddDay1(int year, int mouth, int day)
{
    int s = 0;
    int mouths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    mouths[1] += LeapYear(year); //如果是闰年二月天数加一
    for (int i = 0; i < mouth - 1; i++) 
    {
        s += mouths[i];
    }
    return s + day;
}

//计算总天数
int AddDay2(int birthYear, int birthMouth, int birthDay, int nowYear, int nowMouth, int nowDay)
{
    int d = 0;
    
    //先将出生那一年当一个整年来算，现在这一年不算
    for (int i = birthYear; i < nowYear; i++) 
    {
        d += 365 + LeapYear(i);
    }
    d += AddDay1(nowYear, nowMouth, nowDay); //加上现在这一年的天数
    d -= AddDay1(birthYear, birthMouth, birthDay); //减去出生那年出生日期之前的天数
    return d;
}
//判断那天是星期几
char * Week(int year , int month , int day) 
{
    int iWeekDay = -1; 
    if (1 == month || 2 == month) 
    {   
        month += 12; 
        year--;
    }   
    iWeekDay = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
    switch(iWeekDay)
    {   
        case 0 : return "星期日"; break;
        case 1 : return "星期一"; break;
        case 2 : return "星期二"; break;
        case 3 : return "星期三"; break;
        case 4 : return "星期四"; break;
        case 5 : return "星期五"; break;                                                             
        case 6 : return "星期六"; break;
        default : return NULL; break;
    }   

    return NULL;
}


//十二星座的守护星分别是：白羊座，火星；金牛座，金星；双子座，水星；巨蟹座，月亮；狮子座，太阳；处女座，水星；
//秤座，金星；蝎座，冥王星、火星；射手座，木星；摩羯座，土星；水瓶座，王星、土星；双鱼座，海王星、木星。
//星座对应的守护星
char *GuardianPlanet(char *con)
{
    int y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12;

    y1 = strcmp(con,"白羊座");
    y2 = strcmp(con,"金牛座");
    y3 = strcmp(con,"双子座");
    y4 = strcmp(con,"巨蟹座");
    y5 = strcmp(con,"狮子座");
    y6 = strcmp(con,"处女座");
    y7 = strcmp(con,"天秤座");
    y8 = strcmp(con,"天蝎座");
    y9 = strcmp(con,"射手座");
    y10 = strcmp(con,"摩羯座");
    y11 = strcmp(con,"水瓶座");
    y12 = strcmp(con,"双鱼座");

    
    
    char *cons;
    cons = (char *)malloc(32);
    
    if(y1 == 0)
    {
        strcpy(cons,"火星");
        return cons;
    }
    if(y2 == 0)
    {
        strcpy(cons,"金星");
        return cons;
    }
    if(y3 == 0)
    {
        strcpy(cons,"水星");
        return cons;
    }
    if(y4 == 0)
    {
        strcpy(cons,"月亮");
        return cons;
    }
    if(y5 == 0)
    {
        strcpy(cons,"太阳");
        return cons;
    }
    if(y6 == 0)
    {
        strcpy(cons,"水星");
        return cons;
    }
    if(y7 == 0)
    {
        strcpy(cons,"金星");
        return cons;
    }
    if(y8 == 0)
    {
        strcpy(cons,"冥王星 火星");
        return cons;
    }
    if(y9 == 0)
    {
        strcpy(cons,"木星");
        return cons;
    }
    if(y10 == 0)
    {
        strcpy(cons,"土星");
        return cons;
    }
    if(y11 == 0)
    {
        strcpy(cons,"王星、土星");
        return cons;
    }
    if(y12 == 0)
    {
        strcpy(cons,"海王星 木星");
        return cons;
    }

}
