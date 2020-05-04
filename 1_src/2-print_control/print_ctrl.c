/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    print_ctrl.c
 * \brief   调试输出级别控制模块单元测试用例
 * \author  将狼才鲸
 * \version 1.0.0
 * \date    2020-05-02
 * \license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 * 备注：利用宏定义函数# ## ... __VA_ARGS__等技巧实现
 *
 * -----------------------------------------------------------------------------
 * 文件修改历史：
 * <时间>       | <版本>    | <作者>    | <描述>
 * 2020-05-02   | v1.0.0    | 将狼才鲸  | 创建文件
 * -----------------------------------------------------------------------------
 ******************************************************************************/

/*================================= 头 文 件 =================================*/
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "publicdef.h"
#include "print_ctrl.h"

/*!
 * \brief 调试输出接口
 *
 *        备注：还有种方法，用一个缓存使用snprintf实现，这样既可以控制输出，还可以方便存日志，这种方法后续再做
 */
int pr(int tag, char *tagstr, char *fmt, ...)
{
    if (tag <= PRINT_LEVEL) {
        va_list valist; /**< ...表示的可变参数列表 */

        /** 获取当前时间 */
        //ctime和asctime获取的时间字符串会在末尾加上换行，用不上，这里使用localtime
        time_t timep;
        struct tm *p;
        time(&timep);
        p = localtime(&timep);

        va_start(valist, fmt); /**< 找到参数的起始位置 */
        printf("[%s]{%04d-%02d-%02d %02d:%02d:%02d} ",
                tagstr, 1900 + p->tm_year,
                1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec);
        vprintf(fmt, valist); /* 这里必须用vprintf */
        va_end(valist);
        printf("\n");
    }
}

/*!
 * \brief 调试信息加上级别、时间、文件名、函数名、文件行、换行信息
 */
int pr_func(int tag, char *tagstr, char *filestr, char *funcstr, int line, char *fmt, ...)
{
    if (tag <= PRINT_LEVEL) {
        va_list valist; /**< ...表示的可变参数列表 */

        /** 获取当前时间 */
        //ctime和asctime获取的时间字符串会在末尾加上换行，用不上，这里使用localtime
        time_t timep;
        struct tm *p;
        time(&timep);
        p = localtime(&timep);

        va_start(valist, fmt); /**< 找到参数的起始位置 */
        printf("[%s]{%04d-%02d-%02d %02d:%02d:%02d}<%s %s() line:%d> ",
                tagstr, 1900 + p->tm_year,
                1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec,
                filestr, funcstr, line);
        vprintf(fmt, valist); /* 这里必须用vprintf */
        va_end(valist);
        printf("\n");
    }
}

#define prr_err(tag, tagstr, ...) \
    pr_func((tag), tagstr, __FILE__, __func__, __LINE__, __VA_ARGS__)
/*!
 * \brief 调试输出单元测试用例
 */
#ifdef PRINT_CTRL_MODULE_UNITEST
int print_ctrl_unitest()
{
    int i;
int in = 1, out = 2;
//    pr_entry(in);

    pr_func(ERROR, name2str(INFO), __FILE__, __func__, __LINE__, "aaaaaaaaa");
    prr_err(INFO, name2str(INFO), "aaaaaaaaa");
    pr(INFO, name2str(INFO), "int: %d", 100);
    pr(INFO, name2str(INFO), "info string %s", "test");
//    pr(DEBUG, "debug string %s", "test");
//    pr_err("pure err string");
//    pr_debug("debug string%s\n", "test");

#if 0
    for (i = 0; i < 100; i++)
        pr_info_pure("%d ", i);
    pr_info_pure("\n");

    for (i = 0; i < 100; i++)
        pr_debug_pure("%d ", i);
    pr_debug_pure("\n");
#endif
//    pr_entry(out);

    return 0;
}

/* 如果不是所有模块一起做单元测试，则需要自己定义main函数入口 */
#ifndef ALL_MODULES_UNITEST
int main()
{
    return print_ctrl_unitest();
}
#endif /* ALL_MODULES_UNITEST */

#endif /* PRINT_CTRL_MODULE_UNITEST */
