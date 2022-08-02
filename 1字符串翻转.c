#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[])
{
    char s[32] = "123456789";
    //scanf("%s",s);
    int len = strlen(s);
    char *p = NULL;
    char *q = NULL;
    p = &s[0];
    q = &s[len -1];
    for(; p < q; p++,q--)
    {
        char temp = *p;
        *p = *q;
        *q = temp; 
    }
    printf("%s\n",s);
    return 0;
}
