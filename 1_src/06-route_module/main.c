// 多模块相互之间两两通信的场景

#include <stdio.h>
#include "module1.h"
#include "module2.h"
#include "module3.h"
#include "route.h"

#define MODULES_COUNT 4

typedef int (*FUNC)(void);
typedef struct {
    char *name;
    FUNC init;
    FUNC run;
    FUNC exit;
} MODULE_T;

MODULE_T modules[MODULES_COUNT] = {
    {"模块1", module1_init, module1_run, module1_exit},
    {"模块2", module2_init, module2_run, module2_exit},
    {"模块3", module3_init, module3_run, module3_exit},
    {"路由模块", route_init, NULL, route_exit}
};

int main()
{
    int i;
    int ret;

pr_err("%s enter", __func__);

    /** 初始化 */
    for (i = 0; i < MODULES_COUNT; i++) {
        if (modules[i].init) {
            ret = modules[i].init();
            if (ret) {
                pr_err("Module[%d]%s init error!", i, modules[i].name);
                return ret;
            }
        }
    }

    /** 运行 */
    while (1) {
        for (i = 0; i < MODULES_COUNT; i++) {
            if (modules[i].run)
                modules[i].run();
        }
    }
}
