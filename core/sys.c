#include "cmsis.h"
#include <sys/types.h>
#include <errno.h>

int  main(void);
void exit(int ErrorCode);

static void *heap_pointer = NULL;

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

void _exit(int status) {
    exit(status);
}

int _getpid(void) {
    return 1;
}

void *_sbrk(unsigned int incr) {
  return NULL;
}

