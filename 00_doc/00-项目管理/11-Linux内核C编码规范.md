参考资料：https://www.kernel.org/doc/Documentation/process/coding-style.rst

如果你使用Linux内核C编码规范，
则遵守以下规则：

1、缩进使用tab（显示为8个字符宽），不要使用空格缩进

2、swtch和case放在同一缩进，如：
switch (suffix) {
case 'G':

3、**不要**一行放多条语句，如：
if (condition) do_this;
a = b; c = d;
int a, b, c;

4、不要在行末留有空格

5、每行最多80字符宽，太长的语句要换行，整个字符串除外，不要切断字符串

6、if, switch, case, for, do, while等大多数关键字和小括号之间要有空格，双目操作符两端要有空格，函数参数逗号后要有空格，如：
for (i = 0; i < max; i++) {
}
    对于像函数的关键字sizeof, typeof, alignof 或者 __attribute__后面不要加空格，如：
s = sizeof(struct file);
    **不要**在小括号内侧两边加上空格（有些人喜欢这种格式），如：
s = sizeof( struct file );
    在大多数二元和三元操作符两侧使用一个空格，例如下面所有这些操作符:
    =  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
    但是一元操作符后不要加空格:
    &  *  +  -  ~  !  sizeof  typeof  alignof  __attribute__  defined

6、非函数语句块 (if, switch, for, while, do)起始大括号放在行尾，结束大括号放在行首，如
if (x is true) {
        we do y
}

7、if else while等语句当所有分支都只有一行语句时不要加大括号，有任何一个分支或者有多个下级语句时则因都有大括号，如：
if (condition)
        action();

if (condition)
        do_this();
else
        do_that();

if (condition) {
        do_this();
        do_that();
} else {
        otherwise();
}

8、指针*符号靠近变量名，而不是靠近类型名（有些人喜欢这种格式），推荐的做法如下：
char *linux_banner;
unsigned long long memparse(char *ptr, char **retptr);
char *match_strdup(substring_t *s);

9、不要在在函数名中包含函数类型 (所谓的匈牙利命名法)
本地变量名应该简短，如i j k tmp
不对结构体和指针使用typedef（有些人喜欢这种格式）
函数应该简短而漂亮，并且只完成一件事情。函数应该可以一屏或者两屏显示完，多使用辅助函数
函数的另外一个衡量标准是本地变量的数量。此数量不应超过 5－10 个
在源文件里，使用空行隔开不同的函数。
当一个函数从多个位置退出，并且需要做一些类似清理的常见操作时，使用goto语句
goto标签不错的名字可以是 out_free_buffer: 。别去使用像 err1: 和 err2:

10、不要在注释里解释你的代码是如何运作的，而是告诉别人你的代码做了什么
不要把注释放在一个函数体内部：如果函数复杂到你需要独立的注释其中的一部分，请拆分函数
注释内核API函数时，请使用kernel-doc 格式。请看Documentation/doc-guide/和scripts/kernel-doc以获得详细信息
如：
/*
 * This is the preferred style for multi-line
 * comments in the Linux kernel source code.
 * Please use it consistently.
 *
 * Description:  A column of asterisks on the left side,
 * with beginning and ending almost-blank lines.
 */

10、用于定义常量的宏的名字及枚举里的标签需要大写。
#define CONSTANT 0x12345
形如函数的宏的名字可以用小写字母，如果能写成内联函数就不要写成像函数的宏，含有多个语句的宏应该被包含在一个 do-while 代码块里，如：
#define macrofun(a, b, c)                       \
        do {                                    \
                if (a == 5)                     \
                        do_this(b, c);          \
        } while (0)
如果一个函数有 3 行以上，就不要把它变成内联函数
宏定义函数中不要使用 return，因为获取不了返回值，宏函数必须在参数两边加上小括号
在定义几个相关的常量时，最好用枚举。

11、输出信息不要以.句号结尾

12、Linux内核内打印错误信息使用dev_err(), dev_warn(), dev_info() pr_notice(), pr_info(), pr_warn(), pr_err()
传递结构体大小的首选形式是这样的：
p = kmalloc(sizeof(*p), ...);

13、强制转换一个 void 指针返回值是多余的

14、函数返回值
(-Exxx＝失败，0＝成功) 或者一个 成功 布尔值 (0＝失败，非0＝成功)

15、只要可能，就不要在 .c 文件里面使用预处理条件 (#if, #ifdef)，替代方案是，在头文件中用预处理条件提供给那些 .c 文件 使用，最好倾向于编译整个函数，而不是函数的一部分或表达式的一部分
    对#endif进行注解
#ifdef CONFIG_SOMETHING
...
#endif /* CONFIG_SOMETHING */

16、注释使用 /*  */ 不要使用 //

