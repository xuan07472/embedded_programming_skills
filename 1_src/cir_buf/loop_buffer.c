#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _loop_buf {
        char *buf;
        unsigned int head;
        unsigned int tail;
        unsigned int size;   /**< 4 */
        unsigned int remain; /**< 剩余空间 */
} LOOP_BUFFER;

int buffer_create(LOOP_BUFFER *lbuf, unsigned int size)
{
    lbuf->buf = malloc(size);
    lbuf->head = lbuf->tail = 0;
    lbuf->size = size;
    lbuf->remain = size;

    return 0;
}

int buffer_write(LOOP_BUFFER *lbuf, char *wbuf, unsigned int wlen)
{
    int overflow;

    if (wlen > lbuf->remain)
        return 0;

    overflow = lbuf->head + wlen - lbuf->size;
    if (overflow > 0) {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen - overflow);
        memcpy(lbuf->buf, wbuf + (wlen - overflow), overflow);
        //TODO: 如果会同时写，则lock
        lbuf->head = overflow;
    } else {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen);
        lbuf->head += wlen;
    }

    //TODO: lock
    lbuf->remain -= wlen;

    return wlen;
}

int buffer_read(LOOP_BUFFER *lbuf, char *rbuf, unsigned int rlen)
{
    int overflow;

    if (rlen > (lbuf->size - lbuf->remain))
        return 0;

    overflow = lbuf->tail + rlen - lbuf->size;
    if (overflow > 0) {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen - overflow);
        memcpy(rbuf + (rlen - overflow), lbuf->buf, overflow);
        //TODO: 如果会同时读，则lock
        lbuf->tail = overflow;
    } else {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen);
        lbuf->tail += rlen;
    }
    //TODO: lock
    lbuf->remain += rlen;

    return rlen;
}

int buffer_free(LOOP_BUFFER *lbuf)
{
    free(lbuf->buf);
    lbuf->head = lbuf->tail = 0;

    return 0;
}

#define MAX_LEN 12
int main()
{
    LOOP_BUFFER buf;
    char rbuf[MAX_LEN + 4];
    char wbuf[] = "0123456789abcdef";
    int ret;
    unsigned int rlen;
    unsigned int wlen;

    printf("循环缓存最大长度%d\n", MAX_LEN);
    buffer_create(&buf, MAX_LEN);

    wlen = MAX_LEN;
    printf("\n==1==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = buffer_read(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read fail!\n"); 
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    wlen = 2;
    printf("\n==2==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    wlen = 4;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    wlen = 6;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    wlen = 1;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write[%d] fail!\n", wlen);

    rlen = MAX_LEN - 4;
    memset(rbuf, 0, sizeof(rbuf));
    ret = buffer_read(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read[%d] fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    wlen = 6;
    printf("\n==3==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = buffer_read(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read fail!\n");
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);
    rlen = 5;
    memset(rbuf, 0, sizeof(rbuf));
    ret = buffer_read(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read fail!\n");
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);
    wlen = 7;
    printf("\n==3==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = buffer_write(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write fail!\n");
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = buffer_read(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read fail!\n");
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);
 

    return 0;
}
