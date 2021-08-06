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
#include <stdlib.h>	// malloc() free() memcpy() memset()内存操作 rand()随机数
#include <string.h>	// strlen()等字符串操作
#include <errno.h>	// 各种错误码宏定义，还可以直接打印errno变量查看系统函数出错信息
#include <time.h>	// 用当前时间做随机数种子
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
#define CUR_PRINT_LEVEL DEBUG /** 定义默认输出级别 */
//#define CUR_PRINT_LEVEL DETAIL
#define print(level, pure, ...)	do { \
				if ((level) <= CUR_PRINT_LEVEL && (level) != NOPRINT) { \
					if (pure == LOG) \
						printf("["#level"] "); \
					printf(__VA_ARGS__); \
				} \
			} while (0)

#define KEYVALUE_MAXLEN	64
#define TIME_STRING_LEN 19
#define TIME_STR_CHARSET "1234567890: -" // 用于匹配日期时间字符串
#define TABLE_STR_START " =========================================================================================\n `````````````````````````````````````````````````````````````````````````````````````````\n"
#define TABLE_STR_END " _________________________________________________________________________________________\n =========================================================================================\n\n\n"
#define TABLE_STR_MIDDLE " -----------------------------------------------------------------------------------------\n"

/*==================== 类型定义（struct、 enum 和 typedef） ==================*/
typedef enum {
	FALSE = 0,
	TRUE = 1
} BOOL;

/**
 * @brief	键值枚举
 */
typedef enum _KEY {
	NAME = 0,
	CONTENT,
	FOLLOWER
} KEY;

/**
 * @brief	键值对结构体
 */
typedef struct _KEY_VALUE {
	KEY key;
	char value[KEYVALUE_MAXLEN];
	int valuelen;
} KEY_VALUE;

typedef struct _ITEM {
	KEY_VALUE name;		/** 用户名 */
	KEY_VALUE content;	/** 弹幕 */
	KEY_VALUE follower;	/** 粉丝 */
	BOOL is_follower;	/** 是否粉丝 */
	BOOL true_content;	/** 弹幕是否有效 */
	BOOL can_choose;	/** 是否已去重 */
	BOOL win;			/** 是否已中奖 */
} ITEM;

/*================================= 全局变量 =================================*/
static int file_parse(const char *file);
static int string_parse(char *fdata, int flen);
static int lottery_draw(ITEM *items, int num);
static int dump_file(const char *file);
static int dump_item(char *fdata, int flen);
static int dump_names_contents(char *fdata, int flen);
static ITEM *all_item = NULL;
static int item_totalnum = 0;

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

	/* 4. 判断文件存在并可读 */
	if (access(oname, R_OK) == 0) {
		print(DEBUG, LOG, "yes, valid filename: %s\n", oname);
	} else {
		print(ERROR, LOG, "illegal filename: %s! maybe format(such as UTF8) NOT match current OS.\n", oname);
		ret =  err_no;
		goto exit;
	}

	if (CUR_PRINT_LEVEL >= DETAIL)
		dump_file(oname);

	/* 5. 处理文件并分析文件中信息 */
	file_parse(oname);

	/* 6. 弹幕去重并抽奖 */
	lottery_draw(all_item, item_totalnum);

	/* 7. 释放资源并退出 */
exit:
	fflush(stdout); // 强制printf信息立即显示，和文件写入立即完成

	/* 退出文件转码库 */
	if ((iconv_t)-1 == cd && NULL != cd) {
		iconv_close(cd);
	}

	/* 释放手动分配的内存 */
	if (iname)
		free(iname);
	if (oname)
		free(oname);
	if (all_item)
		free(all_item);

	//while(1); // 用于你直接双击运行时能看到控制台信息，而不是闪退

	return 0;
}

/*================================= 私有函数 =================================*/

/**
 * @brief	读取文件并解析用户名、是否粉丝、弹幕内容等
 * @details	解析后的内容存在全局变量中
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
	if (CUR_PRINT_LEVEL >= DETAIL)
		dump_item(filedata, filelen);
	if (CUR_PRINT_LEVEL >= DETAIL)
		dump_names_contents(filedata, filelen);

	string_parse(filedata, filelen); // 处理字符串并进行抽奖

	if (filedata)
		free(filedata);

	return 0;
}

/**
 * @brief	处理字符串，进行抽奖
 * @details	获取用户名总数，获取用户名-是否粉丝-发言内容并打印，判断是否粉丝，判断发言是否包含‘c/C语言’
 * 
 * @param[in]	fdata	file data
 * @param[in]	flen	file data length
 */
