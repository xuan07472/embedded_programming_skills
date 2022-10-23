/*******************************************************************************
 * /brief	从指定路径中读取并打印所有文件名
 * /details	提示：如果只是将文件夹中所有文件名打印出来，并没有太大实用意义；可以将
 *			读取的所有文件名按一定格式如XML格式存储起来，这样方便需要时通过指定
 *			键值查找和使用文件名；也可以将一批文件用1.txt、2.txt、3.txt等自然数
 *			顺序存储起来，使用时用snprintf直接来遍历文件名打开。
 * /warning	读文件夹调用的不是C语言标准库文件，Linux和Windows下使用的头文件和
 *			函数都不一样，本用例使用Linux的dirent.h头文件。
 * /author	将狼才鲸
 * /date	2022-10-23
 * /note	file format: UTF-8，使用UTF-8可以防止用git提交代码查看文件差异时出现
 *			乱码；防止在Windows环境和Linux下来回编译后中文乱码无法恢复。
 * /note	本文件使用tab缩进，文本宽度80字符，使用等宽字体，中文占两个字符宽度。
 * /note	参考资料：[【获取指定目录下的所有文件名】]
 *			(https://blog.csdn.net/hello_dear_you/article/details/121789764)
 * /note	Linux规定单个文件或单个目录名应该小于255个字符，对目录深度的限制不明
 * /note	Windows下MinGW下的头文件路径：
 *			D:\ProgramFiles\msys64\mingw32\i686-w64-mingw32\include\dirent.h
 *			D:\ProgramFiles\msys64\mingw64\x86_64-w64-mingw32\include\dirent.h
 *			D:\ProgramFiles\msys64\usr\include\sys\dirent.h
 ******************************************************************************/

/********************************** 头文件 ************************************/
#include <stdio.h>	/* printf */
#include <dirent.h>	/* opendir readdir closedir */

/********************************** 宏定义 ************************************/
#define LOOP_BUFFER_UNITEST /* 定义可以直接编译测试此文件 */
#define DEFAULT_DIR_PATH "D:\\1_git\\embedded_programming_skills\\1_src\\09-get_all_file_names_from_dir\\"
#define FILENAME_LENGTH_MAX 256 /* Linux规定单个文件或单个目录名应该小于255个字符 */
#define pr_debug	printf
#define pr_info		printf
#define pr_err		printf
#define EOK			(0)
#define ECOMMON		(-1)

/********************************* 接口函数 ***********************************/
#ifdef LOOP_BUFFER_UNITEST
/**
 * /brief	程序入口
 */
int main(int argc, char *argv[])
{
	DIR *fd_dir;		/* 目录句柄 */
	/* [dirent结构体中d_type](https://blog.csdn.net/fqx111/article/details/8983547) */
	struct dirent *ptr;	/* 读取的文件名或子文件夹信息 */
	char *user_path = NULL;

	pr_info(" ========\n");
	pr_info("| main函数的参数1默认是本程序的路径和名称\n");
	pr_info("| argv[0]: %s\n", argv[0]);
	pr_info("| 可以使用./demo your_path 的方式执行你自己指定的目录\n");
	pr_info(" ========\n\n");

	/* 1. 指定要操作的目录名 */
	if (argc > 1) {
		/* 如果有两个参数传入，则默认第二个参数是目录名，第一个参数固定是本程序的路径和名称 */
		pr_debug("user path name: %s\n", argv[1]);
		user_path = argv[1];
	}

	/* 2. 打开目录 */
	if (!user_path)
		user_path = DEFAULT_DIR_PATH;

	fd_dir = opendir(user_path);
	if (!fd_dir) {
		pr_err("ERR: 打开目录%s失败！\n", user_path);
		return 0;
	}
	pr_info("父目录名：%s\n子文件及子文件夹：\n", user_path);

	/* 3. 遍历文件夹 */
	ptr = (struct dirent *)(-1);
	while (ptr) {
		ptr = readdir(fd_dir);
		if (ptr) {
			pr_info("name: %s\n", ptr->d_name);

			if (!strcmp(ptr->d_name, ".") && !strcmp(ptr->d_name, "..")) {
				/* .是本目录，..是父目录，Linux每个文件夹中都有的两个子目录链接 */
				/**
				 * TODO: 判断名称是文件夹还是文件；将名称加入到自己的键值对、队列或者链表中，
				 * 		 等待进一步使用
				 */

				/**
				 * [64位dirent结构体中d_type](https://blog.csdn.net/fqx111/article/details/8983547)
				 * DT_UNKNOWN = 0,
				 * DT_FIFO = 1,
				 * DT_CHR = 2,
				 * DT_DIR = 4,
				 * DT_BLK = 6,
				 * DT_REG = 8,
				 * DT_LNK = 10,
				 * DT_SOCK = 12,
				 * DT_WHT = 14
				 */
				/* 64位判断是文件还是文件夹 */
				//pr_info("%d ", ptr->d_type); // 32位dirent没有d_type成员

				/**
				 * [32位linux c 判断路径是 目录还是文件](https://blog.csdn.net/whatday/article/details/103362612)
				 * 函数 stat（）用于获取一个文件路径的信息，并把获取到的信息放到结构体 struct stat 中
				 * 宏 S_ISDIR（）用于判断一个文件路径是不是一个目录，是则返回1，否则返回0
				 * 宏 S_ISREG（）用于判断一个文件路径是不是一个普通文件，是则返回1，否则返回0
				 */
			}
		}
	}

	closedir(fd_dir);

	return 0;
}
#endif /* LOOP_BUFFER_UNITEST */
