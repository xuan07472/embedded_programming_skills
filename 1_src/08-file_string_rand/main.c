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
//#include <regex.h>	// 正则表达式，可能需要手动下载编译并安装相关的库，暂未使用

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
//#define CUR_PRINT_LEVEL DETAIL /** 定义输出级别 */
#define print(level, pure, ...)	do { \
				if ((level) <= CUR_PRINT_LEVEL && (level) != NOPRINT) { \
					if (pure == LOG) \
						printf("["#level"] "); \
					printf(__VA_ARGS__); \
				} \
			} while (0);

#define KEYVALUE_MAXLEN	64

/*==================== 类型定义（struct、 enum 和 typedef） ==================*/
/**
 * @brief	键值对结构体
 */
typedef struct _KEY_VALUE {
	char key[KEYVALUE_MAXLEN];
	char value[KEYVALUE_MAXLEN];
	int keylen;
	int valuelen;
} KEY_VALUE;


/*================================= 全局变量 =================================*/
static int file_dump(const char *file);
static int file_parse(const char *file);
static int string_parse(char *fdata, int flen);
static int regex_parse(char *fdata, int flen);

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
	memset(oname, 0, strlen(filename) * 8);
	size_t olen = strlen(filename) * 8;
	print(DEBUG, LOG, "the used file name: %s\n", iname);

	/* 3. 对文件名进行中文格式转码 */
	char *srcstr = iname; /** 因为iconv会改变原指针的地址值(和输入输出长度值)，所以要保留原指针 */
	char *dststr = oname;
	size_t rtn = iconv (cd, &srcstr, &ilen, &dststr, &olen);
	if (rtn < 0) {
		print(ERROR, LOG, "iconv() chinese string format convert fail!\n");
		goto exit;
	}
	print(DETAIL, LOG, "iconv out[%d]: %s\n", olen, oname);

	/* 2. 判断文件存在并可读 */
	if (access(oname, R_OK) == 0) {
		print(DEBUG, LOG, "yes, valid filename: %s\n", oname);
	} else {
		print(ERROR, LOG, "illegal filename: %s! maybe format(such as UTF8) NOT match current OS.\n", oname);
		ret =  err_no;
		goto exit;
	}

	if (CUR_PRINT_LEVEL >= DETAIL)
		file_dump(oname);

	/* 3. 处理文件并抽奖 */
	file_parse(oname);

	/* 4. 释放资源并退出 */
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

	//while(1); // 用于你直接双击运行时能看到控制台信息，而不是闪退

	return 0;
}

/*================================= 接口函数 =================================*/

/**
 * @brief	读取文件并进行抽奖
 *
 * @param[in]	filename	文件名
 */
static int file_parse(const char *filename)
{
	FILE *fp;
	long filelen;

	/* 1. 打开文件 */
	fp = fopen(filename, "r+");
	if (NULL == fp) {
		print(ERROR, LOG, "fopen() fail!\n");
		return err_no;
	}

	/* 2. 获取文件长度 */
	fseek(fp, 0, SEEK_END); // 先跳到文件末尾
	filelen = ftell(fp);	// 获取文件长度
	rewind(fp);		// 文件指针重新跳转到开头
	print(INFO, LOG, "file len: %ld\n", filelen);

	/* 3. 读取文件 */
	char *filedata = malloc(filelen + 1);
	fread(filedata, 1, filelen, fp);	// 读取全部文件内容
	filedata[filelen] = '\0';		// 手动加上字符串结尾
	fclose(fp);

	/* 4. 处理文件 */
	string_parse(filedata, filelen);
	regex_parse(filedata, filelen); // 此函数仅在Ubuntu下验证，之前的部分在安装git后留下的MINGW64可编译并运行

	return 0;
}

/**
 * @brief	处理字符串，从中找出符合条件的字符串组
 * 
 * @param[in]	fdata	file data
 * @param[in]	flen	file data length
 */
int string_parse(char *fdata, int flen)
{
	char *keystr = "粉丝";
	char *substr;	// 临时的符合条件的子字符串所在指针
	int subnum = 0;	// 被找到的总数
	

	if (!fdata || !flen) {
		print(ERROR, LOG, "file data pointer NULL!\n");
		return err_no;
	}

	substr = fdata;
	char *tmpptr;
	do {
		tmpptr = substr + strlen(keystr);
		substr = strstr(tmpptr, keystr); // 返回字符串中首次出现子串的地址
		if (NULL == substr) {
			if (!subnum)
				print(DEBUG, LOG, "can't find '粉丝' key value in the file, please check!\n");

			break;
		} else {
			subnum++;
		}
	} while (substr != NULL);
	print(INFO, LOG, "key-value num: %d\n", subnum);

	return 0;
}

/**
 * @brief	读取文件并打印文件内容
 *
 * @param[in]	filename	文件名
 */