static int string_parse(char *fdata, int flen)
{
	char *str = fdata;
	int pos;
	int item_currentnum = 0;
	char *cur_value;

	/* 1. 获取用户总数，并打印 */
	char *substr = strpbrk(str, TIME_STR_CHARSET); /** 找字符集中的任意一个字符第一次出现的位置 */
	while (substr > 0) {
		pos = strspn(substr, TIME_STR_CHARSET);	/** 找不在字符集中第一个字符出现的相对位置 */
		if (pos < TIME_STRING_LEN) {
			substr++;
			substr = strpbrk(substr, TIME_STR_CHARSET);
			if (substr <= 0) {
				print(DETAIL, LOG, "can't found time string, or string end\n");
				break;
			}
			continue;
		
		}
		item_totalnum++;
		substr = substr + pos + 1; // 越过整个时间字符串和行结尾
		substr = strpbrk(substr, TIME_STR_CHARSET);
	}
	print(DEBUG, LOG, "ITEM NUM: %d\n", item_totalnum);
	print(DETAIL, PURE, TABLE_STR_START);

	/* 2. 分配内存 */
	all_item = malloc(sizeof(ITEM) * item_totalnum);

	/* 3. 读取用户名、是否粉丝、弹幕内容，并打印 */
	str = fdata;
	substr = strstr(str, "\n\n"); // 找到空行，空行后面那行是用户名
	char *findstr;
	while (substr > 0) {
		findstr = strstr(substr + 2, "\n");
		if (findstr <= 0) {
			substr += 3;
			substr = strstr(substr, "\n");
			continue;
		}
		if (findstr - substr - 2 >= KEYVALUE_MAXLEN) {
			print(ERROR, LOG, "name length too long!\n");
			return err_no;
		}
		/** 已找到用户名字符串 */
		all_item[item_currentnum].name.key = NAME;
		cur_value = all_item[item_currentnum].name.value;
		memcpy(cur_value, substr + 2, findstr - substr - 2);
		cur_value[findstr - substr - 2] = '\0';
		all_item[item_currentnum].name.valuelen = findstr - substr - 2;

		substr = findstr + 1; // 越过换行符，下一行如果不是时间，则就是“粉丝”
		pos = strspn(substr, TIME_STR_CHARSET);	/** 找不在字符集中第一个字符出现的相对位置 */

		/** 已找到是否有“粉丝”标志 */
		if (pos <= 0) { // 如果找到了“粉丝标志”
			findstr = strstr(substr, "\n");
			if (findstr > 0) {
				cur_value = all_item[item_currentnum].follower.value;
				memcpy(cur_value, substr, findstr - substr);
				cur_value[findstr - substr] = '\0';
				all_item[item_currentnum].follower.valuelen = findstr - substr;
				print(DETAIL, PURE, " || %s |\t\t", all_item[item_currentnum].follower.value);
				substr = findstr + 1;
			}
			all_item[item_currentnum].follower.key = FOLLOWER;
			all_item[item_currentnum].is_follower = TRUE;
		} else {
			all_item[item_currentnum].follower.key = FOLLOWER;
			all_item[item_currentnum].follower.valuelen = 0;
			all_item[item_currentnum].is_follower = FALSE;
			print(DETAIL, PURE, " || ____ |\t\t");
		}
		print(DETAIL, PURE, "%s |\t\t", all_item[item_currentnum].name.value);

		substr = strpbrk(substr, TIME_STR_CHARSET); /** 找字符集中的任意一个字符第一次出现的位置 */
		pos = strspn(substr, TIME_STR_CHARSET);	/** 找不在字符集中第一个字符出现的相对位置 */
		substr += (pos + 1); // 越过日期时间字符串
		findstr = strstr(substr, "\n"); // 找到当前行末尾，当前行的内容就是弹幕内容

		/** 已找到弹幕 */
		cur_value = all_item[item_currentnum].content.value;
		all_item[item_currentnum].content.valuelen = findstr - substr;
		memcpy(cur_value, substr, findstr - substr);
		cur_value[findstr - substr] = '\0';
		print(DETAIL, PURE, "%s |\t\t", all_item[item_currentnum].content.value);

		char *tmpstr = strpbrk(substr, "cC"); // 找是否存在大小写C，用来匹配“C语言程序设计现代方法”字符串
		/** 已识别弹幕是否符合要求 */
		if (tmpstr >= substr && tmpstr <= findstr) { // 如果弹幕符合条件
			all_item[item_currentnum].true_content = TRUE;
			print(DETAIL, PURE, "!!!!YES!!!!|\t\t");
		} else {
			all_item[item_currentnum].true_content = FALSE;
			print(DETAIL, PURE, "____NO____|\t\t");
		
		}

		print(DETAIL, PURE, "\n");
		item_currentnum++;
		substr = strstr(findstr + 1, "\n\n");
	}
	print(DETAIL, PURE, TABLE_STR_END);

exit:

	return 0;
}

