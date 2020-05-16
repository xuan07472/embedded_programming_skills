/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    menu.c
 * \brief   字符界面菜单模块
 * \author  将狼才鲸
 * \version 1.0.0
 * \date    2020-05-01
 * \license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 * 备注：利用printf和scanf实现多级菜单，用于用户交互，方便在任意位置增加删除
 *       子条目
 *
 * -----------------------------------------------------------------------------
 * 文件修改历史：
 * <时间>       | <版本>    | <作者>    | <描述>
 * 2020-05-01   | v1.0.0    | 将狼才鲸  | 创建文件
 * -----------------------------------------------------------------------------
 ******************************************************************************/

/*================================= 头 文 件 =================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "print_ctrl.h"
#include "menu.h"

/*================================= 宏 定 义 =================================*/
//#define pr_info_pure printf     /* 先临时定义print_ctrl.h中的函数，后续再删除 */
/** 返回上级菜单的菜单项内容 */
#define MENU_GO_UP                          \
        {                                   \
            .id = 0,                        \
            .text_info = "返回上级菜单",    \
            .next = NULL,                   \
            .func = NULL                    \
        }

/*================================= 全局变量 =================================*/
static int menu_exit();

static char ibuf[TEXT_INFO_LEN];/**< 用户输入缓存，防止输入非法字符时的bug */
/*! 实际的菜单项 */
static MENU_T main_menu_exit = {
    .id         = 0,
    .level      = 1,
    .text_info  = "退出程序",
    .next       = NULL,
    .sub_menus  = NULL,
    .func       = menu_exit
};
static MENU_T menu2_3 = {
    .id         = 3,
    .text_info  = "menu2_3",
};
static MENU_T menu2_2 = {
    .id         = 2,
    .text_info  = "menu2_2",
};
static MENU_T menu2_1 = {
    .id         = 1,
    .text_info  = "menu2_1",
};
static MENU_T menu3_3 = {
    .id         = 3,
    .text_info  = "menu3_3",
};
static MENU_T menu3_2 = {
    .id         = 2,
    .text_info  = "menu3_2",
};
static MENU_T menu3_1 = {
    .id         = 1,
    .text_info  = "menu3_1",
};
static MENU_T menu1_3 = {
    .id         = 3,
    .text_info  = "menu1_3",
};
static MENU_T menu1_2 = {
    .id         = 2,
    .text_info  = "menu1_2",
};
static MENU_T menu1_1 = {
    .id         = 1,
    .text_info  = "menu1_1",
};
static MENU_T menu3 = {
    .id         = 3,
    .text_info  = "[第一层][第三项]",
};
static MENU_T menu2 = {
    .id         = 2,
    .text_info  = "[第一层][第二项]",
};
static MENU_T menu1 = {
    .id         = 1,
    .text_info  = "[第一层][第一项]",
};
static MENU_T menu1_x_goup = MENU_GO_UP;
static MENU_T menu2_x_goup = MENU_GO_UP;
static MENU_T menu3_x_goup = MENU_GO_UP;

/*================================= 私有函数 =================================*/
/*!
 * \brief 以下是底层菜单的执行函数
 */
