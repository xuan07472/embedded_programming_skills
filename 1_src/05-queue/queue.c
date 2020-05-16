#include <stdio.h>
#include <string.h>

typedef struct _ELEM {
    struct _ELEM *next;
    struct _ELEM *prev;
} ELEM;

void queue_create(ELEM *elem)
{
    elem->next = elem->prev = elem;
}
void queue_put(ELEM *obj, ELEM *elem)
{
    elem->next = obj;
    elem->prev = obj->prev;

    (obj->prev)->next = elem;
    obj->prev = elem;
}

ELEM *queue_get(ELEM *obj)
{
    ELEM *elem;
    
    elem = obj->next;
    obj->next = elem->next;
    elem->next->prev = obj;

    return elem;
}

int queue_empty(ELEM *obj)
{
    return (obj->next == obj);
}

/** 单元测试 */
typedef struct _config {
    ELEM elem;
    int payload;
} CONFIG_T;
#define pr_info printf
#define MAX_CONF 16
int main(int argc, char *argv[])
{
    int i;
    ELEM queue_obj;
    ELEM *elem_ret;
    CONFIG_T config[MAX_CONF];

    pr_info("%s\n", argv[0]);
        
    memset(&config, 0, sizeof(config));
    queue_create(&queue_obj);
    for (i = 0; i < MAX_CONF; i++) {
        config[i].payload = i;
        queue_put(&queue_obj, &config[i].elem);
    }
    for (i = 0; i < MAX_CONF; i++) {
        if (!queue_empty(&queue_obj)) {
            elem_ret = queue_get(&queue_obj);
            pr_info("==%d\n", ((CONFIG_T *)elem_ret)->payload);
        }
    }

    return 0;    
}

