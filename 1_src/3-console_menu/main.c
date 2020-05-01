#include <stdio.h>

/*!
 * \brief 调试输出级别
 */
typedef enum _print_level{
    ERROR,
    WARN,
    INFO,
    ENTRY,
    DEBUG
} PRINT_LEVEL_M;

#define PRINT_LEVEL     INFO
/*!
 * \brief   将变量名转成字符串的宏定义函数
 *          #是宏定义中转换字符串的用法
 */
#define name2str(name) (#name)

/*!
 * \brief   必须定义PRINT_LEVEL宏定义，否则编译阶段就直接报错退出
 */
#ifndef PRINT_LEVEL
#   error MUST Define PRINT_LEVEL!
#endif

/*!
 * \brief   带调试级别的，补全级别信息和时间信息、自动换行的打印输出宏函数
 *          ...作为宏函数的可变变量，使用时用__VA_ARGS__或者，##_VA_ARGS__
 *          类似的用法还有宏定义中##可以拼接两个变量名，将其变成一个字符串
 */
#define pr(tag, ...)    do {                                        \
            if (tag <= PRINT_LEVEL) {                               \
                printf("[%s]""{%s %s} ", #tag, __DATE__, __TIME__); \
                printf(__VA_ARGS__);                                \
                printf("\n");                                       \
            }                                                       \
        } while(0);
#define pr_err(fmt, ...) pr(ERROR, fmt, ##__VA_ARGS__);
int main()
{
    //pr("name:%s\n", "jim");
    pr(INFO, "name2str(name)aaabbbccc%s\n", "ddd");   
    pr(DEBUG, "name2str(name)aaabbbccc%s\n", "ddd");   
    pr_err("name2str(name)aaabbbccc%s\n", "ddd");   
    //pr_add_time_newline(INFO, "the string");   
}
//##__VA_ARGS__
