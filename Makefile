###############################################################################
# Makefile是make所调用的文件，用来确定源码的编译和链接规则；make全称GNU Make，
# 是用来将源码编译成可执行程序的软件
#
# 使用方法：Linux环境下，在工程文件夹顶层有Makefile文件的路径下执行命令make
# 即可，清除编译痕迹则使用make clean
###############################################################################

#############################################
## 一种最简单的Makefile写法如下，只需要两行：
#all	:
#	gcc -o output_bin *.c
#############################################

## make命令执行gcc命令时默认带的参数，等号前是变量名，调用时使用$(变量名)
#  :=是赋值，并且覆盖之前的值，而=是基本赋值
CFLAGS :=
LDFLAGS :=

## 指定编译工具软件的名字，可以用来切换本地编译工具和交叉编译工具
CC := gcc

## 文件夹名
SRCDIR := 1_src
MODULE1DIR := 01-c_comments
MODULE2DIR := 02-print_control
MODULE3DIR := 03-console_menu
OBJDIR := 2_makefileBuild
BINDIR := 5_release

## 带路径的源文件名
COMPILE := main.c
COMPILE := $(COMPILE) $(SRCDIR)/$(MODULE2DIR)/print_ctrl.c
COMPILE := $(COMPILE) $(SRCDIR)/$(MODULE3DIR)/menu.c

## 头文件包含路径
INCDIR := -I$(SRCDIR)/$(MODULE1DIR) \
          -I$(SRCDIR)/$(MODULE2DIR) \
          -I$(SRCDIR)/$(MODULE3DIR) \
          -I.                       \


## 将所有.c的名字换成.o存在变量中
OBJECTS := $(foreach var, $(COMPILE), $(OBJDIR)/$(var:.c=.o))

## 指定最终生成的可执行文件名称，也就是gcc -o 后面带的参数
TARGET := demo

## 指明伪标签，伪标签不生成内容
.PHONY: default build clean

## :冒号前面的是make时的入口，也就是标签，make命令不带标签参数时
#  就默认执行第一个标签，:冒号后面是执行前面标签所依赖的东西
#  执行时会依此将依赖的东西全部递归生成完
#build : ./$(BINDIR)/$(TARGET).bin
build : $(TARGET)

## 第一个真正执行的有效标签
#  (这些所需要的文件由下面的Shell命令生成，并且只有生成目标内容所需要的文件
#  比要生成的目标新，Shell命令才会执行)
#./$(BINDIR)/$(TARGET).bin : $(OBJECTS)
$(TARGET) : $(OBJECTS)
#       make要执行的任意Shell命令，这里是gcc，**注意**缩进的语句前一定要是
#       tab，不能是空格，空格会报错；@是取消当前语句的屏幕回显
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

## 将所有.c编译成.o
#  $@ 表示规则中的目标文件集，这里是所有的.o
#  $< 表示规则的第一个依赖文件名，这里是每一个.o所对应的同名的.c
$(OBJDIR)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(INCDIR) $(CFLAGS) -o $@ $<

## clean是一个伪标签，用来作为make命令后面的参数，进一步执行rm Sheel脚本
#  clean是一个约定俗称的名字，用来清理编译痕迹，方便下一次make的时候可以
#  让所有的文件重新编译
clean	:
	rm -r $(OBJDIR)/*
#	rm $(BINDIR)/$(TARGET).bin
	rm $(TARGET)

