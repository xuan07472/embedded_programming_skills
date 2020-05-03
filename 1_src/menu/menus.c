#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "menu.h"

#define SCAN_BUF        8192
#define MAX_FILE_LENGTH 128
#define MAX_USER_INPUT  1024

#define INSTRUCTION(fmt, ...)   printf("---- " fmt, ##__VA_ARGS__)
#define RESPONSE(fmt, ...)  printf("\t** :) ***** ;) ** " fmt, ##__VA_ARGS__)

static int scan(char *fmt, void *mem) {
    static char buffer[SCAN_BUF];
    char c;
    int len;

    len = 0;

    while (1) {
        c = getchar();
        if (c == '\n' || c == '\r' || c == EOF)
            break;
        buffer[len++] = c;
    }

    buffer[len] = 0;
    if (fmt[1] == 'd') {
        *(int *)mem =  atoi(buffer);
    }

    if (fmt[1] == 's') {
        strcpy(mem, buffer);
    }

    if (fmt[1] == 'f') {
        *(float *)mem =  atof(buffer);
    }

    return 0;
}

static void mn_tx_wave_action(struct menu *m) {
}

static void mn_tx_burn_in_action(struct menu *m) {
}

static void mn_tx_rand_times_action(struct menu *m) {
}
static void mn_tx_rand_action(struct menu *m) {
}

static void mn_tx_user_action(struct menu *m) {
}
static void mn_tx_phy_action(struct menu *m) {
    INSTRUCTION("Current Phy Params:\n");
    INSTRUCTION("Destination: %d\n", g_phy.dst);
    INSTRUCTION("Type: %d\n", g_phy.type);
    INSTRUCTION("Mode: %d\n", g_phy.mode);
    INSTRUCTION("Guard Time: %d\n", g_phy.guard_time);
    INSTRUCTION("Power Level: %d\n", g_phy.power_level);
}

static struct menu mn_tx_exit = EXIT_MENU(NULL);
static struct menu mn_tx_wave = {
    .id = 6,
    .submenus = NULL,
    .next = &mn_tx_exit,
    .text = "Transmit Wave File",
    .action = mn_tx_wave_action
};
static struct menu mn_tx_burn_in = {
    .id = 5,
    .submenus = NULL,
    .next = &mn_tx_wave,
    .text = "Burn-in with Random Continuous transmission",
    .action = mn_tx_burn_in_action
};
static struct menu mn_tx_rand_times = {
    .id = 4,
    .submenus = NULL,
    .next = &mn_tx_burn_in,
    .text = "Transmit Random Data of N times",
    .action = mn_tx_rand_times_action
};
static struct menu mn_tx_rand = {
    .id = 3,
    .submenus = NULL,
    .next = &mn_tx_rand_times,
    .text = "Transmit Random Data",
    .action = mn_tx_rand_action
};
static struct menu mn_tx_user = {
    .id = 2,
    .submenus = NULL,
    .next = &mn_tx_rand,
    .text = "Transmit User Data",
    .action = mn_tx_user_action
};
static struct menu mn_tx_phy = {
    .id = 1,
    .submenus = NULL,
    .next = &mn_tx_user,
    .text = "Configure Phy Params",
    .action = mn_tx_phy_action
};

static void mn_phy_mode_action(struct menu *m) {
    int mode;
    INSTRUCTION("Regular OFDM modes: 1 - 5\n");
    INSTRUCTION("AUV OFDM modes: 11 - 12\n");
    INSTRUCTION("MFSK modes: 21\n");

    printf("Input mode: ");
    scan("%d", &mode);

    /* mode not checked, intentionally */

    INSTRUCTION("\nMode %d selected\n", mode);
    g_phy.mode = mode;
}

static void mn_phy_type_action(struct menu *m) {
}

static void mn_phy_dst_action(struct menu *m) {
}

static void mn_phy_gdt_action(struct menu *m) {
}

static void mn_phy_pwrlvl_action(struct menu *m) {
}

static struct menu mn_phy_exit = EXIT_MENU(NULL);
static struct menu mn_phy_pwrlvl = {
    .id = 5,
    .submenus = NULL,
    .next = &mn_phy_exit,
    .text = "power level",
    .action = mn_phy_pwrlvl_action
};
static struct menu mn_phy_gdt = {
    .id = 4,
    .submenus = NULL,
    .next = &mn_phy_pwrlvl,
    .text = "guard time",
    .action = mn_phy_gdt_action
};
static struct menu mn_phy_type = {
    .id = 3,
    .submenus = NULL,
    .next = &mn_phy_gdt,
    .text = "type",
    .action = mn_phy_type_action
};
static struct menu mn_phy_mode = {
    .id = 2,
    .submenus = NULL,
    .next = &mn_phy_type,
    .text = "mode",
    .action = mn_phy_mode_action
};
static struct menu mn_phy_dst = {
    .id = 1,
    .submenus = NULL,
    .next = &mn_phy_mode,
    .text = "destination",
    .action = mn_phy_dst_action
};

