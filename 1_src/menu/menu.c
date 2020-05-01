#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"

void Menu_display(struct menu *menus) {
    struct menu *m;

    for_each_menu(m, menus)
        printf("%d.\t%s\n", m->id, m->text);

    return;
}

struct menu *Menu_find(struct menu *menus, int id) {
    static struct menu *m;

    for_each_menu(m, menus)
        if (m->id == id)
            break;

    return m;
}

struct menu *Menu_select(struct menu *menus, int id) {
    static struct menu *m;

    m = Menu_find(menus, id);
    if (!m) {
        printf("!!! Match failed, please check your input.\n");
        return NULL;
    }

    if (m->action)
        m->action(m);

    return m->submenus;
}
