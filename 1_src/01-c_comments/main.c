/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * 版权描述信息略
 * 每个.c文件都要包含文件头注释信息
 *
 * @file    main.c
 * @brief   整个程序的入口
 * @author  将狼才鲸
 * @version 0.0.1
 * @date    2020-03-29
 * @license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 * 备注：一切从main开始，这一次主要展示注释规范
 * 文件头注释块开始和结束的*星号为80个字符宽，一是为了给代码换行做参考，代码尽量
 * 不要超过80个字符，除非遇到不能换行的字符串常量；二也是为了美观
 *
 * 参考资料：
 * 《Doxygen注释规范》
 * https://blog.csdn.net/qq_25779555/article/details/77737391
 *
 * -----------------------------------------------------------------------------
 * 文件修改历史：
 * <时间>       | <版本>    | <作者>    | <描述>
 * 2020-03-29   | v0.0.1    | 将狼才鲸  | 创建文件
 * -----------------------------------------------------------------------------
 ******************************************************************************/

/*=============================== 头文件包含 =================================*/
#include <stdio.h>

/*============================= 结构体联合体定义 =============================*/
/*!
 * 1、结构体和联合体都需要用typedef重定义
 * 2、原结构体/联合体命名用下划线+小驼峰命名法，
 *    加下划线是为了避免不经意间和其它全局变量发生冲突，重定义后的名字用全大写
 *    +下划线的方式，和宏定义的命名类似
 * 3、结构体/联合体定义时起始大括号放在行末，结束大括号放在行首，大括号和名称
 *    之间留一个空格
 */
typedef struct _optInfo {
    int opt;        /**< 每个结构体/联合体成员都要有注释 */
    char *pInfo;    /**< 程序传入的字符串参数 */
} OPT_INFO_S;   /**< 行尾注释用tab缩进，至少和要注释的元素留有一个空格 */

/*!
 * main函数参数的操作类型
 */
typedef enum _opt {
    OPT_NUMBER,     /**< 第一个enum如果不赋值则默认赋值0，占一个int的空间 */
    OPT_STRING = 1, /**< 之后的元素如果不赋值，则值自动加1 */
    OPT_NOTICY      /**< 最后一个元素结尾不要加逗号 */
} OPT_U;        /**< main函数参数的操作类型 */

/*============================= 宏定义/重定义 ================================*/
#define pr_info printf

#define MAX_NUM (5)     /**< main函数最多支持的参数个数 */
#define MIN_NUM (1)     /**< main函数最少需要的参数个数 */

#define ERR_PARAM (-1)  /**< 错误码：参数错误 */
#define ERR_OPTION (-2) /**< 错误码：操作错误 */

/*================================ 全局变量 ==================================*/
int g_curNum = 0;   /**< 当前的main函数参数个数 */

/*!
 * 不同的代码块之间，两个函数之间，都只空一行，不要空两行或以上
 */

/*================================ 接口函数 ==================================*/
/*******************************************************************************
 * @brief   主函数入口
 *          函数不同参数之间要有一个空格，且空格在逗号后面
 *          指针的*星号靠近变量名，而不是靠近类型名；函数命名使用小写+下划线的
 *          方式，如get_info()，不要使用驼峰命名法，如getInfo()或者GetInfo()
 *
 * @param   [in]argc    运行可执行程序时后面带的参数个数（其中第0个参数默认
 *              是程序的全名），[in]代表的是输出参数，[out]代表是输出参数
 *              [in out]代表既是输入又是输出参数
 *          [in]argv    字符串指针数组，程序执行传入的参数都是字符串形式，
 *              即使传入的是数字
 * @return  0代表返回成功，负数代表返回错误码
 ******************************************************************************/
int main(int argc, char *argv[])
{
    int i;
    /*!
     * 1、变量名不要使用中文拼音，不要一行定义多个变量，如int i, j, k ,tmp;
     * 2、不要使用无意义的单个字母或同一个字母叠写，如int a; int aaa; 但用在for
     *    或者while循环内的局部循环变量i、j、k是允许的
     * 3、不要使用不常用的缩写，如fui、pfa，而tmp、isr、fd、xmt、tx、rx等这些
     *    约定俗成的除外；变量命名使用小驼峰命名法或者匈牙利命名法
     *    （小写属性+下划线+小驼峰命名法）
     *    不要使用大驼峰命名法（帕斯卡命名法）或者全小写/全大写+下划线的
     *    命名方法
     */
    int opt;                /**< 同一块注释尽量保持对齐 */
    char *p_noticeInfo;     /**< 匈牙利命名法，具体的属性代号可上网搜索 */

    /*! 变量定义和实际处理代码之间要留一个空行 */
    /*! 注释使用斜杠星号星号斜杠的方式，不要使用两斜杠 */
    /*! 注释与两边的星号斜杠间要各留一个空格，注释末尾不用添加句号 */

    /*! 每个函数的第一件事要对所有输入参数要做参数检查 */
    /*!
     * 1、if else while等关键字与后面小括号留一个空格，不允许if()
     * 2、+ - = ==等双目操作符两边必须留一个空格，不允许int i=0;
     * 3、小括号里面不要和表达式留有空格，不允许if ( a < 0 )
     * 4、if else while for等所有接数据块的表达式后面的内容必须用大括号扩
     *    扩起来，即使只有一条语句，不允许if (TRUE) return 0;
     * 5、起始大括号和结束大括号都独占一行
     * 6、不要在任何行尾留有空格或tab
     * 7、编辑器要设置成等宽字体，方便对齐和确定每行不超过80个字符
     */
    if (argc <= 1)
    {
        /*!
         * 1、任何时候不要直接在代码中使用printf或者printk等输出语句，
         *    要使用带输出级别控制的输出语句，可以是操作系统自带的，也可以
         *    是自己单独写的一个输出控制模块
         * 2、输出字符串末尾不要带句号，输出信息不要有中文
         * 3、语句中, 逗号后要有一个空格
         */
        pr_info("Error: Using %s -o option -f string\n", argv[0]);

        /*! return语句需要和上面的内容隔开一行 */
        return ERR_PARAM;
    }

    pr_info("%s %s %s: Hello world\n", argv[0], __DATE__, __TIME__);

    return 0;
}
