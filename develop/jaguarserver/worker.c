#include "worker.h"
#include <stdio.h>
#include <unistd.h>


void worker_run(void)
{
    printf("Worker runs! Yipiie. Now taking a nap\n");
    sleep(3);
    printf("  Nap done!\n");
}
