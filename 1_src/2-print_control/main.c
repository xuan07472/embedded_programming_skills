#if 0
#include <stdio.h>
#include <string.h>
#include <time.h> 
  
int main() 
{   
    time_t timep;   
    time(&timep);
   
    printf("%s\n", asctime(gmtime(&timep)));   
    
    return 0;
}
#endif
#if 0
#include <stdio.h>
#include <string.h>
#include <time.h>   

int main(void)   
{   
    time_t timep;
 
    time(&timep);   
    printf("%s\n", ctime(&timep));
    
    return 0;  
}
#endif
#if 0
#include <stdio.h>
#include <stdarg.h>
double average(int num,...)
{
        va_list valist;
        double sum = 0.0;
        int i; /* 为 num 个参数初始化 valist */ 
        va_start(valist, num); /* 访问所有赋给 valist 的参数 */
        for (i = 0; i < num; i++)
        {
                sum += va_arg(valist, int);
        }
        /* 清理为 valist 保留的内存 */
        va_end(valist);
        return sum/num;
}

int main()
{
        printf("Average of 2, 3, 4, 5 = %f\n", average(4, 2,3,4,5));
        printf("Average of 5, 10, 15 = %f\n", average(3, 5,10,15));
}
#endif

#if 1
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#define name2str(x) #x
#define PRINT_LEVEL 3
#define INFO  3
#define ERROR 1
void test(char *fmt, ...)
{
    va_list valist;

    va_start(valist, fmt); /* 访问所有赋给 valist 的参数 */
    vprintf(fmt, valist);
    va_end(valist);
}

int pr(int tag, char *tagstr, char *fmt, ...)
{
//va_start va_arg va_end 
    if (tag <= PRINT_LEVEL) {
        va_list valist;
        time_t timep;
        struct tm *p; 
        time(&timep);
        p = localtime(&timep);
        va_start(valist,fmt);
        //ctime和asctime获取的字符串会在末尾加上换行，用不上
        //printf("[%s]{%s }", name2str(tag), ctime(&timep));
        if (tag <= ERROR)
            printf("[%s]{%04d-%02d-%02d %02d:%02d:%02d} ",
                tagstr, 1900 + p->tm_year,
                1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec);
        else
            printf("[%s]{%04d-%02d-%02d %02d:%02d:%02d} ",
                tagstr, 1900 + p->tm_year,
                1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec);
        vprintf(fmt, valist);
        printf("\n");
    }
}

#define pr_err(x,y,...) pr((x),(y), "()"__VA_ARGS__)
//__func__和__LINE__不能在宏定义函数中拼接，只能作为单个参数
#define pr_test(x) printf("%s %d\n", __DATE__ __TIME__, (x))
int main()
{
    test("this is a %s, int: %d, float: %f\n",
         "test string", 10, 5.2);
    //pr(INFO, name2str(INFO),"test string");
    //pr_err(ERROR, name2str(ERROR), __func__"()line:"__LINE__"TEST STRING");
    //pr_err(ERROR, name2str(ERROR), "TEST STRING");
//    pr_err(ERROR, name2str(ERROR), "TEST STRING %s %d",  __func__, __LINE__);
    //printf("%s %d\n", __func__, __LINE__);
    //pr_test(11);
}
#endif

#if 0
#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
    time_t timep;   
    struct tm *p; 
   
    time(&timep);   
    p = localtime(&timep);
   
    printf("%d-%d-%d %d:%d:%d\n", (1900 + p->tm_year), ( 1 + p->tm_mon), p->tm_mday,
                                (p->tm_hour + 12), p->tm_min, p->tm_sec); 

    return 0;
}
#endif

#if 0
#include <stdio.h>
#include <string.h>
#include <time.h> 
  
int main() 
{   
    time_t timep;   
    time(&timep);
   
    printf("%s", asctime(gmtime(&timep)));   
    printf("====");    
    return 0;
}
#endif

#if 0
#include <stdio.h>
#include <string.h>
#include <time.h>   

int main(void)   
{   
    time_t timep;
 
    time(&timep);   
    printf("%s", ctime(&timep));
    printf("====\n");   
    return 0;  
}
#endif

#if 0
#include <stdio.h>
#include <time.h>

int main()
{
struct tm tm_time;
strptime("2010-11-15 10:39:30", "%Y-%m-%d %H:%M:%S", &tm_time);
printf("%ld/n", mktime(&tm_time));
printf("-------------------------------------/n");

char szBuf[256] = {0};
time_t timer = time(NULL);
strftime(szBuf, sizeof(szBuf), "%Y-%m-%d %H:%M:%S", localtime(&timer));
printf("%s/n", szBuf);
return 0;
}
#endif
