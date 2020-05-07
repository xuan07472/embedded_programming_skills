#define MODULE_COUNT 3
static struct xxx module[MODULE_COUNT];
enum {
    NONE,
    INITIALLISED,
    STARTED,
    RUNNING
};

int main()
{
    int i;
    int ret;

    /** 初始化 */
    for (i = 0; i < MODULE_COUNT; i++) {
        ret = -1; //TODO: 增加错误号
        if (module[i].init)
            ret = module[i].init();
        if (ret >= 0)
            module[i].running_state = INITIALLISED;
    }

    /** 开始准备 */
    for (i = 0; i < MODULE_COUNT; i++) {
        ret = -1; //TODO: 增加错误号
        if (module[i].start && (module[i].running_state == INITIALLISED))
            ret = module[i].start();
        if (ret >= 0)
            module[i].running_state = STARTED;
    }

    /** 运行中 */
    while (1) {
        for (i = 0; i < MODULE_COUNT; i++) {
            if (module[i].run && (module[i].running_state == STARTED))
                module[i].run();
        }
    }
}
