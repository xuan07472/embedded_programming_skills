/*******************************************************************************
 * /brief	从指定路径中读取所有的文件名，并将文件名写入到配置文件中。
 * /details	如果只是将文件夹中所有文件名打印出来，并没有太大实用意义，还需要将
 *			读取的所有文件名按一定格式存储起来，这样方便需要时通过指定文件名
 *			打开指定文件。
 * /warning	读文件夹调用的不是C语言标准库文件，Linux和Windows下使用的头文件和
 *			函数都不一样，本用例使用Linux的dirent.h头文件。
 * /author	将狼才鲸
 * /date	2022-10-23
 * /note	file format: UTF-8，使用UTF-8可以防止用git提交代码查看文件差异时出现
 *			乱码；防止在Windows环境和Linux下来回编译后中文乱码无法恢复。
 * /note	本文件使用tab缩进，文本宽度80字符，使用等宽字体，中文占两个字符宽度。
 ******************************************************************************/

#include <stdio.h>

#define LOOP_BUFFER_UNITEST

#ifdef LOOP_BUFFER_UNITEST
/**
 * /brief	程序入口
 */
int main(void)
{
	printf("test\n");
}
#endif /* LOOP_BUFFER_UNITEST */
