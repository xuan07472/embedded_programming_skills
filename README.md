[《项目启动说明书》](https://gitee.com/langcai1943/embedded_programming_skills/blob/develop/0_doc/00-项目管理/01-项目启动说明书.md)

**项目目录机构**
<p>
.
└── embedded_programming_skills
    ├── 0_doc// 存放项目文档
    │   ├── 00-项目管理
    │   │   ├── 01-项目启动说明书.md
    │   │   ├── 11-Linux内核C编码规范.md
    │   │   ├── 12-C语言应用编码规范.md
    │   │   └── 13-用于API文档自动生成的doxygen注释规范.md
    │   ├── 01-doxygen_A// doxygen工具从源码注释中自动生成文档的路径
    │   └── LICENSE     // 整个工程的许可
    ├── 1_src            // 源码目录
    │   ├── aDoxygenDemo.h
    │   └── example
    ├── 2_makefileBuild  // 直接使用makefile工具编码时的输出目录
    ├── 3_cmakeBuild     // 使用cmake编译时的输出目录
    ├── 4_automakeBuild  // 使用automake编译时的输出目录（里面有一份automake demo）
    │   ├── compile -> /usr/share/automake-1.15/compile
    │   ├── config.h.in
    │   ├── configure
    │   ├── configure.ac
    │   ├── depcomp -> /usr/share/automake-1.15/depcomp
    │   ├── install-sh -> /usr/share/automake-1.15/install-sh
    │   ├── main.c
    │   ├── Makefile
    │   ├── Makefile.am
    │   ├── Makefile.in
    │   ├── missing -> /usr/share/automake-1.15/missing
    │   └── stamp-h1
    ├── 5_release        // 编译完成后会将可执行文件和库拷贝到这个目录
    ├── automake.txt     // automake编译工具的简易使用教程
    ├── CMakeLists.txt   // cmake编译工具的配置文件
    ├── Doxyfile         // doxygen使用源码注释生成文档工具的配置文件
    ├── main.c           // 整个工程的入口
    ├── Makefile         // 直接使用makefile编码时的配置文件
    ├── README.md        // 整个工程的说明文档
    └── vimrc
</p>
