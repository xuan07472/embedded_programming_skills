################################################################################
# CMake是一个自动化的Makefile生成工具，可以同时在Linux和Windows下建立C/C++编译
# 工程，同时Qt和许多开源项目也是使用的CMake作为编译管理工具
#
# 使用方法：先安装cmake，再在源码目录下手动创建一个CMakeLists.txt文件，编写配置
# 文件，然后进入同级目录下的编译输出目录，然后执行cmake .. 再执行make
# cd 02_build
# cmake ..
# make
#
# 参考资料：
# 《CMake Tutorial》
# https://cmake.org/cmake/help/v3.17/guide/tutorial/index.html
################################################################################

# set the project name
project(demo)

# add the executable
add_executable(demo main.c)
