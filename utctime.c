#include <stdio.h>
#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <time.h>

time_t utctime (time_t *tp) {
    message m;

    // Perform system call.
    if (_syscall(PM_PROC_NR, UTCTIME, &m) < 0) {
        fprintf(stderr, "Error: do_utctime failed!\n");
        return -1;
    }

    //Read utctime from the syscall message
    time_t utctime = (time_t)m.m2_l1;

    // Save utctime in the pointer is it isn't NULL
    if (tp != NULL) {
        *tp = utctime;
    }

    // Return result.
    return utctime;
}