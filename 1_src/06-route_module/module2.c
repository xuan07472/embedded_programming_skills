#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "module2.h"
#include "route_table.h"
#include "route.h"

static ROUTE_ITEM my_route;

int module2_recv(int src, int dst, char *buf, int len)
{
    //TODO: 参数检查

    if (dst != MODULE2_ID)
        pr_err("%s error! dst id: %d, my id: %d",
               __func__, dst, MODULE2_ID);

    pr_info("【模块2】收到数据：%.*s\n", len, buf);

    return 0;
}

int module2_init(void)
{
    my_route.src = MODULE2_ID;
    my_route.dst = MODULE3_ID;
    my_route.recv = route_recv;
    my_route.send = module2_recv;
    route_register(&my_route);

    return 0;
}

#define MODULE2_UNITEST
int module2_run(void)
{
    //TODO:从链路中获取数据

#ifdef MODULE2_UNITEST
#define _2STR1 "这是【模块2】发送给模块1的数据"
#define _2STR2 "这是【模块2】发送给模块2的数据"
#define _2STR3 "这是【模块2】发送给模块3的数据"
    my_route.recv(MODULE2_ID, MODULE3_ID, _2STR3, strlen(_2STR3) + 1);
    my_route.recv(MODULE2_ID, MODULE2_ID, _2STR2, strlen(_2STR2) + 1);
    my_route.recv(MODULE2_ID, MODULE1_ID, _2STR1, strlen(_2STR1) + 1);
    pr_info("~~~~~~~~~~~~~~~~~~");
    sleep(2);
#endif /* MODULE2_UNITEST */

    return 0;
}

int module2_exit(void)
{

    return 0;
}
