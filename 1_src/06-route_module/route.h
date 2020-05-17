#ifndef _ROUTE__H
#define _ROUTE__H

#include "route_table.h"

int route_init(void);
int route_run(void);
int route_exit(void);
int route_recv(int src, int dst, char *buf, int len);
int route_register(ROUTE_ITEM *route_item);

#endif /* _ROUTE__H */
