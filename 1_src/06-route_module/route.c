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

int route_send(int src, int dst, char *buf, int len)
{

}

int route_recv(int src, int dst, char *buf, int len)
{

}

ROUTE_ITEM *route_find(int dst)
{
    int i;

    for (i = 0; i < ROUTE_MAX_NUM; i++) {
        if (!route_table[i]) {
            if (route_table[i]->src == dst)
                return route_table[i];
        }
    }

    return NULL;
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
