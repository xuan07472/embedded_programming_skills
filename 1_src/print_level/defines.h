#ifndef ADDEFINE_H
#define ADDEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "merrno.h"
#include "stddef.h"

#define DBG_ERROR         9
#define DBG_WARNING       6
#define DBG_INFO          3
#define DBG_DETAIL        2
#define ERROR       "Error: "
#define WARN        "Warning: "
#define INFO        "Info: "
#define DETAIL      "Detail: "

extern int G_DEBUG_LEVEL;
#define DEFINE_PROG_WIDE_DEBUG_LEVEL \
            int G_DEBUG_LEVEL = CONFIG_DEBUG_LEVEL

#ifdef CONFIG_ARM
#include "stdio.h"
#define print   printf
#else
    #ifdef CONFIG_DSP
    #include <xdc/runtime/System.h>
    #define print System_printf
    #else
        #error "Platform has not been selected: ARM/DSP"
    #endif
#endif

#define dbg_print(lvl, prefix, fmt, ...) do {               \
    if ((lvl) >= G_DEBUG_LEVEL)                             \
        print(prefix fmt, ##__VA_ARGS__);                   \
} while (0)

#define pr_error(fmt, ...) dbg_print(DBG_ERROR, ERROR, fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...) dbg_print(DBG_WARNING, WARN, fmt, ##__VA_ARGS__)
#define pr_info(fmt, ...) dbg_print(DBG_INFO, INFO, fmt, ##__VA_ARGS__)
#define pr_detail(fmt, ...) dbg_print(DBG_DETAIL, DETAIL, fmt, ##__VA_ARGS__)

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type, member) );})

#define other_member(ptr, type, self, member) ({        \
        (type *) obj = container_of(ptr, type, self);   \
        &obj->member; })

#ifdef __cplusplus
}
#endif

#endif
