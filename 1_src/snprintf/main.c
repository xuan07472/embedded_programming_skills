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


#define LOG_BUF_LEN (4096 * 6)
char logBuf[LOG_BUF_LEN];
void rx(char *payload, int len, Params *phy, int err, char *dbg) {
    int i;
    int logLen;
    unsigned char *p;
    char *config;

    config = strstr(dbg, "CONFIG:");
    if (config)
        printf("\n\tconfig: %f\n", atof(config + strlen("CONFIG:")));

    logLen = snprintf(logBuf, LOG_BUF_LEN, "** Received:\n");
    logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen,
            "\tError: %d\n", err);
    if (!err) {
        logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen,
                    "\tPayload: ");
        for (i=0, p=payload; i<len; i++, p++) {
            if (isprint(*p))
                logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen,
                                   "%c", *p);
            else
                logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen,
                        "\\x%x%x", *p>>4, *p&0xF);
        }
        logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen, "\n");
    }
    logLen += snprintf(logBuf + logLen, LOG_BUF_LEN - logLen,
                       "\n** Dbg:\n %s\n", dbg);

    printf("%.*s", logLen, logBuf);
    LogWrite(logBuf, logLen);
}