/**
 * @brief	信息数组去重并抽奖
 *
 * @param[in]	items	已解析过的所有的用户和弹幕信息
 * @param[in]	num		项目总数
 */
static int lottery_draw(ITEM *items, int num)
{
	if (!items || !num)
		return err_no;

	/* 1. 打印原始信息 */
	print(DEBUG, PURE, " \t\t\t@@@@####======== 所有弹幕信息 ========####@@@@\n");
	print(DEBUG, PURE, TABLE_STR_START);
	print(DEBUG, PURE, " || index\t| follower\t| true_content\t| name\t\t| content\t\t|\n");
	print(DEBUG, PURE, TABLE_STR_MIDDLE);
	for (int i = 0; i < num; i++) {
		print(DEBUG, PURE, " || %d\t\t|", i);
		if (items[i].follower.valuelen) {
			print(DEBUG, PURE, " %s\t\t|", items[i].follower.value);
		} else {
			print(DEBUG, PURE, " ____\t\t|");
		}
		print(DEBUG, PURE, " %s\t|", (items[i].true_content ? "!!YES!!" : "__NO__"));
		print(DEBUG, PURE, " %s\t|", items[i].name.value);
		print(DEBUG, PURE, " %s |\n", items[i].content.value);
	}
	print(DEBUG, PURE, TABLE_STR_END);

	/* 2. 去除不是粉丝、弹幕不符合要求的用户 */
	print(DEBUG, PURE, " \t\t\t@@@@####======== 有效弹幕（未去重） ========####@@@@\n");
	print(DEBUG, PURE, TABLE_STR_START);
	print(DEBUG, PURE, " || index\t| follower\t| true_content\t| name\t\t| content\t\t|\n");
	print(DEBUG, PURE, TABLE_STR_MIDDLE);
	int index = 0;
	for (int i = 0; i < num; i++) {
		items[i].can_choose = TRUE;
		if (!items[i].is_follower)
			items[i].can_choose = FALSE;
		if (!items[i].true_content)
			items[i].can_choose = FALSE;
		if (items[i].can_choose) {
			print(DEBUG, PURE, " || %d\t\t|", index);
			if (items[i].follower.valuelen) {
				print(DEBUG, PURE, " %s\t\t|", items[i].follower.value);
			} else {
				print(DEBUG, PURE, " ____\t\t|");
			}
			print(DEBUG, PURE, " %s\t|", (items[i].true_content ? "!!YES!!" : "__NO__"));
			print(DEBUG, PURE, " %s\t|", items[i].name.value);
			print(DEBUG, PURE, " %s |\n", items[i].content.value);
			index++;
		}
	}
	print(DEBUG, PURE, TABLE_STR_END);

	/* 3. 用户名去重，打印奖池信息 */
	print(DEBUG, PURE, " \t\t\t@@@@####======== 有效弹幕 ========####@@@@\n");
	print(DEBUG, PURE, TABLE_STR_START);
	print(DEBUG, PURE, " || index\t| follower\t| true_content\t| name\t\t| content\t\t|\n");
	print(DEBUG, PURE, TABLE_STR_MIDDLE);
	index = 0;
	for (int i = 0; i < num; i++) {
		for (int j = i; j < num; j++) {
			if (i == j)
				continue;
			if (!strcmp(items[i].name.value, items[j].name.value)) { // 如果两个字符串相等
				items[j].can_choose = FALSE;
			}
		}
		if (items[i].can_choose) {
			print(DEBUG, PURE, " || %d\t\t|", index);
			if (items[i].follower.valuelen) {
				print(DEBUG, PURE, " %s\t\t|", items[i].follower.value);
			} else {
				print(DEBUG, PURE, " ____\t\t|");
			}
			print(DEBUG, PURE, " %s\t|", (items[i].true_content ? "!!YES!!" : "__NO__"));
			print(DEBUG, PURE, " %s\t|", items[i].name.value);
			print(DEBUG, PURE, " %s |\n", items[i].content.value);
			index++;
		}
	}
	print(DEBUG, PURE, TABLE_STR_END);

	/* 4. 抽奖 */
	print(DEBUG, PURE, " @@@@####======== 抽奖5名粉丝，送《C语言程序设计-现代方法（最新修订版）》，书价一百多元 ========####@@@@\n");
	print(INFO, LOG, "现在开始抽奖……\n");
	fflush(stdout);
	for (int i = 0; i < 100; i++) {
		print(INFO, PURE, ".");
		fflush(stdout);
		usleep(10 * 1000);
	}
	print(INFO, PURE, "\n");

	int can_choose_total = index; // 可抽奖粉丝数 + 1
	int choose_num = 5; // 抽奖5名粉丝
	int a, p;
	srand((unsigned int )time(NULL)); // 用当前时间重置随机数种子
#define CHOOSE_MAX 64
	int was_choose[CHOOSE_MAX] = {0};
	print(INFO, LOG, "中奖序号：");
	for (int i = 0; i < choose_num; i++)
	{
again:
		a = rand();
		p = a % can_choose_total; // 设置范围0 ~ (can_choose_total - 1)
		p++; // 0是特殊值，抽奖的每个数都 + 1
		int j;
		for (j = 0; j < CHOOSE_MAX; j++) {
			if (was_choose[j] == p) // 如果抽到重复的则重新抽
				goto again;
		}
		if (j == CHOOSE_MAX) {
			was_choose[i] = p; // 中奖序号 + 1
			print(INFO, PURE, "%d  ", p - 1);
		}
	}
	print(INFO, PURE, "\n");

	/* 5. 打印中奖信息 */
	print(DEBUG, PURE, " \t\t\t@@@@####======== 中奖信息 ========####@@@@\n");
	print(DEBUG, PURE, TABLE_STR_START);
	print(DEBUG, PURE, " || index\t| follower\t| true_content\t| name\t\t| content\t\t|\n");
	print(DEBUG, PURE, TABLE_STR_MIDDLE);
	index = 0;
	for (int i = 0; i < num; i++) {
		if (items[i].can_choose) {
			index++; // 提前+1和中奖序号进行对比
			for (int j = 0; j < CHOOSE_MAX; j++) {
				if (index == was_choose[j]) {
					print(DEBUG, PURE, " || %d\t\t|", index - 1);
					if (items[i].follower.valuelen) {
						print(DEBUG, PURE, " %s\t\t|", items[i].follower.value);
					} else {
						print(DEBUG, PURE, " ____\t\t|");
					}
					print(DEBUG, PURE, " %s\t|", (items[i].true_content ? "!!YES!!" : "__NO__"));
					print(DEBUG, PURE, " %s\t|", items[i].name.value);
					print(DEBUG, PURE, " %s |\n", items[i].content.value);
				}
			}
		}
	}
	print(DEBUG, PURE, TABLE_STR_END);

	return 0;
}

