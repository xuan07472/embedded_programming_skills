参考资料：
http://doxygen.nl/files/doxygen_manual-1.8.17.pdf.zip
https://www.cnblogs.com/etangyushan/p/3753879.html

1、doxygen可以自动对C/C++或其它源码进行分析，将文件/函数头注释和文件名/函数一起生成HTML、PDF或其它格式的API说明文档，前提是按照doxygen的规则编写注释，这样可以省去很多公司里要求你必须做的文档编写时间。

2、从doxygen官网下载软件安装，并下载文档

3、doxygen -g 命令生成默认配置文件
doxygen 命令生成文档，用浏览器打开自动生成的html目录下的index.html文件可以看到API文档

4、可识别的注释块
/**
 * ... Javadoc风格 ...
 */

/*!
 * ... Qt风格 ...
 */

/*!
... 自创风格 ...
*/

///
/// ... C++风格 ...
///

//!
//!... C++风格 ...
//!

/********************************************//**
 * ... 通用风格
 ***********************************************/

/////////////////////////////////////////////////
/// ... 通用风格 ...
/////////////////////////////////////////////////

/************************************************
 * ... 通用风格
 ***********************************************/

5、注释块举例
5.1 、JAVADOC风格
/**
 * 关于cstyle函数的简介
 *
 * 关于cstule函数更详细的介绍，这个函数名由6个英文字母组成，它们分别是c、s、t
 * y、l、e
 *
 * @param theory 参数说明param是关键字，theory是参数名
 *
 */
void cstyle( int theory );

/*******************************************************************************
 * 这是一个神奇的函数。
 *
 * 这个函数有很多功能，比如，它首先名字叫javadocBanner，另外，一般注释用*号画一个
 * 框的话，它的宽度是刚好80个字符，这样既美观又能给你编码时在哪里折行提供一个
 * 可视化的参考。
 *
 * 然后，它有一个int参数。
 *
 * 最后，它没有返回值。
 *
 * 我也不知道这个函数怎么用。
 *
 * @param theory 把所有的参数都要在函数头注释里写出来。
 *
 ******************************************************************************/
void javadocBanner( int theory );

/***************************************************************************//**
 * 函数简述。
 *
 * 函数详述巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦
 * 巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦巴啦。
 * 补充描述哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩哔哩
 * 哔哩哔哩哔哩哔哩哔哩哔哩。
 *
 * @param theory 参数描述
 *
 ******************************************************************************/
void doxygenBanner( int theory );

5.2、使用brief关键字
/*! \brief Brief description.
 *         Brief description continued.
 *
 *
 * Detailed description starts here.
 */

/** Brief description which ends at this dot. Details follow
 * here.
 */

/// Brief description which ends at this dot. Details follow
/// here.

/// Brief description.
/** Detailed description. */

//! Brief description.
//! Detailed description
//! starts here.

//! Brief description, which is
//! really a detailed description since it spans multiple lines.
/*! Another detailed description!
 */

6、单行注释
int var; /*!< Detailed description after the member */
int var; /**< Detailed description after the member */

int var; //!< Detailed description after the member
         //!<

int var; ///< Detailed description after the member
         ///<

int var; //!< Brief description after the member

int var; ///< Brief description after the member

也可以在函数形参注释中表明是输入还是输出
void foo(int v /**< [in] docs for input parameter v. */);

举例：支持的单行注释类型
/*! A test class */

class Afterdoc_Test
{
  public:
    /** An enum type. 
     *  The documentation block cannot be put after the enum! 
     */
    enum EnumType
    {
      int EVal1,     /**< enum value 1 */
      int EVal2      /**< enum value 2 */
    };
    void member();   //!< a member function.
    
  protected:
    int value;       /*!< an integer value */
};

7、支持的注释关键字，关键字如\brief或者@brief以\或者@开头
    \struct to document a C-struct.
    \union to document a union.
    \enum to document an enumeration type.
    \fn to document a function.
    \var to document a variable or typedef or enum value.
    \def to document a #define.
    \typedef to document a type definition.
    \file to document a file.
    \namespace to document a namespace.
    \package to document a Java package.
    \interface to document an IDL interface.

举例：一个.h的C语言头文件注释
/*! \file structcmd.h
    \brief A Documented file.
    
    Details.
*/
 
/*! \def MAX(a,b)
    \brief A macro that returns the maximum of \a a and \a b.
   
    Details.
*/
 
/*! \var typedef unsigned int UINT32
    \brief A type definition for a .
    
    Details.
*/
 
/*! \var int errno
    \brief Contains the last error code.
 
    \warning Not thread safe!
*/
 
/*! \fn int open(const char *pathname,int flags)
    \brief Opens a file descriptor.
 
    \param pathname The name of the descriptor.
    \param flags Opening flags.
*/
 
/*! \fn int close(int fd)
    \brief Closes the file descriptor \a fd.
    \param fd The descriptor to close.
*/
 
/*! \fn size_t write(int fd,const char *buf, size_t count)
    \brief Writes \a count bytes from \a buf to the filedescriptor \a fd.
    \param fd The descriptor to write to.
    \param buf The data buffer to write.
    \param count The number of bytes to write.
*/
 
/*! \fn int read(int fd,char *buf,size_t count)
    \brief Read bytes from a file descriptor.
    \param fd The descriptor to read from.
    \param buf The buffer to read into.
    \param count The number of bytes to read.
*/
 
#define MAX(a,b) (((a)>(b))?(a):(b))
typedef unsigned int UINT32;
int errno;
int open(const char *,int);
int close(int);
size_t write(int,const char *, size_t);
int read(int,char *,size_t);

如果有doxygen识别不了的格式你也想生成文档，可以这样做：
/*! \file myscript.sh
 *  Look at this nice script:
 *  \verbinclude myscript.sh
 */

doxygen支持对Markdown注释，这个很方便


