#ifndef _ROUTE_TABLE__H
#define _ROUTE_TABLE__H

#define pr_err(...) \
            printf(__VA_ARGS__);\
            printf("\n");

#define NONE_ID         0
#define MODULE1_ID      1
#define MODULE2_ID      2
#define MODULE3_ID      3
#define ROUTE_MAX_NUM   10

typedef struct {
    int src;
    int dst;
    int (*send)(int src, int dst, char *buf, int len);
    int (*recv)(int src, int dst, char *buf, int len);
} ROUTE_ITEM;

extern ROUTE_ITEM *route_table[ROUTE_MAX_NUM];

#endif /* _ROUTE_TABLE__H */
