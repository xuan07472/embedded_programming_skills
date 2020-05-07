/*
 * Copied from TI SYS/BIOS.
 *
 *  ======== Queue.c ========
 */

#include <pthread.h>
#include "queue.h"
#include "merrno.h"

pthread_spinlock_t mutex;

/*
 *  ======== init ========
 */
int Queue_init(void)
{
    pthread_spin_init(&mutex, 0);
}

/*
 *  ======== create ========
 */
void Queue_create(Queue_Object *obj)
{
    /** 第一个元素不算有效数据 */
    obj->elem.next = obj->elem.prev = &(obj->elem);
}

/*
 *  ======== empty ========
 */
int Queue_empty(Queue_Object *obj)
{
    return (obj->elem.next == &(obj->elem));
}

/*
 *  ======== get ========
 */
Queue_Elem *Queue_get(Queue_Object *obj)
{
    Queue_Elem *elem;

    pthread_spin_lock(&mutex);

    elem = obj->elem.next;

    /** obj->elem.next是队头元素，抽出队头元素 */
    obj->elem.next = elem->next;
    elem->next->prev = &(obj->elem);

    pthread_spin_unlock(&mutex);

    return (elem);

}

/*
 *  ======== put ========
 */
void Queue_put(Queue_Object *obj, Queue_Elem *elem)
{
    pthread_spin_lock(&mutex);

    elem->next = &(obj->elem);
    elem->prev = obj->elem.prev;
    /** obj->elem.prev是队尾元素，在队尾元素后面插入一个 */
    obj->elem.prev->next = elem;
    obj->elem.prev = elem;

    pthread_spin_unlock(&mutex);
}

/*
 *  ======== delete ========
 */
void Queue_delete(Queue_Object *obj)
{
    obj->elem.next = obj->elem.prev = &(obj->elem);
}

/** 以下为非常用接口或者兼容接口 */
/*
 *  ======== dequeue ========
 */
Queue_Elem *Queue_dequeue(Queue_Object *obj)
{
    Queue_Elem *elem;
    Queue_Elem *next;

    elem = obj->elem.next;
    next = elem->next;
    obj->elem.next = next;
    next->prev = &(obj->elem);

    return (elem);
}

/*
 *  ======== enqueue ========
 */
void Queue_enqueue(Queue_Object *obj, Queue_Elem *elem)
{
    Queue_Elem *prev;

    prev = obj->elem.prev;

    elem->next = &(obj->elem);
    elem->prev = prev;
    prev->next = elem;
    obj->elem.prev = elem;
}

/*
 *  ======== getTail ========
 */
Queue_Elem *Queue_getTail(Queue_Object *obj)
{
    Queue_Elem *elem;

    pthread_spin_lock(&mutex);

    elem = obj->elem.prev;

    obj->elem.prev = elem->prev;
    elem->prev->next = &(obj->elem);

    pthread_spin_unlock(&mutex);

    return (elem);

}

/*
 *  ======== head ========
 */
Queue_Elem *Queue_head(Queue_Object *obj)
{
    return (obj->elem.next);
}

/*
 *  ======== elemClear ========
 */
void Queue_elemClear(Queue_Elem *qelem)
{
    qelem->next = qelem->prev = qelem;
}

/*
 *  ======== insert ========
 */
void Queue_insert(Queue_Elem *qelem, Queue_Elem *elem)
{
    Queue_enqueue((Queue_Object *)qelem, elem);
}

/*
 *  ======== next ========
 */
Queue_Elem *Queue_next(Queue_Elem *qelem)
{
    return (qelem->next);
}

/*
 *  ======== prev ========
 */
Queue_Elem *Queue_prev(Queue_Elem *qelem)
{
    return (qelem->prev);
}

/*
 *  ======== putHead ========
 */
void Queue_putHead(Queue_Object *obj, Queue_Elem *elem)
{
    pthread_spin_lock(&mutex);

    elem->prev = &(obj->elem);
    elem->next = obj->elem.next;
    obj->elem.next->prev = elem;
    obj->elem.next = elem;

    pthread_spin_unlock(&mutex);
}

/*
 *  ======== remove ========
 */
void Queue_remove(Queue_Elem *qelem) 
{
    qelem->prev->next = qelem->next;
    qelem->next->prev = qelem->prev;
}

/*
 *  ======== isQueued ========
 */
int Queue_isQueued(Queue_Elem *qelem) 
{
    if ((qelem->prev == qelem) && (qelem->next == qelem)) {
        return (-MENODATA);
    }
    else {
        return (0);
    }
}
