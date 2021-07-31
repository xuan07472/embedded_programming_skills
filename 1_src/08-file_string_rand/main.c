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
#include <stdio.h>	// printf()打印
#include <unistd.h>	// access()判断文件可访问
#include <wchar.h>	// L"中文字符串" 双字节宽字符(并不直接就是GB2312或者Unicode)
#include <iconv.h>	// iconv()中文编码转换
#include <stdlib.h>	// malloc() free()

/*==================== 类型定义（struct、 enum 和 typedef） ==================*/

/*================================== 宏定义 ==================================*/
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
//#	define DEFAULT_FILE_NAME L"弹幕-正式版.txt" /** 双字节宽字符(Windows默认 GB2312) */
#	define DEFAULT_FILE_NAME "弹幕-正式版.txt"  /** Linux default UTF-8 */
//static const wchar_t *m_filename;
//#	define DEFAULT_FILE_NAME "aaa.txt" /** 双字节宽字符(Windows默认 GB2312) */
static const char *m_filename;
#elif defined(__linux__)
#	define DEFAULT_FILE_NAME "弹幕-正式版.txt"  /** Linux default UTF-8 */
static const char *m_filename;
#elif defined(__APPLE__)
#else
#endif
#define err_no (-1)	/** 定义默认错误码 */

/**
 * @brief	重定义输出级别控制
 * @details	- ...和__VA_ARGS__一起实现宏定义函数的调用
 * 		- #和##实现宏定义函数的变量名转成字符串，和变量名拼接并转字符串；
 *		- printf第一个参数可以用多个"" ""拼起来作为一个参数，高级语言也有类似用法；
 *		- 用do{}while(0);包起来可以用在if else等以为后续只有一样代码时不加{}大括号
 *		  的情况，不加会有bug
 * @param[in]	level	输出级别
 * @param[in]	pure	输出时是否自动加上额外前缀信息
 */
#define DETAIL	6
#define DEBUG	5
#define INFO	4
#define WARN	3
#define ERROR	2
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

/*================================= 接口函数 =================================*/
/**
 * @brief	主函数入口
 * @details	- 用户使用方法：
 * 		- 处理默认文件 (Linux命令行)./demo
 * 		  或者(windows cmd命令提示符)demo.exe或者双击应用程序
 * 		- 处理用户自定义文件 (Linux命令行)./demo yourfilename
 * 		  或者(window cmd命令提示符)demo.exe youfilename
 *
 * @param[in]	argc	参数个数+1
 * @param[in]	argv	参数字符串指针数据，第一个参数是默认应用程序路径，从第二个
 *			参数开始才是用户参数
 */
int main(int argc, void *argv[])
{
	iconv_t cd;
	//cd = iconv_open("GB2312", "UTF-8"); /** frome UTF-8 to GBK2312 */
	cd = iconv_open("ASCII", "ASCII"); /** frome UTF-8 to GBK2312 */
	if ((iconv_t)-1 == cd) {
		print(ERROR, LOG, "iconv_open() open fail! maybe format name is illegal\n");
		return err_no;
	}
	char *istr = malloc(64);
	memset(istr, 0, 64);
	memcpy(istr, "aaaaaaaaaa", 10);
	char *ostr = malloc(64);
	memset(ostr, 0, 64);
	size_t ilen = strlen(istr);
	printf("ilen:%d\n",ilen);
	size_t olen;
	errno=0;
	size_t ret = iconv (cd, &istr, &ilen, &ostr, &olen);
	printf("errno:%d\n",errno);
	printf("olen:%d, iconv ret:%llx\n",olen, ret);
	printf("i====%s, len:%d\n", istr,ilen);
	for(int i=0;i<ilen;i++)
		printf("%x ", istr[i]);
	printf("o====%s\n", ostr);
	for(int i=0;i<ilen;i++)
		printf("%x ", ostr[i]);
	printf("~~~");
	iconv_close(cd);

	m_filename = DEFAULT_FILE_NAME;

	/* 1. 如果用户输入了自定义文件，则处理 */
	if (argc > 1) { // 第1个参数默认是应用程序路径，参数2开始才是用户数据
		m_filename = argv[1];
		print(DEBUG, LOG, "user input filename: %s\n", (char *)argv[1]);
	}

	/* 2. 判断文件存在并可读 */
	if (access(m_filename, R_OK) == 0) {
		print(DEBUG, LOG, "yes, valid filename: %s\n", m_filename);
		return err_no;
	} else {
		print(ERROR, LOG, "illegal filename: %s! maybe format(such as UTF8) NOT match current OS.\n", m_filename);
	}

	readfile_and_print(m_filename);
}

/*================================= 接口函数 =================================*/
static int readfile_and_print(const char *filename)
{

}

/*================================= 文件结尾 =================================*/

