#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "module1.h"
#include "route_table.h"
#include "route.h"

static ROUTE_ITEM my_route;

int module1_recv(int src, int dst, char *buf, int len)
{
    //TODO: 参数检查

    if (dst != MODULE1_ID)
        pr_err("%s error! dst id: %d, my id: %d",
               __func__, dst, MODULE1_ID);

    pr_info("【模块1】收到数据：%.*s\n", len, buf);

    return 0;
}

int module1_init(void)
{
    my_route.src = MODULE1_ID; /**< 自己模块的id */
    my_route.dst = MODULE2_ID; /**< 默认要发送到的模块id */
    /** 自己模块收到的数据都要从route转发一次，不管数据最终是传给自己模块还是其它模块的 */
    my_route.recv = route_recv; /**< 将数据送到route转发 */
    my_route.send = module1_recv; /**< 接收从route转发过来的数据 */
    route_register(&my_route);

    return 0;
}

#define MODULE1_UNITEST
int module1_run(void)
{
    //TODO:从链路中获取数据

#ifdef MODULE1_UNITEST
#define _1STR1 "这是【模块1】发送给模块1的数据"
#define _1STR2 "这是【模块1】发送给模块2的数据"
#define _1STR3 "这是【模块1】发送给模块3的数据"
    /** 这里的第二个参数可以用my_route.dst这个默认的目的地址 */
    my_route.recv(MODULE1_ID, MODULE3_ID, _1STR3, strlen(_1STR3) + 1);
    my_route.recv(MODULE1_ID, MODULE2_ID, _1STR2, strlen(_1STR2) + 1);
    my_route.recv(MODULE1_ID, MODULE1_ID, _1STR1, strlen(_1STR1) + 1);
    pr_info("..................");
    sleep(2);
#endif /* MODULE1_UNITEST */

    return 0;
}

int module1_exit(void)
{

    return 0;
}
