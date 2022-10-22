/**
 * \name loop_buffer.c
 * \brief loop buffer, ring buffer
 * \author JiangLangCaiJing
 * \date 2022-08-04
 * \note the logic is based on linux/lib/kfifo.c from linux
 */

#include <stdlib.h> // malloc
#include <string.h> // memcpy

#define EPERM 1

/**
 * \brief struct of loop buffer(ring buffer)
 */
typedef struct loop_buf {
        char *buf;
        unsigned int head;
        unsigned int tail;
        unsigned int size;      /**< buffer total size, unit is unit_size */
        unsigned int unit_size; /**< element unit size, for example 1(bytes) */
        unsigned int remain;    /**< remaining unused space, unit is unit_size */
} LOOP_BUFFER;

/**
 * \brief malloc memory and init loop buffer
 * \param[in] lbuf: loop buffer pointer
 * \param[in] size: buffer total size
 * \return 0 if success, otherwiss fail
 */
int lbuf_malloc(LOOP_BUFFER *lbuf, unsigned int unit_size, unsigned int size)
{
    if (!lbuf || !unit_size ||!size)
        return -EPERM;

    lbuf->buf = malloc(size * unit_size);
    if (!lbuf->buf)
        return -EPERM;

    lbuf->head = lbuf->tail = 0;
    lbuf->size = size;
    lbuf->unit_size = unit_size;
    lbuf->remain = size;

    return 0;
}

/**
 * \brief use a buffer pointer initialize loop buffer
 * \param[in] lbuf: loop buffer pointer
 * \param[in] origin_buf: buffer pointer used for loop buffer
 * \param[in] size: buffer total size
 * \return 0 if success, otherwiss fail
 */
int lbuf_create(LOOP_BUFFER *lbuf, char *origin_buf, unsigned int unit_size, unsigned int size)
{
    if (!lbuf || !origin_buf || !unit_size || !size)
        return -EPERM;

    lbuf->buf = origin_buf;
    lbuf->head = lbuf->tail = 0;
    lbuf->size = size;
    lbuf->unit_size = unit_size;
    lbuf->remain = size;

    return 0;
}

/**
 * \brief write buffer careful
 * \details the data must be total write, otherwise return direct
 * \param[in] lbuf: loop buffer pointer
 * \param[in] wbuf: pointer of wirte data
 * \param[in] wlen: length of write data, unit is unit_size
 * \return 'wlen' if success(the length write and return is the same), otherwiss fail
 */
int lbuf_write_careful(LOOP_BUFFER *lbuf, char *wbuf, unsigned int wlen)
{
    int overflow;

    if (!lbuf || !wbuf || !wlen)
        return -EPERM;

    if (wlen > lbuf->remain)
        return -EPERM;

    overflow = lbuf->head + (wlen - lbuf->size) * lbuf->unit_size;
    if (overflow > 0) {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen * lbuf->unit_size - overflow);
        memcpy(lbuf->buf, wbuf + (wlen * lbuf->unit_size - overflow), overflow);
        //TODO: add lock if multi writer
        lbuf->head = overflow;
    } else {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen * lbuf->unit_size);
        lbuf->head += (wlen * lbuf->unit_size);
    }

    //TODO: add lock if multi writer
    lbuf->remain -= wlen;

    return wlen;
}

/**
 * \brief write buffer
 * \details if the remaining space is less than the write length,
 *          fill the buffer first
 * \param[in] lbuf: loop buffer pointer
 * \param[in] wbuf: pointer of wirte data
 * \param[in] wlen: length of write data, unit is unit_size
 * \return positive number if success(value is length wrote success),
 *         otherwiss fail, the return value maybe not equal to the length want to write
 */
int lbuf_write(LOOP_BUFFER *lbuf, char *wbuf, unsigned int wlen)
{
    int overflow;

    if (!lbuf || !wbuf || !wlen)
        return -EPERM;

    wlen = (wlen > lbuf->remain ? lbuf->remain : wlen);

    overflow = lbuf->head + (wlen - lbuf->size) * lbuf->unit_size;
    if (overflow > 0) {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen * lbuf->unit_size - overflow);
        memcpy(lbuf->buf, wbuf + (wlen * lbuf->unit_size - overflow), overflow);
        //TODO: add lock if multi writer
        lbuf->head = overflow;
    } else {
        memcpy(lbuf->buf + lbuf->head, wbuf, wlen * lbuf->unit_size);
        lbuf->head += (wlen * lbuf->unit_size);
    }

    //TODO: add lock if multi writer
    lbuf->remain -= wlen;

    return wlen;
}

