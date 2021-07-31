/*******************************************************************************
 * Copyleft (C) 2021 将狼才鲸 zhangjing1943@163.com
 *
 * @file 	main.c
 * @brief	这个文件用于示范文件读写、字符串操作、随机数操作
 * 功能是从Bilibili弹幕里找出符合关注了Up主和发送了特定弹幕条件的所有用户，去重，
 * 然后从中抽奖，并打印抽奖结果
 *
 * @author	将狼才鲸
 * @email	zhangjing1943@163.com
 * @version	1.0.0
 * @date	2021年7月31日
 * @文件编码	UTF-8 Linux换行
 * @编译方式	1、Linux下或者Windows的git MINGW64命令行界面make clean; make
 *		2、Windows下Qt或者Visual Studio自行新建工程并编译
 * @源码地址	https://gitee.com/langcai1943/embedded_programming_skills/tree/develop/1_src/08-file_string_rand
 * @视频教程	https://space.bilibili.com/106424039
 *
 * ----------------------------------------------------------------------------*
 * 修改记录:                                                                   *
 * <日期>     | <版本> | <作者>       | <修改记录>                             *
 * ----------------------------------------------------------------------------*
 * 2021-07-31 | 1.0.0  | 将狼才鲸     | 创建文件并实现功能                     *
 * ----------------------------------------------------------------------------*
 *
 ******************************************************************************/

/*================================== 头文件 ==================================*/
#include <stdio.h>

/*==================== 类型定义（struct、 enum 和 typedef） ==================*/

/*================================== 宏定义 ==================================*/
#define DEFAULT_FILE_NAME "弹幕-正式版.txt"

/**
 * @brief	重定义输出级别控制
 * @details	- ...和__VA_ARGS__一起实现宏定义函数的调用
 * 		- #和##实现宏定义函数的变量名转成字符串，和变量名拼接并转字符串；
 *		- printf第一个参数可以用多个"" ""拼起来作为一个参数，高级语言也有类似用法；
 * @param[in]	level	输出级别
 * @param[in]	pure	输出时是否自动加上额外前缀信息
 */
#define DETAIL	4
#define DEBUG	3
#define INFO	2
#define RELEASE	1
#define NOPRINT	0
#define LOG  1
#define PURE 0
#define CUR_PRINT_LEVEL DEBUG /** 定义输出级别 */
#define print(level, pure, ...)	do { \
				if ((level) <= CUR_PRINT_LEVEL && (level) != NOPRINT) { \
					if (pure == LOG) \
						printf("["#level"] "); \
					printf(__VA_ARGS__); \
				} \
			} while (0);

/*================================= 全局变量 =================================*/
static int readfile_and_print(const char *file);
static const char *m_filename;

/*================================= 接口函数 =================================*/
int main(int argc, void *argv[])
{
	m_filename = DEFAULT_FILE_NAME;
	
	if (argc > 1) { /** 参数1默认是应用程序路径，参数2开始才是用户数据 */
		print(DEBUG, PURE, "str: %s\n", argv[1]);
	}

	readfile_and_print(m_filename);
}

/*================================= 接口函数 =================================*/
static int readfile_and_print(const char *filename)
{

}

/*================================= 文件结尾 =================================*/

