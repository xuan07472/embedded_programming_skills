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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "publicdef.h"
//#include "print_ctrl.h"

#define TEXT_INFO_LEN   128     /**< 菜单信息最大长度 */
static char ibuf[TEXT_INFO_LEN];

/*!
 * \brief 菜单结构体
 */
typedef struct _menu {
    int id;                         /**< 打印出的菜单序号 */
    int level;                      /**< 菜单层级 */
    char text_info[TEXT_INFO_LEN];  /**< 打印出的菜单信息 */
    struct _menu *next;             /**< 同一级菜单中下一条菜单指针 */
    struct _menu *sub_menus;        /**< 下一级菜单入口 */
    int (*func)(struct _menu *);   /**< 当前菜单的响应函数 */
} MENU_T;

#define pr_info_pure printf

/*!
 * \brief 打印某一级菜单里面所有菜单项的ID和输出信息
 */
int menu_display(MENU_T *menus)
{
    MENU_T *menu;

    pr_info_pure(" ______________________________\n");
    if (menus)
        pr_info_pure("|~~~~~~~~~~ %d级菜单 ~~~~~~~~~~~\n", menus->level);
    for (menu = menus; menu; menu = menu->next) {
        pr_info_pure("| %d.\t%s\n", menu->id, menu->text_info);
    }

    pr_info_pure(" ------------------------------\n");

    return 0;
}
static MENU_T menu2_3 = {
    .id         = 3,
    .level      = 2,
    .text_info  = "menu2_3",
};
static MENU_T menu2_2 = {
    .id         = 2,
    .level      = 2,
    .text_info  = "menu2_2",
};
static MENU_T menu2_1 = {
    .id         = 1,
    .level      = 2,
    .text_info  = "menu2_1",
};

static MENU_T menu3_3 = {
    .id         = 3,
    .level      = 2,
    .text_info  = "menu3_3",
};
static MENU_T menu3_2 = {
    .id         = 2,
    .level      = 2,
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
int func_menu1_1(){printf("执行完%s\n", __func__);}
int func_menu1_2(){printf("执行完%s\n", __func__);}
int func_menu1_3(){printf("执行完%s\n", __func__);}
int func_menu2_1(){printf("执行完%s\n", __func__);}
int func_menu2_2(){printf("执行完%s\n", __func__);}
int func_menu2_3(){printf("执行完%s\n", __func__);}
int func_menu3_1(){printf("执行完%s\n", __func__);}
int func_menu3_2(){printf("执行完%s\n", __func__);}
int func_menu3_3(){printf("执行完%s\n", __func__);}
static int menu_exit(){exit(0);}
static MENU_T main_menu_exit = {
    .id         = 0,
    .level      = 1,
    .text_info  = "退出程序",
    .next       = NULL,
    .sub_menus  = NULL,
    .func       = menu_exit
};
#define MENU_GO_UP {.id = 0, .text_info = "返回上级菜单", .next = NULL, .func = NULL}
static MENU_T menu1_x_goup = MENU_GO_UP;
static MENU_T menu2_x_goup = MENU_GO_UP;
static MENU_T menu3_x_goup = MENU_GO_UP;

MENU_T *menu_init()
{
    MENU_T *main_menu = NULL;

    main_menu = &menu1;
    /** 第一级菜单指针建立 */
    menu1.next = &menu2;
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
    menu1_x_goup.sub_menus = &menu1;

    menu2_1.level = 2;
    menu2_1.next = &menu2_2;
    menu2_2.next = &menu2_3;
    menu2_3.next = &menu2_x_goup;
    menu2_x_goup.sub_menus = &menu1;

    menu3_1.level = 2;
    menu3_1.next = &menu3_2;
    menu3_2.next = &menu3_3;
    menu3_3.next = &menu3_x_goup;
    menu3_x_goup.sub_menus = &menu1;

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

MENU_T *menu_enter(MENU_T *menu, int id)
{
    MENU_T *m;

    for (m = menu; m; m = m->next) {
        if (m->id == id)
            break;
    }

    if (!m)
        return NULL;

    if (m->func) {
        pr_info_pure(" ```````` 开始执行程序 ````````\n\t");
        m->func(m);
        pr_info_pure(" .......... 执行完成 ..........\n");
    }

    return m->sub_menus;
}

#define CONSOLE_MENU_UNITEST
#ifdef  CONSOLE_MENU_UNITEST
int main()
{
    int id;
    MENU_T *menu;

    menu = menu_init();

    /*!
     * 循环展示一级菜单，等待选择菜单项，选定后进入子菜单或者执行菜单内容，然后继续循环
     */
    while (1) {
        menu_display(menu);
        printf("请输入序号:\n");
        scanf("%d", &id);
        printf("\n");
        menu = menu_enter(menu, id);
    }
}

#endif /* CONSOLE_MENU_UNITEST */
