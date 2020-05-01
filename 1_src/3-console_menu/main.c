#include <stdio.h>

/*!
 * \brief 调试输出级别
 */
#define ERROR   1
#define WARN    2
#define INFO    3
#define ENTRY   4
#define DEBUG   5

#define PRINT_LEVEL     DEBUG    /**< 默认调试级别 */

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
 *
 *          ...作为宏函数的可变变量，使用时用__VA_ARGS__
 *          或者##_VA_ARGS__(这里的##意思是即使参数为空也正常执行)。
 *          另一种不同的用法是宏定义中##可以拼接两个变量名，将其变成一个字符串。
 *          printf的第一个参数可以用多个字符串拼起来，不加逗号，
 *          如printf("Th""is"" is"" a"" string");
 */
#define pr(tag, ...)    do {                                        \
            if (tag <= PRINT_LEVEL) {                               \
                printf("[" #tag "]" "{" __DATE__ __TIME__ "} "__VA_ARGS__); \
                printf("\n");                                       \
            }                                                       \
        } while(0);
#define pr_err(fmt, ...)            pr(ERROR, fmt, ##__VA_ARGS__);
#define pr_warn(fmt, ...)           pr(WARN, fmt, ##__VA_ARGS__);
#define pr_info(fmt, ...)           pr(INFO, fmt, ##__VA_ARGS__);
#define pr_entry(inout)             pr(ENTRY, "%s() %s", __func__, #inout);
#define pr_debug(fmt, ...)          pr(DEBUG, fmt, ##__VA_ARGS__);

/*!
 * \brief 带调试级别，但是不带任何额外输出信息的打印输出宏函数
 *        用于在循环中输出缓存等数据
 */
#define pr_pure(tag, ...)    do {                                        \
            if (tag <= PRINT_LEVEL)                                 \
                printf(__VA_ARGS__);                               \
        } while(0);
#define pr_err_pure(fmt, ...)            pr_pure(ERROR, fmt, ##__VA_ARGS__);
#define pr_warn_pure(fmt, ...)           pr_pure(WARN, fmt, ##__VA_ARGS__);
#define pr_info_pure(fmt, ...)           pr_pure(INFO, fmt, ##__VA_ARGS__);
#define pr_debug_pure(fmt, ...)          pr_pure(DEBUG, fmt, ##__VA_ARGS__);

/** TODO: 一种从元素地址倒推数据结构首地址的方法: offsetof typeof */

#ifdef PRINT_CTL_UNITEST
/*!
 * \brief 调试输出单元测试用例
 */
int main()
{
    int i;

    pr_entry(in);

    pr(INFO, "info string %s", "test");
    pr(DEBUG, "debug string %s", "test");
    pr_err("pure err string");
    pr_debug("debug string%s\n", "test");

    for (i = 0; i < 100; i++)
        pr_info_pure("%d", i);
    pr_info_pure("\n");

    for (i = 0; i < 100; i++)
        pr_debug_pure("%d", i);
    pr_debug_pure("\n");

    pr_entry(out);

    return 0;
}
#endif /* PRINT_CTL_UNITEST */