static int func_menu1_1(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu1_2(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu1_3(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu2_1(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu2_2(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu2_3(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu3_1(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu3_2(void) {pr_info_pure("执行完%s\n", __func__);}
static int func_menu3_3(void) {pr_info_pure("执行完%s\n", __func__);}
static int menu_exit(void) {pr_info_pure("退出菜单......\n"); exit(0);}

/*================================= 接口函数 =================================*/
/*!
 * \brief 打印某一级菜单里面所有菜单项的ID和输出信息
 */
int menu_display(MENU_T *menus)
{
    MENU_T *menu;

    pr_info_pure(" ______________________________\n");

    if (menus)
        pr_info_pure("|~~~~~~~~~~ %d级菜单 ~~~~~~~~~~~\n", menus->level);
    for (menu = menus; menu; menu = menu->next)
        pr_info_pure("| %d.\t%s\n", menu->id, menu->text_info);

    pr_info_pure(" ------------------------------\n");

    return 0;
}

/*!
 * \brief 建立起完整的菜单链表
 */
MENU_T *menu_init(void)
{
    MENU_T *main_menu = &menu1;

    /** 第一级菜单指针建立 */
    main_menu->next = &menu2;
    menu2.next = &menu3;
    menu3.next = &main_menu_exit;

    menu1.level = 1;
    menu1.func = NULL;
    menu1.sub_menus = &menu1_1;

    menu2.level = 1;
    menu2.func = NULL;
    menu2.sub_menus = &menu2_1;

    menu3.level = 1;
    menu3.func = NULL;
    menu3.sub_menus = &menu3_1;

    /** 第二级菜单指针建立 */
    menu1_1.level = 2;
    menu1_1.next = &menu1_2;
    menu1_2.next = &menu1_3;
    menu1_3.next = &menu1_x_goup;
    menu1_x_goup.sub_menus = main_menu;

    menu2_1.level = 2;
    menu2_1.next = &menu2_2;
    menu2_2.next = &menu2_3;
    menu2_3.next = &menu2_x_goup;
    menu2_x_goup.sub_menus = main_menu;

    menu3_1.level = 2;
    menu3_1.next = &menu3_2;
    menu3_2.next = &menu3_3;
    menu3_3.next = &menu3_x_goup;
    menu3_x_goup.sub_menus = main_menu;

    /** 末级菜单执行执行函数 */
    menu1_1.func = func_menu1_1;
    menu1_1.sub_menus = &menu1_1;
    menu1_2.func = func_menu1_2;
    menu1_2.sub_menus = &menu1_1;
    menu1_3.func = func_menu1_3;
    menu1_3.sub_menus = &menu1_1;
    menu2_1.func = func_menu2_1;
    menu2_1.sub_menus = &menu2_1;
    menu2_2.func = func_menu2_2;
    menu2_2.sub_menus = &menu2_1;
    menu2_3.func = func_menu2_3;
    menu2_3.sub_menus = &menu2_1;
    menu3_1.func = func_menu3_1;
    menu3_1.sub_menus = &menu3_1;
    menu3_2.func = func_menu3_2;
    menu3_2.sub_menus = &menu3_1;
    menu3_3.func = func_menu3_3;
    menu3_3.sub_menus = &menu3_1;

    return main_menu;
}

/*!
 * \brief 进入一个子菜单或者执行末级菜单的程序
 */
MENU_T *menu_enter(MENU_T *menu, int id)
{
    MENU_T *m;

    for (m = menu; m; m = m->next) {
        if (m->id == id)
            break;
    }

    if (!m) {
        pr_info_pure("!该序号%d不存在! 请重新输入......\n", id);
        return menu;
    }

    if (m->func) {
        pr_info_pure(" ```````` 开始执行程序 ````````\n\t");
        m->func();
        pr_info_pure(" .......... 执行完成 ..........\n\n");
    }

    /** 返回后会直接进入子菜单 */
    return m->sub_menus;
}

//#define MENU_MODULE_UNITEST /* 已经在publidef.h中定义 */
#ifdef  MENU_MODULE_UNITEST
/*!
 * \brief 单元测试程序
 */
int menu_unitest(void)
{
    int id = 0;
    MENU_T *menu;

    menu = menu_init();

    /*!
     * 循环展示一级菜单，等待选择菜单项，选定后进入子菜单或者执行菜单内容，然后继续循环
     */
    while (1) {
        menu_display(menu);

        pr_info_pure("请输入序号......\n");
        memset(ibuf, 0, sizeof(ibuf));
        scanf("%s", ibuf);
        if ((ibuf[0] < '0') || (ibuf[0] > '9')) {
            pr_info_pure("你输入了非数字: %s，请重新输入......\n", ibuf);
            continue;
        }
        id = atoi(ibuf); /**< 修正输入错误id时刷屏的bug  */
        pr_info_pure("你输入的id为: %d\n", id);
        pr_info_pure("\n");

        menu = menu_enter(menu, id);
    }
}

/* 如果不是所有模块一起做单元测试，则需要自己定义main函数入口 */
#ifndef ALL_MODULES_UNITEST
int main()
{
    return menu_unitest();
}
#endif /* ALL_MODULES_UNITEST */

#endif /* MENU_MODULE_UNITEST */