static void mn_main_exit_action(struct menu *m);
static void mn_locate_action(struct menu *m);
static void mn_range_action(struct menu *m);
static void mn_sleep_action(struct menu *m);
static void mn_version_action(struct menu *m);
static void mn_usbl_action(struct menu *m);

static struct menu mn_main_exit = {
    .id = EXIT_MENU_ID,
    .submenus = NULL,
    .text = "Exit",
    .action = mn_main_exit_action
};
static struct menu mn_main_usbl = {
    .id = 7,
    .submenus = NULL,
    .next = &mn_main_exit,
    .text = "usbl",
    .action = mn_usbl_action
};
static struct menu mn_main_version = {
    .id = 6,
    .submenus = NULL,
    .next = &mn_main_usbl,
    .text = "versions",
    .action = mn_version_action
};
static struct menu mn_main_sleep = {
    .id = 5,
    .submenus = NULL,
    .next = &mn_main_version,
    .text = "deepsleep",
    .action = mn_sleep_action
};
static struct menu mn_main_locate = {
    .id = 4,
    .submenus = NULL,
    .next = &mn_main_sleep,
    .text = "localization",
    .action = mn_locate_action
};
static struct menu mn_main_range = {
    .id = 3,
    .submenus = NULL,
    .next = &mn_main_locate,
    .text = "range",
    .action = mn_range_action
};
static struct menu mn_main_tx = {
    .id = 2,
    .submenus = NULL,
    .next = &mn_main_range,
    .text = "transmission",
    .action = NULL
};
static struct menu mn_main_mreg = {
    .id = 1,
    .submenus = NULL,
    .next = &mn_main_tx,
    .text = "modem registers",
    .action = NULL
};

static void mn_version_action(struct menu *m) {
}

static void mn_sleep_action(struct menu *m) {
}

static void mn_locate_action(struct menu *m) {
}

static void mn_range_action(struct menu *m) {
}
static void mn_usbl_action(struct menu *m) {
}


static int gs_regId, gs_field;
static void mn_reg_write_action(struct menu *m) {
}

static void mn_reg_read_action(struct menu *m) {
}

static void mn_reg_reg_action(struct menu *m) {
}

static void mn_reg_field_action(struct menu *m) {
}

static struct menu *mn_reglist;
static struct menu mn_reg_exit = EXIT_MENU(NULL);
static struct menu mn_reg_write = {
    .id = 2,
    .submenus = NULL,
    .next = NULL,
    .text = "Write Register",
    .action = mn_reg_write_action
};
static struct menu mn_reg_read = {
    .id = 1,
    .submenus = NULL,
    .next = &mn_reg_write,
    .text = "Read Register",
    .action = mn_reg_read_action
};
static struct menu *generate_menu_reglist() {
}

static pthread_t menu_thread_id;
void *menu_thread(void *param) {
    struct menu *cur, *sub;
    int select;
    char t;

    cur = &mn_main_mreg;

    while (1) {
        printf("----------------------------------\n");
        Menu_display(cur);

        printf("\n\tYour Choice -->>> ");
        scan("%d", &select);

        sub = Menu_select(cur, select);
        if (!sub)
            continue;

        if (strcmp(sub->text, "none") == 0)
            sub = Menu_select(sub, 0);

        cur = sub;
    }
    return NULL;
}

void tsMenus_exit();

static void mn_main_exit_action(struct menu *m) {
    tsMenus_exit();
}

int tsMenus_init() {
    mn_reglist = generate_menu_reglist();
    /* level 1 */
    mn_main_mreg.submenus = mn_reglist;
    mn_main_tx.submenus = &mn_tx_phy;

    /* level 2 */
    mn_tx_phy.submenus = &mn_phy_dst;

    mn_tx_exit.submenus = &mn_main_mreg;

    /* level 3 */
    mn_phy_exit.submenus = &mn_tx_phy; /* tricky */

    mn_reg_read.submenus = mn_reglist;
    mn_reg_write.submenus = mn_reglist;

    pthread_create(&menu_thread_id, NULL, menu_thread, NULL);

    return menu_thread_id;
}

void tsMenus_exit() {
    pthread_cancel(menu_thread_id);
}

void tsMenus_run() {
    pthread_join(menu_thread_id, NULL);
}
