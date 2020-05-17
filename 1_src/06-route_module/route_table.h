#ifndef _ROUTE_TABLE__H
#define _ROUTE_TABLE__H

#define pr_err(...) \
            printf(__VA_ARGS__);\
            printf("\n");

#define pr_info(...) \
            printf(__VA_ARGS__);\
            printf("\n");

#define NONE_ID         0
#define MODULE1_ID      1
#define MODULE2_ID      2
#define MODULE3_ID      3
#define ROUTE_MAX_NUM   10

/*!
 * 一个模块从recv接口进入到路由，查找目的模块，并通过调用目的模块的recv实现将数据从一个模块传输到另一个模块
 */
typedef struct {
    int src;
    int dst;
    int (*send)(int src, int dst, char *buf, int len);
    int (*recv)(int src, int dst, char *buf, int len);
} ROUTE_ITEM;

extern ROUTE_ITEM *route_table[ROUTE_MAX_NUM];

#endif /* _ROUTE_TABLE__H */
