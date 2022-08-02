#include "tongxunlu.h"

int main(int argc, char const *argv[])
{
    system("clear");//清屏
    printf("欢迎使用通讯录!\n");
    sleep(1);

    int n = 0;//定义已保存联系人的数量
 
    n = read_file(n);   //把联系人的信息从文件读到结构体数组内
    
    while(1)
    {
        
        int choice = menu();
        switch (choice)
        {
        case 1:
            n = add_new_person(n);
            break;
        case 2:
            look_all_person(n);
            break;
        case 3:
            search_person(n);
            break;
        case 4:
            change_person_data(n);
            break;
        case 5:
            n = delete_person(n);
            break;
        case 6:
            n = delete_all_person(n);
            break;
        case 0:
            system("clear");
			printf("感谢使用，下次再见!\n");
            sleep(1);
			return 0;	
        default:
            break;
        }
    }
    return 0;
}
