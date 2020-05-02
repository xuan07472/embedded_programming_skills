#include <stdio.h>
#define NONE    0
#define ERROR   1
#define WARN    2
#define INFO    3
#define ENTRY   4
#define DEBUG   5
#define PRINT_LEVEL DEBUG
#define pr(level, ...)                      \
            do {                            \
                if (level <= PRINT_LEVEL)   \
    printf("["#level"]""{"__DATE__" "__TIME__"} " __VA_ARGS__);                                   \
    printf("\n");                           \
            } while (0)

#define pr_pure(level, ...)                 \
            do {                            \
                if (level <= PRINT_LEVEL)   \
                    printf(__VA_ARGS__);    \
            } while (0)
#define pr_err(...) pr(ERROR, ##__VA_ARGS__)
#define pr_warn(...) pr(WARN, ##__VA_ARGS__)
#define pr_info(...) pr(INFO, ##__VA_ARGS__)
#define pr_entry(inout) pr(ENTRY, "%s() %s", __func__, #inout)
#define pr_dbg pr(DEBUG, ##__VA_ARGS__)

#define pr_err_pure(...) pr_pure(ERROR, ##__VA_ARGS__)
#define pr_warn_pure(...) pr_pure(WARN, ##__VA_ARGS__)
#define pr_info_pure(...) pr_pure(INFO, ##__VA_ARGS__)
#define pr_entry_pure(inout) pr_pure(ENTRY, "%s() %s", __func__, #inout)
#define pr_dbg_pure pr_pure(DEBUG, ##__VA_ARGS__)



int main()
{
    pr_entry(in);
    pr_err("pr_err");
    pr(INFO, "test string %s\n", "test");
    for (int i = 0; i < 10; i++) {
        pr_pure(DEBUG, "%d", i);
    }
}

