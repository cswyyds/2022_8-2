#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//冒泡排序
void BubSort(int a[],int len)
{
    for(int i = 0; i < len -1; i++)
    {
        for(int j = 0; j < len - i - 1; j++)
        {
            if(a[j] < a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j +1];
                a[j + 1] = tmp;
            }
        }
    }
}
//快速排序
void ChooseSort(int a[], int len)
{
    for(int i = 0; i < len; i++)
    {
        int min = a[i]; //最小初始值
        int index = i;  //最小初始下标
        for(int j = i + 1; j < len; j++)
        {
            if(a[j] < min)
            {
                min = a[j];
                index = j;
            }
        }
        if(min < a[i])
        {
            a[index] = a[i];
            a[i] = min;
        }
    }
}
//二分查找
int HalfFind(int a[],int len, int key)
{
    int low = 0;
    int high = len - 1;
    while(low < high)
    {
        int mid =(low + high);
        if(a[mid] < key)
        {
            low = mid + 1;
        }
        else if(a[mid] > key)
        {
            high = mid - 1;
        }
        else
        {
            return mid;
        }
    }
    return -1;
}
int main(int argc, char const *argv[])
{
    int a[10] = {0};
    srand((unsigned)time(NULL));//初始化随机数
    for(int i = 0; i < 10; i++)
    {
        a[i] = rand()%50 + 1;
        printf("%d\t",a[i]);
    }
    putchar(10);

    BubSort(a,10);//冒泡排序 从大到小
    //遍历
    for(int i = 0; i < 10; i++)
    {
        printf("%d\t",a[i]);
    }
    putchar(10);

    ChooseSort(a,10);  //快速排序 从小到大
    //遍历
    for(int i = 0; i < 10; i++)
    {
        printf("%d\t",a[i]);
    }
    putchar(10);

    printf("请输入要查找的元素\n");
    int t = 0;
    scanf("%d",&t);
    int ret = HalfFind(a,10,t);  //二分查找
    if(-1 == ret)
    {
        printf("查找失败\n");
    }
    else
    {
        printf("查找成功\n");
        printf("a[%d] = %d\n",ret,a[ret]);
    }  
    return 0;
}
