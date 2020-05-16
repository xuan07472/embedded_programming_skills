#include "module2.h"
#include "route_table.h"
#include "route.h"

static ROUTE_ITEM my_route;

int module2_init(void)
{
    my_route.src = MODULE2_ID;
    my_route.dst = MODULE3_ID;
    my_route.send = route_send;
    my_route.recv = route_recv;
    route_register(&my_route);

    return 0;
}

int module2_run(void)
{

    return 0;
}

int module2_exit(void)
{

    return 0;
}