static int file_dump(const char *filename)
{
	FILE *fp;
	long filelen;

	/* 1. 打开文件 */
	fp = fopen(filename, "r+");
	if (NULL == fp) {
		print(ERROR, LOG, "fopen() fail!\n");
		return err_no;
	}

	/* 2. 获取文件长度 */
	fseek(fp, 0, SEEK_END); // 先跳到文件末尾
	filelen = ftell(fp);	// 获取文件长度
	rewind(fp);		// 文件指针重新跳转到开头
	print(DEBUG, LOG, "file len: %ld\n", filelen);

	char *filedata = malloc(filelen);
	fread(filedata, 1, filelen, fp); // 读取全部文件内容
	fclose(fp);

	print(DEBUG, LOG, "filedata:\n%s\n", filedata);

	return 0;
}

/**
 * @brief	处理字符串，从中找出符合条件的字符串组
 * 
 * @param[in]	fdata	file data
 * @param[in]	flen	file data length
 */
int regex_parse(char *fdata, int flen)
{
#define NAME_MAX_NUM 64
#define TIME_STR_CHARSET "1234567890: -"
	/**
	 * UTF-8英文字符1个字节 0x00 ~ 0x7F
	 * UTF-8中文字符2~4个字节 第一个字节有几个bit1则占几个字节，后续的字节高两位也以10开头
	 */

	char *ptr = fdata; // 拷贝地址进行处理，不修改原始地址
	int totalchar = 0;
	int totalline = 0;
	char name[NAME_MAX_NUM];
	char *substr;
	char *namestr;

	while (*ptr++ != '\0') {
		totalchar++;
		if (*ptr == '\n')
			totalline++;
	}
	print(DEBUG, LOG, "total character: %d\n", totalchar);
	print(DEBUG, LOG, "total line: %d\n", totalline);

	/**
	 * 空行后面的整行是用户名
	 * 找到特殊值\n\n一个空行和\n下一个换行之间的是用户名，
	 * （明明是Windows格式的换行格式，为什么找不到\r\n我也不清楚，可能Linux读文件时内部有替换）
	 */
	ptr = fdata;
	substr = strstr(ptr, "\n\n"); // 找字符串第一次出现的位置
	print(DEBUG, LOG, "names:")
	while (substr > 0) {
		namestr = strstr(substr + 2, "\n");
		if (namestr > 0) {
			if (namestr - substr - 2 >= NAME_MAX_NUM) {
				print(ERROR, LOG, "name length too long!\n");
				return err_no;
			}
			memcpy(name, substr + 2, namestr - substr - 2);
			name[namestr - substr - 2] = '\0';
			print(INFO, PURE, " %s |", name);
		}
		substr = strstr(namestr + 1, "\n\n");
	}
	print(DEBUG, PURE, "\n\n");


	/**
	 * 前一行和后一行都是时间，那么中间的就是弹幕内容
	 * 前一行和后一行的所有字符都在[0-9]数字、 空格、-和:之间，那么中间的整行就是弹幕内容
	 */
	int pos;
	char *starttime;
	char *endtime;
	ptr = fdata;
	substr = strpbrk(ptr, TIME_STR_CHARSET); // 找字符集中的任意一个字符第一次出现的位置
	//printf("substr: %p, %d\n", substr, substr - ptr);
	print(DEBUG, LOG, "contents:")
	while (substr > 0) {
		pos = strspn(substr, TIME_STR_CHARSET);
		//printf("time str len: %d\n", pos);
		if (pos < 5) { // 如果符合条件的字符数目不够，没找到时间字符串
			substr++;
			substr = strpbrk(substr, TIME_STR_CHARSET);
			if (substr <= 0) {
				print(ERROR, LOG, "can't found time string\n");
				break;
			}
			continue;
		}
		/* 已找到第一行时间字符串 */
		starttime = substr + pos + 1; // 越过整个时间字符串和行结尾

second_timestring_find_again:
		substr = strpbrk(substr + pos + 1, TIME_STR_CHARSET);
		if (substr <= 0) {
			print(ERROR, LOG ,"second time string not found!\n");
			break;
		}
		pos = strspn(substr, TIME_STR_CHARSET);
		//printf("second time str len: %d\n", pos);
		if (pos < 5) {
			substr++;
			goto second_timestring_find_again;
		}
		/* 已找到第二行时间字符串 */
		endtime = substr - 1; // 回退用户弹幕后的换行符
		if (endtime - starttime <= 0) {
			print(ERROR, LOG, "contents length error: %d\n", endtime - starttime);
			break;
		}
		memcpy(name, starttime, endtime - starttime);
		name[endtime - starttime] = '\0';
		print(INFO, PURE, " %s |", name);
		//printf("\n");

		endtime += pos;
		substr = strpbrk(endtime, TIME_STR_CHARSET);
	}
	print(DEBUG, PURE, "\n\n");


	return 0;
}

/*================================= 文件结尾 =================================*/