/**
 * @brief	处理字符串，从中找出符合条件的字符串组
 * @details	打印发言用户中粉丝总数（未去重）
 *
 * @param[in]	fdata	file data
 * @param[in]	flen	file data length
 */
static int dump_item(char *fdata, int flen)
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
static int dump_file(const char *filename)
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
	if (filedata)
		free(filedata);

	return 0;
}

/**
 * @brief	处理字符串，从中找出符合条件的字符串组
 * @details	打印所有发言用户和所有弹幕内容（未去重）
 * 
 * @param[in]	fdata	file data
 * @param[in]	flen	file data length
 */
static int dump_names_contents(char *fdata, int flen)
{
	/**
	 * UTF-8英文字符1个字节 0x00 ~ 0x7F
	 * UTF-8中文字符2~4个字节 第一个字节从最高位算起有几个bit 1 则占几个字节，后续的字节高两位也以10开头
	 */

	char *ptr = fdata; // 拷贝地址进行处理，不修改原始地址
	int totalchar = 0;
	int totalline = 0;
	char name[KEYVALUE_MAXLEN];
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
	 * （明明是Windows格式的换行格式，为什么找不到\r\n我也不清楚，可能Linux读文件时内部直接替换为了\n）
	 */
	ptr = fdata;
	substr = strstr(ptr, "\n\n"); // 找字符串第一次出现的位置
	print(DEBUG, LOG, "names:");
	while (substr > 0) {
		namestr = strstr(substr + 2, "\n");
		if (namestr > 0) {
			if (namestr - substr - 2 >= KEYVALUE_MAXLEN) {
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
	substr = strpbrk(ptr, TIME_STR_CHARSET); /** 找字符集中的任意一个字符第一次出现的位置 */
	print(DEBUG, LOG, "contents:");
	while (substr > 0) {
		pos = strspn(substr, TIME_STR_CHARSET);	/** 找不在字符集中第一个字符出现的相对位置 */
		if (pos < 5) { // 如果符合条件的字符数目不够，没找到时间字符串
			substr++;
			substr = strpbrk(substr, TIME_STR_CHARSET);
			if (substr <= 0) {
				print(DETAIL, LOG, "can't found time string, or string end\n");
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
		if (endtime - starttime >= KEYVALUE_MAXLEN) {
			print(ERROR, LOG, "contents length too long!\n");
			return err_no;
		}
		memcpy(name, starttime, endtime - starttime);
		name[endtime - starttime] = '\0';
		print(INFO, PURE, " %s |", name);

		endtime += pos;
		substr = strpbrk(endtime, TIME_STR_CHARSET);
	}
	print(DEBUG, PURE, "\n\n");


	return 0;
}

/*================================= 文件结尾 =================================*/
