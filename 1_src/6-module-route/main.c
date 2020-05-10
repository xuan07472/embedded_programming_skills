// 适用于无操作系统，无线程同步，但有多模块相互之间两两通信的场景

#include <stdio.h>

#define MODULES_COUNT 3
#define ECOMM       (-1)
#define pr_err printf

typedef int (*FUNC)(void);
typedef struct {
    char *name;
    FUNC init;
    FUNC run;
    FUNC exit;
} MODULE_T;

MODULE_T modules[MODULES_COUNT] = {
    {"模块1", module1_init, module1_run, module1_exit},
    {"模块1", module2_init, module2_run, module2_exit},
    {"模块1", module3_init, module3_run, module3_exit}
};

int main()
{
    int i;
    int ret;

    /** 初始化 */
    for (i = 0; i < MODULES_COUNT; i++) {
        ret = ECOMM;
        if (modules[i].init)
            ret = modules[i].init();
        if (ret) {
            pr_err("Module[%d]%s init error!\n", i, modules[i].name);
            return ECOMM;
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
