#include <errno.h>
#include <stdio.h>
#include "route.h"

int route_init(void)
{

    return 0;
}

int route_run(void)
{

    return 0;
}

int route_exit(void)
{

    return 0;
}

ROUTE_ITEM *route_find(int dst)
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

/*!
 * route只有接收接口没有发送接口，
 * route接收接口用于找到目的模块后，调用目的模块的send
 * 函数把数据发送到目的模块
 * 该函数需要是可重入的
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
