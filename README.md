[《项目启动说明书》](https://gitee.com/langcai1943/embedded_programming_skills/blob/develop/0_doc/00-项目管理/01-项目启动说明书.md)

**项目目录结构**
<p>
.<br>
└── embedded_programming_skills<br>
    ├── 0_doc// 存放项目文档<br>
    │   ├── 00-项目管理<br>
    │   │   ├── 01-项目启动说明  <br>
    │   │   ├── 11-Linux内核C编码规范<br>
    │   │   ├── 12-C语言应用编码规范.md<br>
    │   │   └── 13-用于API文档自动生成的doxygen注释规范.md<br>
    │   ├── 01-doxygen_A// doxygen工具从源码注释中自动生成文档的路<br>
    │   └── LICENSE     // 整个工程的许可<br>
    ├── 1_src            // 源码目录<br>
    │   ├── aDoxygenDemo.h<br>
    │   └── example<br>
    ├── 2_makefileBuild  // 直接使用makefile工具编码时的输出目录<br>
    ├── 3_cmakeBuild     // 使用cmake编译时的输出目录<br>
    ├── 4_automakeBuild  // 使用automake编译时的输出目录（里面有一份automake demo）<br>
    │   ├── compile -> /usr/share/automake-1.15/compile<br>
    │   ├── config.h.in<br>
    │   ├── configure<br>
    │   ├── configure.ac<br>
    │   ├── depcomp -> /usr/share/automake-1.15/depcomp<br>
    │   ├── install-sh -> /usr/share/automake-1.15/install-<br>
    │   ├── main.c<br>
    │   ├── Makefile<br>
    │   ├── Makefile.am<br>
    │   ├── Makefile.in<br>
    │   ├── missing -> /usr/share/automake-1.15/missing<br>
    │   └── stamp-h1<br>
    ├── 5_release        // 编译完成后会将可执行文件和库拷贝到这个目录<br>
    ├── automake.txt     // automake编译工具的简易使用教程<br>
    ├── CMakeLists.txt   // cmake编译工具的配置文件<br>
    ├── Doxyfile         // doxygen使用源码注释生成文档工具的配置文件<br>
    ├── main.c           // 整个工程的入口<br>
    ├── Makefile         // 直接使用makefile编码时的配置文件<br>
    ├── README.md        // 整个工程的说明文档<br>
    └── vimrc<br>
</p>
