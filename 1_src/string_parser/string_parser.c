/*******************************************************************************
 * copyrigt
 *
 * @file    string_parser.c
 * @brief   输入一个ASCII字符串，识别字符串中的CTD数据并将数据返回
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "defines.h"
#include "string_parser.h"
#include "merrno.h"

typedef enum _str_type {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DATE,
    TYPE_TIME
} STR_TYPE_M;

/**!
 * @brief 切分CTD数据字符串
 */
int ctd_str_tok(const char *i_str, unsigned int i_len, char delim)
{
    char *p_str;

    printf("s==:%s \n", strtok(i_str, &delim));
    while(p_str = strtok(NULL, &delim))
        printf("==:%s \n", p_str);

    return 0;
}

/**!
 * @brief 获取字符串的类型：整形、浮点型、时间
 */
#define name2str(name) (#name)
STR_TYPE_M get_string_type(char *str, int num)
{
    char *ret_str;
    STR_TYPE_M ret = TYPE_INT;
    const char match_alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char match_number[] = "0123456789";
    const char match_time[] = ":";

    pr_detail("\n[==>%d]%s\n", num, str);

    ret_str = strpbrk(str, match_alphabet);
    if (ret_str) {
        ret = TYPE_DATE;
        pr_detail("Type: %s, match: %c\n", name2str(TYPE_DATE), *ret_str);
        goto _exit;
    }

    ret_str = strpbrk(str, match_time);
    if (ret_str) {
        ret = TYPE_TIME;
        pr_detail("Type: %s, match: %c\n", name2str(TYPE_TIME), *ret_str);
        goto _exit;
    }

    ret_str = strpbrk(str, match_number);
    if (ret_str) {
        ret = TYPE_FLOAT;
        pr_detail("Type: %s, match: %c\n", name2str(TYPE_FLOAT), *ret_str);
        goto _exit;
    }

_exit:
    return ret;
}

/*
strptime()参数说明:
%y 两位数的年份表示（00-99）
%Y 四位数的年份表示（000-9999）
%m 月份（01-12）
%d 月内中的一天（0-31）
%H 24小时制小时数（0-23）
%I 12小时制小时数（01-12）
%M 分钟数（00=59）
%S 秒（00-59）
%a 本地简化星期名称
%A 本地完整星期名称
%b 本地简化的月份名称
%B 本地完整的月份名称
%c 本地相应的日期表示和时间表示
%j 年内的一天（001-366）
%p 本地A.M.或P.M.的等价符
%U 一年中的星期数（00-53）星期天为星期的开始
%w 星期（0-6），星期天为星期的开始
%W 一年中的星期数（00-53）星期一为星期的开始
%x 本地相应的日期表示
%X 本地相应的时间表示
%Z 当前时区的名称
*/

/**!
 * @brief o_buf输出数据格式为：
 *           _______________________________________________________
 *    Byte: |B0|B1|B2|B3|B4|B5|B6|B7|B8|B9|.|.|.|.|Bn-3|Bn-2|Bn-1|Bn|
 *   Value: |Flag |Float value|Float value|.......|  Time int value |
 *           -------------------------------------------------------
 */
int put_data(char *p_str, unsigned int num, STR_TYPE_M type,
             char *o_buf, unsigned int o_buflen)
{
    float *data = (float *)(o_buf + sizeof(short int));
    short int *flag = (short int *)o_buf;
    short int temp;
    char *p_ret;
    struct tm tb;
    int seconds;
    static char time[64];
    const char time_fmt[] = "%d %b %Y %H:%M:%S";

    pr_detail("p_str: %s\n", p_str);

    if (0 == num)
        memset(flag, 0, sizeof(short int));

    switch (type) {
    case TYPE_INT:
        /* Flow down */
    case TYPE_FLOAT:
        *(data + num) = (float)atof(p_str);
        temp = *flag;
        temp |= (1 << num);
        *flag = temp;
        break;
    case TYPE_DATE:
        memset(time, 0, sizeof(time));
        strcpy(time, p_str);
        strcat(time, " ");
        break;
    case TYPE_TIME:
        strcat(time, p_str);
        p_ret = strptime(time, time_fmt, &tb);
        if (p_ret)
            pr_detail("Get time ok\n");
        else {
            pr_error("String to time fail!\n");
            return -MEBADE;
        }
        seconds = (int)mktime(&tb);
        *(int *)(data + num - 1) = seconds;
        temp = *flag;
        temp |= (1 << (num - 1));
        *flag = temp;
    }

    return 0;
}


/**！
 * @brief 字符串转成数组
 */
