/*******************************************************************************
 * Copyleft (c) 2020 将狼才鲸
 *
 * \file    route.c
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

/*================================= 头 文 件 =================================*/
#include <errno.h>
#include <stdio.h>
#include "route.h"

/*================================= 全局变量 =================================*/
/*!
 * \brief 全局的路由表
 *
 * TODO: 复杂的路由结构可以将路由表专门放在一个.c文件中处理
 */
ROUTE_ITEM *route_table[ROUTE_MAX_NUM] = {0};

/*================================= 私有函数 =================================*/
/*!
 * \brief 通过目的ID找到目的模块路由
 *
 * \param[in] dst 目的模块ID
 * \return    找到的目的模块路由指针，未找到则返回NULL
 */
static ROUTE_ITEM *route_find(int dst)
{
    int i;

    for (i = 0; i < ROUTE_MAX_NUM; i++) {
        if (route_table[i]) {
            if (route_table[i]->src == dst)
                return route_table[i];
        }
    }

    pr_err("Can't found route!");
    return NULL;
}

/*================================= 接口函数 =================================*/
/*！
 * \brief 初始化默认路由表
 */
int route_init(void)
{
    //TODO: 注册缺省路由表
    return 0;
}

/*!
 * \brief 销毁路由表
 */
int route_exit(void)
{
    //TODO: 销毁路由表
    return 0;
}

/*！
 * \brief 注册一条路由信息
 *
 * \param[in] route_item 要注册的一项路由
 * \return    0处理正常，负数错误码
 */
int route_register(ROUTE_ITEM *route_item)
{
    int i;

    for (i = 0; i < ROUTE_MAX_NUM; i++) {
        if (route_table[i] == NULL) {
            route_table[i] = route_item;
            break;
        }
    }

    if (i == ROUTE_MAX_NUM) {
        pr_err("Route table full!");
        return -EBUSY; 
    }

    return 0;
}

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
int route_recv(int src, int dst, char *buf, int len)
{
    ROUTE_ITEM *item;

    if (item = route_find(dst)) {
        if (item->send)
            return item->send(src, dst, buf, len);
        else
            pr_err("route send() NULL!");
    }

    return -ESRCH;
}
