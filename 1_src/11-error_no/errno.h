/** Copied from linux code */
/** 这里只是用来参考，没有实际使用，错误号是直接用的linux自带的errno.h文件 */

#ifndef MERRNO_H
#define MERRNO_H

#define MEPERM            1      /* Operation not permitted */
#define MENOENT           2      /* No such file or directory */
#define MESRCH            3      /* No such process */
#define MEINTR            4      /* Interrupted system call */
#define MEIO              5      /* I/O error */
#define MENXIO            6      /* No such device or address */
#define ME2BIG            7      /* Arg list too long */
#define MENOEXEC          8      /* MExec format error */
#define MEBADF            9      /* Bad file number */
#define MECHILD          10      /* No child processes */
#define MEAGAIN          11      /* Try again */
#define MENOMEM          12      /* Out of memory */
#define MEACCES          13      /* Permission denied */
#define MEFAULT          14      /* Bad address */
#define MENOTBLK         15      /* Block device required */
#define MEBUSY           16      /* Device or resource busy */
#define MEEXIST          17      /* File exists */
#define MEXDEV           18      /* Cross-device link */
#define MENODEV          19      /* No such device */
#define MENOTDIR         20      /* Not a directory */
#define MEISDIR          21      /* Is a directory */
#define MEINVAL          22      /* Invalid argument */
#define MENFILE          23      /* File table overflow */
#define MEMFILE          24      /* Too many open files */
#define MENOTTY          25      /* Not a typewriter */
#define METXTBSY         26      /* Text file busy */
#define MEFBIG           27      /* File too large */
#define MENOSPC          28      /* No space left on device */
#define MESPIPE          29      /* Illegal seek */
#define MEROFS           30      /* Read-only file system */
#define MEMLINK          31      /* Too many links */
#define MEPIPE           32      /* Broken pipe */
#define MEDOM            33      /* Math argument out of domain of func */
#define MERANGE          34      /* Math result not representable */
#define MEDEADLK         35      /* Resource deadlock would occur */
#define MENAMETOOLONG    36      /* File name too long */
#define MENOLCK          37      /* No record locks available */
#define MENOSYS          38      /* Function not implemented */
#define MENOTEMPTY       39      /* Directory not empty */
#define MELOOP           40      /* Too many symbolic links encountered */
#define MEWOULDBLOCK     MEAGAIN  /* Operation would block */
#define MENOMSG          42      /* No message of desired type */
#define MEIDRM           43      /* Identifier removed */
#define MECHRNG          44      /* Channel number out of range */
#define MEL2NSYNC        45      /* Level 2 not synchronized */
#define MEL3HLT          46      /* Level 3 halted */
#define MEL3RST          47      /* Level 3 reset */
#define MELNRNG          48      /* Link number out of range */
#define MEUNATCH         49      /* Protocol driver not attached */
#define MENOCSI          50      /* No CSI structure available */
#define MEL2HLT          51      /* Level 2 halted */
#define MEBADE           52      /* Invalid exchange */
#define MEBADR           53      /* Invalid request descriptor */
#define MEXFULL          54      /* MExchange full */
#define MENOANO          55      /* No anode */
#define MEBADRQC         56      /* Invalid request code */
#define MEBADSLT         57      /* Invalid slot */

#define MEDEADLOCK       MEDEADLK

#define MEBFONT          59      /* Bad font file format */
#define MENOSTR          60      /* Device not a stream */
#define MENODATA         61      /* No data available */
#define METIME           62      /* Timer expired */
#define MENOSR           63      /* Out of streams resources */
#define MENONET          64      /* Machine is not on the network */
#define MENOPKG          65      /* Package not installed */
#define MEREMOTE         66      /* Object is remote */
#define MENOLINK         67      /* Link has been severed */
#define MEADV            68      /* Advertise error */
#define MESRMNT          69      /* Srmount error */
#define MECOMM           70      /* Communication error on send */
#define MEPROTO          71      /* Protocol error */
#define MEMULTIHOP       72      /* Multihop attempted */
#define MEDOTDOT         73      /* RFS specific error */
#define MEBADMSG         74      /* Not a data message */
#define MEOVERFLOW       75      /* Value too large for defined data type */
#define MENOTUNIQ        76      /* Name not unique on network */
#define MEBADFD          77      /* File descriptor in bad state */
#define MEREMCHG         78      /* Remote address changed */
#define MELIBACC         79      /* Can not access a needed shared library */
#define MELIBBAD         80      /* Accessing a corrupted shared library */
#define MELIBSCN         81      /* .lib section in a.out corrupted */
#define MELIBMAX         82      /* Attempting to link in too many shared libraries */
#define MELIBEXEC        83      /* Cannot exec a shared library directly */
#define MEILSEQ          84      /* Illegal byte sequence */
#define MERESTART        85      /* Interrupted system call should be restarted */
#define MESTRPIPE        86      /* Streams pipe error */
#define MEUSERS          87      /* Too many users */
#define MENOTSOCK        88      /* Socket operation on non-socket */
#define MEDESTADDRREQ    89      /* Destination address required */
#define MEMSGSIZE        90      /* Message too long */
#define MEPROTOTYPE      91      /* Protocol wrong type for socket */
#define MENOPROTOOPT     92      /* Protocol not available */
#define MEPROTONOSUPPORT 93      /* Protocol not supported */
#define MESOCKTNOSUPPORT 94      /* Socket type not supported */
#define MEOPNOTSUPP      95      /* Operation not supported on transport endpoint */
#define MEPFNOSUPPORT    96      /* Protocol family not supported */
#define MEAFNOSUPPORT    97      /* Address family not supported by protocol */
#define MEADDRINUSE      98      /* Address already in use */
#define MEADDRNOTAVAIL   99      /* Cannot assign requested address */
#define MENETDOWN        100     /* Network is down */
#define MENETUNREACH     101     /* Network is unreachable */
#define MENETRESET       102     /* Network dropped connection because of reset */
#define MECONNABORTED    103     /* Software caused connection abort */
#define MECONNRESET      104     /* Connection reset by peer */
#define MENOBUFS         105     /* No buffer space available */
#define MEISCONN         106     /* Transport endpoint is already connected */
#define MENOTCONN        107     /* Transport endpoint is not connected */
#define MESHUTDOWN       108     /* Cannot send after transport endpoint shutdown */
#define METOOMANYREFS    109     /* Too many references: cannot splice */
#define METIMEDOUT       110     /* Connection timed out */
#define MECONNREFUSED    111     /* Connection refused */
#define MEHOSTDOWN       112     /* Host is down */
#define MEHOSTUNREACH    113     /* No route to host */
#define MEALREADY        114     /* Operation already in progress */
#define MEINPROGRESS     115     /* Operation now in progress */
#define MESTALE          116     /* Stale NFS file handle */
#define MEUCLEAN         117     /* Structure needs cleaning */
#define MENOTNAM         118     /* Not a XENIX named type file */
#define MENAVAIL         119     /* No XENIX semaphores available */
#define MEISNAM          120     /* Is a named type file */
#define MEREMOTEIO       121     /* Remote I/O error */
#define MEDQUOT          122     /* Quota exceeded */

#define MENOMEDIUM       123     /* No medium found */
#define MEMEDIUMTYPE     124     /* Wrong medium type */

#endif /* MERRNO_H */
