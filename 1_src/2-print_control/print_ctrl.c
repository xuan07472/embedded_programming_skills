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
#include "publicdef.h"
#include "print_ctrl.h"

inline int pr(int tag, ...)
{
//va_start va_arg va_end 
    if (tag <= PRINT_LEVEL) {
        va_list valist;
        time_t timep;
        time(&timep);
        va_start(valist, tag);
        printf("[%s]{%s }", name2str(tag), ctime(&timep));
        printf(valist);
        printf("\n");
    }
}

/*!
 * \brief 调试输出单元测试用例
 */
#ifdef PRINT_CTRL_MODULE_UNITEST
int print_ctrl_unitest()
{
    int i;

    pr_entry(in);

    pr(INFO, "info string %s", "test");
    pr(DEBUG, "debug string %s", "test");
    pr_err("pure err string");
    pr_debug("debug string%s\n", "test");

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
