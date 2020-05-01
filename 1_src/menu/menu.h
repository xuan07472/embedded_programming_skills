#ifndef _MENU_H
#define _MENU_H

#define MENU_TEXT_LEN       64
#define MENU_DETAIL_LEN     128
#define EXIT_MENU_ID        99

struct menu {
    int id;
    struct menu *submenus;
    struct menu *next;
    char text[MENU_TEXT_LEN];
    void (*action)(struct menu *);
};

#define EXIT_MENU(up)  {   \
    .id = EXIT_MENU_ID,    \
    .submenus = up,        \
    .text = "Go Back",     \
    .action = NULL         \
}

#define MAIN_MENU {     \
    .id = 0,            \
    *next = NULL,       \
    .text = "",         \
    .action = NULL      \
}

#define for_each_menu(m, menus) for (m = menus; m; m = m->next)

void Menu_display(struct menu *menus);

struct menu *Menu_find(struct menu *menus, int id);
struct menu *Menu_select(struct menu *menus, int id);

#endif /* _MENU_H */
