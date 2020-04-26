/*
 * @file Queue.h
 */
/*
 *  ======== Queue.h ========
 *
 */

/*!
 *  ======== Queue ========
 *  Queue manager.
 *  
 */

/*!
 *  ======== Elem ========
 *  Opaque queue element.
 *
 *  A field of this type is placed at the head of client structs.
 */
typedef struct _Elem {
    struct _Elem *volatile next;
    struct _Elem *volatile prev;
} Queue_Elem;

typedef struct Instance_State {
    Queue_Elem elem;
} Queue_Object;

/*!
 *  @_nodoc
 *  ======== elemClear ========
 *  Clears a Queue element's pointers so that if isQueued() is called on
 *  the element it will return FALSE. When a Queue element is dequeued or
 *  removed from a Queue, this API must be called on the element for 
 *  isQueued() to return FALSE.     
 *
 *  To be clear, this API is not for removing elements from a queue, and
 *  should never be called on an element in a queue--only on dequeued 
 *  elements.
 *
 *  @param(qelem)           element to be cleared
 */
void Queue_elemClear(Queue_Elem *qelem); 

/*!
 *  ======== insert ========
 *  Insert `elem` in the queue in front of `qelem`.
 *
 *  @param(qelem)           element already in queue
 *
 *  @param(elem)            element to be inserted in queue
 */
void Queue_insert(Queue_Elem *qelem, Queue_Elem *elem); 

/*!
 *  ======== next ========
 *  Return next element in queue (non-atomically).
 *
 *  This function returns a pointer to an Elem object in the queue 
 *  after `qelem`. A Queue is represented internally as a doubly-linked
 *  list, so 'next' can be called in a continuous loop over the queue.
 *  See the module description for an example of iterating once over a
 *  Queue.
 *
 *  @param(qelem)           element in queue
 *
 *  @b(returns)             next element in queue
 */
Queue_Elem *Queue_next(Queue_Elem *qelem);

/*!
 *  ======== prev ========
 *  Return previous element in queue (non-atomically).
 *
 *  This function returns a pointer to an Elem object in the queue 
 *  before `qelem`. A Queue is represented internally as a doubly-linked
 *  list, so 'prev' can be called in a continuous loop over the queue.
 *  See the module description for an example of iterating once over a
 *  Queue.
 *
 *  @param(qelem)           element in queue
 *
 *  @b(returns)             previous element in queue
 */
Queue_Elem *Queue_prev(Queue_Elem *qelem);

/*!
 *  ======== remove ========
 *  Remove qelem from middle of queue (non-atomically).
 *
 *  The `qelem` parameter is a pointer to an existing element to be removed
 *  from the Queue.
 *
 *  @param(qelem)           element in queue
 */
void Queue_remove (Queue_Elem *qelem);

/*!
 *  @_nodoc
 *  ======== isQueued ========
 *  Check if the elem is on any queue. 
 *  
 *  In order for this API to return false on an element that has been
 *  dequeued or removed from a Queue, elemClear must have been called on
 *  the element.
 *
 *  @param(qelem)           element in queue
 */
int Queue_isQueued (Queue_Elem *qelem);

/*!
 *  ======== create ========
 *  Create a Queue object
 */
int Queue_init(void);
void Queue_create(Queue_Object *obj);
void Queue_delete(Queue_Object *obj);

/*!
 *  ======== dequeue ========
 *  Remove the element from the front of queue and return elem.
 *
 *  This function removes an element from the front of queue and returns
 *  it. If the queue is empty, the return value of Queue_dequeue() will
 *  be non-NULL, due to the Queue implementation. Use Queue_empty() to
 *  determine whether or not the Queue is empty before calling
 *  Queue_dequeue().
 *
 *  @b(returns)             pointer to former first element
 */
Queue_Elem *Queue_dequeue(Queue_Object *obj);

/*!
 *  ======== empty ========
 *  Test for an empty queue.
 *
 *  @b(returns)             TRUE if this queue is empty
 */
int Queue_empty(Queue_Object *obj);

/*!
 *  ======== enqueue ========
 *  Insert at end of queue (non-atomically).
 *
 *  @param(elem)            pointer to an element
 */
void Queue_enqueue(Queue_Object *obj, Queue_Elem *elem);

/*!
 *  ======== get ========
 *  Get element from front of queue (atomically).
 *
 *  This function removes the element from the front of queue and returns
 *  a pointer to it.  If the queue is empty, the return value of
 *  Queue_get() will be non-NULL, due to the Queue implementation.
 *  Use Queue_empty() to determine whether or not the Queue is empty before
 *  calling Queue_get().
 *
 *  @b(returns)             pointer to former first element
 */
Queue_Elem *Queue_get(Queue_Object *obj);

/*!
 *  ======== getTail ========
 *  Get the element at the end of the queue (atomically).
 *
 *  This function removes the element at the end of queue and returns
 *  a pointer to it.  If the queue is empty, the return value of
 *  Queue_getTail() will be non-NULL, due to the Queue implementation.
 *  Use Queue_empty() to determine whether or not the Queue is empty before
 *  calling Queue_getTail().
 *
 *  @b(returns)             pointer to former end element
 */
Queue_Elem *Queue_getTail(Queue_Object *obj);

/*!
 *  ======== head ========
 *  Return element at front of queue.
 *
 *  This function returns a pointer to the element at the front of queue.
 *  The element is not removed from the queue.
 *  Due to the Queue implementation, the return value will be non-NULL
 *  if the Queue is empty.  Use Queue_empty() to determine whether or
 *  not the Queue is empty before calling Queue_head().
 *
 *  @b(returns)             pointer to first element
 */
Queue_Elem *Queue_head(Queue_Object *obj);

/*!
 *  ======== put ========
 *  Put element at end of queue (atomically).
 *
 *  @param(elem)            pointer to new queue element
 */
void Queue_put(Queue_Object *obj, Queue_Elem *elem);

/*!
 *  ======== putHead ========
 *  Put element at the front of the queue (atomically).
 *
 *  @param(elem)            pointer to new queue element
 */
void Queue_putHead(Queue_Object *obj, Queue_Elem *elem);