int string_token(const char *i_str, unsigned int i_len, char delim,
                 char *o_buf, unsigned int o_buflen)
{
    int ret = 0;
    int i;
    unsigned int num = 0;
    unsigned int put_len = 0;
    short int flag;
    char *p_str;
    STR_TYPE_M type;

    struct tm *p_tm;
    char time_buf[128]; 
    char fmt[] = "%Y-%m-%d %H:%M:%S";

    pr_detail("\n");

    memset(o_buf, 0, o_buflen);
    p_str = strtok(i_str, &delim);
    type = get_string_type(p_str, num);
    put_len += put_data(p_str, num, type, o_buf, o_buflen);
    num++;
    while (p_str = strtok(NULL, &delim)) {
        type = get_string_type(p_str, num);
        put_len += put_data(p_str, num, type, o_buf, o_buflen);
        num++;
        /** time struct is the end of string, ignore all data after time */
        if (TYPE_TIME == type)
            break;
    }

    pr_detail("\n");

    flag = *(short int *)o_buf;
    o_buf += sizeof(short int);
    pr_detail("Flag: 0x%08x\n", flag);

    /** 求flag里面bit为1的个数 */
    num = 0;
    for (; flag; num++)
        flag &= (flag - 1);

    for(i = 0; i < num; i++) {
        if (i == (num - 1)) {
            /** 时间字符串打印输出测试 */
            p_tm = localtime((time_t *)o_buf);
            memset(time_buf, 0, sizeof(time_buf));
            strftime(time_buf, sizeof(time_buf), fmt, p_tm);

            pr_detail("The time seconds is: %d, date: %s\n", *(int *)o_buf, time_buf);
        } else
            pr_detail("Float value: %f\n", *(float *)o_buf);

        o_buf += sizeof(float);
    }

    return ret;
}

/************************************Unit test*********************************/
#ifdef  STRING_PARSER_UNITTEST
#define PARAM_MAX   2

/**!
 * @brief 单元测试
 */

/**!
 * @brief 终端输出测试test1
 */
int test1_print(void)
{
    pr_info("Unit test: %s ok\n", __func__);
}

/**!
 * @brief 队列测试test2
 */
#define PACKET_MAX  32
static CTD_PACKET_T ctdPackets[PACKET_MAX];
int test2_queue(void)
{
    int i;
    int ret = TRUE;
    Queue_Object q;
    CTD_PACKET_T *packet;

    Queue_init();
    Queue_create(&q);
    memset(ctdPackets, 0, sizeof(ctdPackets));
    for (i = 0; i < PACKET_MAX; i++) {
        ctdPackets[i].payload = i;
        Queue_put(&q, (Queue_Elem *)&ctdPackets[i]);
    }

    for (i = 0; i < PACKET_MAX; i++) {
        if (!Queue_empty(&q)) {
            packet = (CTD_PACKET_T *)Queue_get(&q);
            pr_info("Payload: :%d\n", packet->payload);
        } else {
            pr_info("Get packet fail!\n");
        }
        if (packet->payload != i)
            ret = FALSE;
    }
    Queue_delete(&q);

    if (ret) 
        pr_info("Unit test: %s ok\n", __func__);
    else
        pr_info("Unit test: %s FAIL!\n", __func__);
}

/**!
 * @brief 切分字符串
 */
int test3_strtok(void)
{
    char s[] = "111,222,333 , 444 , 555,666,777,888";
    char *delim = ",";
    char *p;

    printf("s==:%s \n", strtok(s, delim));
    while(p = strtok(NULL, delim))
        printf("==:%s \n", p);

    return 0;
}

/**!
 * @brief 判断字符串中没有特定字符存在
 */
int test4_strpbrk(void)
{
    const char str1[] = "111,222,333 , 444 , 555,666,777,888";
    const char str2[] = "abcdefg";
    char *ret;

    ret = strpbrk(str1, str2);
    if (ret) {
        printf("第一个匹配的字符是： %c\n", *ret);
    } else {
        printf("未找到字符\n");
    }

    return(0);
}

/**
 * @brief日期字符串转成时间戳
 */
#include <time.h>
int test5_timeToInt(void)
{
    char *ret;
    struct tm tb;
    char obuf[128];
    char ibuf[] = "2020-04-19 02:06:24";
    char fmt[] = "%Y-%m-%d %H:%M:%S";
    char ibuf2[] = "6 Dec 2001 12:33:45";
    char fmt2[] = "%d %b %Y %H:%M:%S";

    ret = strptime(ibuf2, fmt2, &tb);
    if (ret)
        pr_info("Test pass\n");
    else
        pr_error("Test fail!\n");

    memset(obuf, 0, sizeof(obuf));
    strftime(obuf, sizeof(obuf), fmt, &tb);
    pr_info("%s\n", obuf);

    return 0;
}

/**!
 * @brief 单元测试程序入口
 */
int main(int argc, char *argv[])
{
    int param;

    if (argc < PARAM_MAX) {
        pr_error("%s need %d param: 1print 2queue 3strtok 4strpbrk 5timeToInt!\n", argv[0], PARAM_MAX);
        exit(0);
    }

    pr_info("Program name:%s, param:%s\n", argv[0], argv[1]);
    param = atoi(argv[1]);
    switch (param) {
    case 1:
        test1_print();
        break;
    case 2:
        test2_queue();
        break;
    case 3:
        test3_strtok();
        break;
    case 4:
        test4_strpbrk();
        break;
    case 5:
        test5_timeToInt();
        break;
    default:
        break;

    }

    return 0;
}
#endif /* STRING_PARSER_UNITTEST */
