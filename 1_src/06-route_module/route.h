/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    route.h
 * \brief   路由模块
 * \author  将狼才鲸
 * \version 1.0.0
 * \date    2020-05-17
 * \license MulanPSL-1.0
 *
 * -----------------------------------------------------------------------------
 *
 * -----------------------------------------------------------------------------
 * 文件修改历史：
 * <时间>       | <版本>    | <作者>    | <描述>
 * 2020-05-17   | v1.0.0    | 将狼才鲸  | 创建文件
 * -----------------------------------------------------------------------------
 ******************************************************************************/

#ifndef _ROUTE__H
#define _ROUTE__H

/*!
 * 临时定义调试级别输出，整合进整个工程时将此删掉，并包含print_ctrl.h
 */
#define pr_err(...) \
            printf(__VA_ARGS__);\
            printf("\n");

#define pr_info(...) \
            printf(__VA_ARGS__);\
            printf("\n");

/*================================= 宏 定 义 =================================*/
/*!
 * 开启路由模块单元测试
 */
#define MODULE1_UNITEST
#define MODULE2_UNITEST
#define MODULE3_UNITEST

/*!
 * 定义模块ID
 */
#define ROUTE_MAX_NUM   10
#define NONE_ID         0
#define MODULE1_ID      1
#define MODULE2_ID      2
#define MODULE3_ID      3

/*================================= 结 构 体 =================================*/
/*!
 * \brief 路由结构体
 *
 * 模块要转发数据时，从recv接口进入到路由，该接口里会通过dst查找目的模块，
 * 并通过调用目的模块的sendh接口实现将数据从一个模块传输到另一个模块
 */
typedef struct {
    int src;    /**< 源模块ID */
    int dst;    /**< 目的模块ID */
    /** 路由进入接口 */
    int (*recv)(int src, int dst, char *buf, int len);
    /** 目的模块接收接口 */
    int (*send)(int src, int dst, char *buf, int len);
} ROUTE_ITEM;

/*================================= 全局变量 =================================*/
/*!
 * \brief 全局的路由表
 *
 * TODO: 复杂的路由结构可以将路由表专门放在一个.c文件中处理
 */
extern ROUTE_ITEM *route_table[ROUTE_MAX_NUM];

/*================================= 接口函数 =================================*/
/*！
 * \brief 初始化默认路由表
 */
int route_init(void);

/*!
 * \brief 销毁路由表
 */
int route_exit(void);

/*!
 * \brief 路由接收，转发数据到目的模块
 *
 * route只有接收接口没有发送接口，
 * route接收接口用于找到目的模块后，调用目的模块的send
 * 函数把数据发送到目的模块。
 * 该函数需要是可重入的
 *
 * \param[in] src 源模块ID，可根据此ID做一些额外的操作，如按源模块格式进行解包分析
 * \param[in] dst 目的模块ID，根据此找到目的模块路由，并调用目的模块的recv接口
 * \param[in] buf 输出的数据，可以是特定格式的数据，并在此接口中解析数据
 * \param[in] len 输入的数据长度
 * \return    0处理正常，负数错误码
 */
int route_recv(int src, int dst, char *buf, int len);

/*！
 * \brief 注册一条路由信息
 *
 * \param[in] route_item 要注册的一项路由
 * \return    0处理正常，负数错误码
 */
int route_register(ROUTE_ITEM *route_item);

#endif /* _ROUTE__H */
