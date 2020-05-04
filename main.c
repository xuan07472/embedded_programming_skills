/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    main.c
 * \brief   整个程序的入口
 * \author  将狼才鲸
 * \version 1.0.0
 * \date    2020-05-02
 * \license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 文件修改历史：
 * <时间>       | <版本>    | <作者>    | <描述>
 * 2020-05-02   | v1.0.0    | 将狼才鲸  | 创建文件
 * -----------------------------------------------------------------------------
 ******************************************************************************/

/*================================= 头 文 件 =================================*/
#include <stdio.h>
#include "publicdef.h"

#ifdef ALL_MODULES_UNITEST
#include "print_ctrl.h"
#include "menu.h"
int main()
{
#ifdef ALL_MODULES_UNITEST
    pr_info("ALL_MODULES_UNITEST defined, 运行所有模块单元测试用例\n");
#else
    printf("ALL_MODULES_UNITEST not define，不运行单元测试用例\n");
#endif

    print_ctrl_unitest();
    menu_unitest();

    return 0;
}
#endif /* ALL_MODULES_UNITEST */
