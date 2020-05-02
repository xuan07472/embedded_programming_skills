#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

extern int menus_init();
extern void menus_exit();
extern void menus_run();

void Ctrl_C_handle(int arg) {
    menus_exit();
}

int main(int argc, char *argv[]) {
    printf("Time: %s %s\n", __DATE__, __TIME__);

    signal(SIGINT, Ctrl_C_handle);

    menus_init();

    menus_run();

    menus_exit();

	return 0;
}
