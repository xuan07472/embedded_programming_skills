#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "module3.h"
#include "route_table.h"
#include "route.h"

static ROUTE_ITEM my_route;

int module3_recv(int src, int dst, char *buf, int len)
{
    //TODO: 参数检查

    if (dst != MODULE3_ID)
        pr_err("%s error! dst id: %d, my id: %d",
               __func__, dst, MODULE3_ID);

    pr_info("【模块3】收到数据：%.*s\n", len, buf);

    return 0;
}

int module3_init(void)
{
    my_route.src = MODULE3_ID;
    my_route.dst = MODULE1_ID;
    my_route.recv = route_recv;
    my_route.send = module3_recv;
    route_register(&my_route);

    return 0;
}

#define MODULE3_UNITEST
int module3_run(void)
{
    //TODO:从链路中获取数据

#ifdef MODULE3_UNITEST
#define _3STR1 "这是【模块3】发送给模块1的数据"
#define _3STR2 "这是【模块3】发送给模块2的数据"
#define _3STR3 "这是【模块3】发送给模块3的数据"
    my_route.recv(MODULE3_ID, MODULE1_ID, _3STR1, strlen(_3STR1) + 1);
    my_route.recv(MODULE3_ID, MODULE2_ID, _3STR2, strlen(_3STR2) + 1);
    my_route.recv(MODULE3_ID, MODULE3_ID, _3STR3, strlen(_3STR1) + 1);
    pr_info("==================");
    sleep(2);
#endif /* MODULE3_UNITEST */

    return 0;
}

int module3_exit(void)
{

    return 0;
}
