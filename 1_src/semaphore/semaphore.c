#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

static pthread_t pid, m_pid;
static sem_t *sem;
static int test_number = 0;
static int flag = 0;

void Ctrl_C_handle(int arg)
{
    printf("Exit ctrl+c.\n");
    sem_unlink("sem");
    pthread_cancel(pid);
    pthread_cancel(m_pid);
    printf("cancel ok, return.\n");
    return;
}

void *recv_thread(void *param)
{
    while(1) {
        if (flag) {
            sleep(2);
            printf("Enter post.\n");
            test_number++;
            sem_post(sem);
            sem_post(sem);
            flag = 0;
        }
    }
    printf("Exit thread");
}

void *main_thread(void *param)
{
    int in;
    struct timespec ts;
    time_t s;
    long ns;
    int ret;

    while(1) {
        printf("!!Please input option number.\n");
        scanf("%d", &in);
        test_number = 0;
        printf("Start wait sem, num:%d.\n", test_number);
#if 0
        flag = 1;
        ret = sem_wait(sem);
#else
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 15;
        flag = 1;
        int tmp = -2;
        sem_getvalue(sem, &tmp);
        printf("Get sem:%d.\n", tmp);
        ret = sem_timedwait(sem, &ts);
#endif
        printf("Wait end, ret:%d, num:%d.\n", ret, test_number);
        printf("====\n");
    }
}


int main(int argc, char *argv[])
{
    printf("Version: 5.\n");

    signal(SIGINT, Ctrl_C_handle);

    sem = sem_open("sem", O_CREAT, 0660, 0);
    pthread_create(&pid, NULL, recv_thread, NULL);
    pthread_create(&m_pid, NULL, main_thread, NULL);
    pthread_join(m_pid, NULL);
    pthread_join(pid, NULL);

    printf("====Exit====.\n");
    return 0;
}
