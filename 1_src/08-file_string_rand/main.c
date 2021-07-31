/*******************************************************************************
 * Copyleft (C) 2021 将狼才鲸 zhangjing1943@163.com
 *
 * @file 	main.c
 * @brief	这个文件用于示范文件读写、字符串操作、中文编码格式转换、随机数操作
 * 功能是从Bilibili弹幕里找出符合关注了Up主和发送了特定弹幕条件的所有用户，去重，
 * 然后从中抽奖，并打印抽奖结果
 *
 * @author	将狼才鲸
 * @email	zhangjing1943@163.com
 * @version	1.0.0
 * @date	2021年7月31日
 * @文件编码	UTF-8 Linux换行
 * @编译方式	1、Linux下或者Windows的git MINGW64命令行界面make clean; make
 * 		2、也可以自己在Windos安装gcc和iconv，然后在Windows cmd命令提示符中
 * 		   gcc -o demo main.c -liconv，再运行demo.exe
 *		3、Windows下Qt或者Visual Studio自行新建工程并编译
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
#include <wchar.h>	// L"中文字符串" 双字节宽字符(并不直接就是GB2312或者Unicode，依赖你文件的保存格式)
#include <iconv.h>	// iconv()中文编码转换，可能要手动下载并安装libiconv动态库
#include <stdlib.h>	// malloc() free() memcpy() memset()内存操作
#include <string.h>	// strlen()等字符串操作
#include <errno.h>	// 各种错误码宏定义，还可以直接打印errno变量查看系统函数出错信息

/*==================== 类型定义（struct、 enum 和 typedef） ==================*/

/*================================== 宏定义 ==================================*/
#define DEFAULT_FILE_NAME "弹幕-正式版.txt"  /** Linux default file format UTF-8 */
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#	define DST_FORMAT "GB2312"
#	define SRC_FORMAT "UTF-8" // 也可能是GB2312，这取决于你创建文件的软件，可以用Notepad++软件打开main.c，在右下角查看编码格式
#elif defined(__linux__)
#	define DST_FORMAT "UTF-8"
#	define SRC_FORMAT "UTF-8"
#elif defined(__APPLE__)
#else
#endif

#define err_no (-1)	/** 定义默认错误码 */

/**
 * @brief	重定义输出级别控制
 * @details	- ...和__VA_ARGS__一起实现宏定义函数的调用
 * 		- #和##实现宏定义函数的变量名转成字符串，和变量名拼接并转字符串；
 *		- printf第一个参数可以用多个"" ""拼起来作为一个参数，高级语言也有类似用法；
 *		- 用do{}while(0);包起来可以适配在if else等以为后续只有一行代码而不加{}大括号
 *		  的情况，这样才不会有bug
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
	int ret = 0;

	/* 1. 初始化中文格式转码库 */
	iconv_t cd = iconv_open(DST_FORMAT, SRC_FORMAT); /** 初始化中文转码iconv动态库 */
	if ((iconv_t)-1 == cd) {
      		//perror ("iconv_open()"); // 也可以用这个系统函数，实际输出结果: iconv_open(): Invalid argument
		print(ERROR, LOG, "line:%d iconv_open() open fail! maybe format name illegal\n", __LINE__);
		ret = err_no;
		goto exit;
	}

	char *filename = DEFAULT_FILE_NAME;
	/* 2. 如果用户输入了自定义文件，则处理 */
	if (argc > 1) { // 第1个参数默认是应用程序路径，参数2开始才是用户数据
		filename = argv[1];
		print(DEBUG, LOG, "user input filename: %s\n", (char *)argv[1]);
	}

	char *iname = malloc(strlen(filename) + 1);
	memcpy(iname, filename, strlen(filename));
	size_t ilen = strlen(filename);
	char *oname = malloc(strlen(filename) * 8);
	printf("origin ilen:%d\n",ilen);
	memset(oname, 0, strlen(filename) * 8);
	size_t olen = strlen(filename) * 8;
	errno=0;
	printf("@@@@origin idata:");
	for(int i=0;i<ilen;i++)
		printf("%x ", iname[i]);
	printf("@@@@\n");
	char *srcstr = iname; /** 因为iconv会改变原指针的地址值，所以要保留原指针 */
	char *dststr = oname;
	size_t rtn = iconv (cd, &srcstr, &ilen, &dststr, &olen);
	printf("out olen:%d, iconv ret:%d!!!!!!!!\n",olen, rtn);
	printf("iconv errno:%d\n\n",errno);

	printf("origen idata:%s, (len:%d)\n", iname,ilen);
	printf(">>>not parsed idata:");
	for(int i=0;i<ilen;i++)
		printf(".%x ", iname[i]);
	printf("----\n");
	printf("odata%s, (olen:%d)\n", oname, olen);
	for(int i=0;i<olen;i++)
		printf(".%x ", oname[i]);
	printf("\n<<<<\n\n");


	printf("====\n");
	for(int i=0;i<strlen(filename)+1;i++)
		printf("_%x ", iname[i]);
	printf("\n\n~~~~\n");
	for(int i=0;i<strlen(filename) * 8;i++)
		printf("_%x ", oname[i]);
	printf("\n");

	/* 2. 判断文件存在并可读 */
	if (access(oname, R_OK) == 0) {
		print(DEBUG, LOG, "yes, valid filename: %s\n", oname);
	} else {
		print(ERROR, LOG, "illegal filename: %s! maybe format(such as UTF8) NOT match current OS.\n", oname);
		ret =  err_no;
		goto exit;
	}

	readfile_and_print(oname);

	/* 释放资源 */
exit:
	/* 退出文件转码库 */
	if ((iconv_t)-1 == cd && NULL != cd) {
		iconv_close(cd);
	}

	/* 释放手动分配的内存 */
	if (iname)
		free(iname);
	if (oname)
		free(oname);

	fflush(stdout); // 强制printf信息立即显示，和文件写入立即完成

	while(1); // 用于你直接双击运行时能看到控制台信息，而不是闪退

	return 0;
}

/*================================= 接口函数 =================================*/
static int readfile_and_print(const char *filename)
{

}

/*================================= 文件结尾 =================================*/

