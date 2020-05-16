#include "module1.h"
#include "route_table.h"
#include "route.h"

static ROUTE_ITEM my_route;

int module1_init(void)
{
    my_route.src = MODULE1_ID;
    my_route.dst = MODULE2_ID;
    my_route.send = route_send;
    my_route.recv = route_recv;
    route_register(&my_route);

    return 0;
}

int module1_run(void)
{
    return 0;

}

int module1_exit(void)
{

    return 0;
}