/**
 * \brief read buffer careful
 * \details the data must be total read, otherwise return direct
 * \param[in] lbuf: loop buffer pointer
 * \param[in] rbuf: pointer of read data
 * \param[in] rlen: length of read data, unit is unit_size
 * \return 'rlen' if success(the length read and return is the same), otherwiss fail
 */
int lbuf_read_careful(LOOP_BUFFER *lbuf, char *rbuf, unsigned int rlen)
{
    int overflow;

    if (!lbuf || !rbuf || !rlen)
        return -EPERM;

    if (rlen > (lbuf->size - lbuf->remain))
        return -EPERM;

    overflow = lbuf->tail + (rlen - lbuf->size) * lbuf->unit_size;
    if (overflow > 0) {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen * lbuf->unit_size - overflow);
        memcpy(rbuf + (rlen * lbuf->unit_size - overflow), lbuf->buf, overflow);
        //TODO: add lock if multi reader
        lbuf->tail = overflow;
    } else {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen * lbuf->unit_size);
        lbuf->tail += (rlen * lbuf->unit_size);
    }
    //TODO: add lock if multi reader
    lbuf->remain += rlen;

    return rlen;
}

/**
 * \brief read buffer
 * \details if the remaining data is less than the read length,
 *          read the all remaining data first
 * \param[in] lbuf: loop buffer pointer
 * \param[in] rbuf: pointer of read data
 * \param[in] rlen: length of read data, unit is unit_size
 * \return positive number if success(value is length read success),
 *         otherwiss fail, the return value maybe not equal to the length want to read
 */
int lbuf_read(LOOP_BUFFER *lbuf, char *rbuf, unsigned int rlen)
{
    int overflow;

    if (!lbuf || !rbuf || !rlen)
        return -EPERM;

    rlen = (rlen > (lbuf->size - lbuf->remain) ? (lbuf->size - lbuf->remain): rlen);

    overflow = lbuf->tail + (rlen - lbuf->size) * lbuf->unit_size;
    if (overflow > 0) {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen * lbuf->unit_size - overflow);
        memcpy(rbuf + (rlen * lbuf->unit_size - overflow), lbuf->buf, overflow);
        //TODO: add lock if multi reader
        lbuf->tail = overflow;
    } else {
        memcpy(rbuf, lbuf->buf + lbuf->tail, rlen * lbuf->unit_size);
        lbuf->tail += (rlen * lbuf->unit_size);
    }
    //TODO: add lock if multi writer
    lbuf->remain += rlen;

    return rlen;
}

/**
 * \brief free memory and reinit loop buffer
 * \param[in] lbuf: loop buffer pointer
 * \return 0 if success, otherwiss fail
 */
int lbuf_free(LOOP_BUFFER *lbuf)
{
    free(lbuf->buf);
    lbuf->head = lbuf->tail = 0;
    lbuf->size = 0;
    lbuf->remain = 0;

    return 0;
}

/**
 * \brief reinit loop buffer
 * \param[in] lbuf: loop buffer pointer
 * \return 0 if success, otherwiss fail
 */
int lbuf_destroy(LOOP_BUFFER *lbuf)
{
    lbuf->buf = NULL;
    lbuf->head = lbuf->tail = 0;
    lbuf->size = 0;
    lbuf->remain = 0;

    return 0;
}

#define LOOP_BUFFER_UNITEST

#ifdef LOOP_BUFFER_UNITEST
#include <stdio.h>
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
    lbuf_malloc(&buf, 1, MAX_LEN);

    wlen = MAX_LEN;
    printf("\n==1==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read %d fail!\n", rlen); 
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    wlen = 2;
    printf("\n==2==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    wlen = 4;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    wlen = 6;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    wlen = 1;
    printf("Write[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write[%d] fail!\n", wlen);

    rlen = MAX_LEN - 4;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read %d fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    wlen = 6;
    printf("\n==3==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read[%d] fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);
    rlen = 5;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read %d fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    wlen = 7;
    printf("\n==4==\nWrite[%02d]: %.*s\n", wlen, wlen, wbuf);
    ret = lbuf_write_careful(&buf, wbuf, wlen);
    if (ret != wlen)
        printf("Error: write %d fail!\n", wlen);
    rlen = MAX_LEN;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read %d fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);
    rlen = 1;
    memset(rbuf, 0, sizeof(rbuf));
    ret = lbuf_read_careful(&buf, rbuf, rlen);
    if (ret != rlen)
        printf("Error: read[%d] fail!\n", rlen);
    printf("Read [%02d]: %.*s\n", rlen, rlen, rbuf);

    lbuf_free(&buf);

    return 0;
}
#endif // LOOP_BUFFER_UNITEST
