/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    print_ctrl.c
 * \brief   调试输出控制模块
 * \author  将狼才鲸
 * \version 1.0.0
 * \date    2020-05-02
 * \license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 * 备注：利用宏定义函数# ## ... __VA_ARGS__ va_list等技巧实现
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

/*================================= 接口函数 =================================*/
/*!
 * \brief 调试输出接口
 *
 * 备注：还有另外一种方法，用一个缓存使用snprintf实现，这样既可以控制
 *       输出，还可以方便存日志，这种方法后续再做
 *
 * \param[in] tag       调试级别
 * \param[in] tagstr    调试级别字符串
 * \param[in] fmt, ...  printf的可变参数
 */
int pr(int tag, char *tagstr, char *fmt, ...)
{
    /* 将变量定义放在if里面，提高输出关闭时的执行效率 */
    if (tag <= PRINT_LEVEL) {
        va_list valist; /**< ...表示可变参数，用valist来取出可变参数 */

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

    return 0;
}

/*!
 * \brief 调试信息加上级别、时间、文件名、函数名、文件行、换行信息
 *
 *        因为__func__ __LINE__ __FILE__这几个变量不能和printf的fmt参数
 *        直接拼接起来，所以这里用参数传入
 *
 * \param[in] tag       调试级别
 * \param[in] tagstr    调试级别字符串
 * \param[in] filestr   文件名字符串
 * \param[in] funcstr   函数名字符串
 * \param[in] line      当前代码行数
 * \param[in] fmt, ...  printf的可变参数
 */
int pr_func(int tag, char *tagstr, char *filestr, const char *funcstr, int line, char *fmt, ...)
{
    if (tag <= PRINT_LEVEL) {
        va_list valist;

        time_t timep;
        struct tm *p;
        time(&timep);
        p = localtime(&timep);

        va_start(valist, fmt);
        printf("[%s]{%04d-%02d-%02d %02d:%02d:%02d}<%s %s() line:%d> ",
                tagstr, 1900 + p->tm_year,
                1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec,
                filestr, funcstr, line);
        vprintf(fmt, valist);
        va_end(valist);
        printf("\n");
    }

    return 0;
}

/*!
 * \brief 调试输出单元测试用例
 */
#ifdef PRINT_CTRL_MODULE_UNITEST
int print_ctrl_unitest()
{
    int i;
    pr_entry(in);

    pr_func(ERROR, name2str(ERROR), __FILE__, __func__, __LINE__, "err pr_func...");
    pr_err("err %s %d %f", "pr_err", 1024, 5.20);

    pr(WARN, name2str(WARN), "warn pr int: %d", 100);
    pr_warn("pr_warn %s", "test");

    pr(INFO, name2str(INFO), "int: %d", 100);
    pr(INFO, name2str(INFO), "info string %s", "test");

    pr(DEBUG, name2str(DEBUG), "pr debug string %s", "test");
    pr_debug("pr_debug string%s\n", "test");

    for (i = 0; i < 100; i++)
        pr_info_pure("%d ", i);
    pr_info_pure("\n");

    for (i = 0; i < 100; i++)
        pr_debug_pure("%d ", i);
    pr_debug_pure("\n");

    pr_entry(out);

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
