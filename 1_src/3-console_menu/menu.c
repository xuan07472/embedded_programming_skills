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

/*!
 * \brief 调试输出级别
 */
typedef enum _print_level{
    LEVEL_ERROR,
    LEVEL_WARN,
    LEVEL_INFO,
    LEVEL_ENTRY,
    LEVEL_DEBUG
} PRINT_LEVEL_M;

#define TEXT_INFO_LEN   128     /**< 菜单信息最大长度 */
/** 不带调试级别tag和时间信息的字符串输出,自带换行 */
#define pr_no_tag(...)      do {    \
            printf("%c", '\n')                \
            while(0);
/** 调试输出 */
#define name2str(x) (#x)
#define pr_no_newline       printf
#define pr_err(...)         do {    \
                if (print_level > LEVEL_ERROR) {    \
                    printf("[%s %s]""%s", __DATE__, __TIME__,
                            name2str(LEVEL_ERROR)\
                           );                       \
                }                                   \
            } while (0);    

/*!
 * \brief 菜单结构体
 */
typedef struct _menu {
    int id;                         /**< 打印出的菜单序号 */
    char text_info[TEXT_INFO_LEN];  /**< 打印出的菜单信息 */
    struct _menu *next;             /**< 同一级菜单中下一条菜单指针 */
    struct _menu *sub_menus;        /**< 下一级菜单入口 */
    int (*func)(struct _menu *);   /**< 当前菜单的响应函数 */
} MENU_T;

int print_level = LEVEL_DEBUG;

/*!
 * \brief 打印一条菜单ID和输出信息
int menu_display








