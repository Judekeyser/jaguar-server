#include <stdio.h>
/* Standard library */
#include <stdlib.h>
/* Used for `fork` method */
#include <unistd.h>
/* Used for `waitpid` method */
#include <sys/wait.h>
/* Used for signal definitions */
#include <signal.h>
/* Always useful to have assertions and booleans */
#include <assert.h>
#include <stdbool.h>

#include "worker.h"


static void handle_main_program_exit(int signal) {
    printf("Main program (%d) exit, Caramba (%d)\n", getpid(), signal);
    exit(0);
}


int main()
{
    printf("** Jaguar Process Manager started **\n");
    signal(SIGTERM, handle_main_program_exit);
    signal(SIGINT, handle_main_program_exit);

    bool isChild = false;
    {
        const int N = 3;
        int worker_count = 0;
        int failures = 0;
        for(pid_t worker_pid;;) {
            if(worker_count + failures < N) {
                worker_pid = fork();
                if(worker_pid == -1) {
                    perror("Cannot spawn worker");
                    failures += 1;
                    if(failures == N) {
                        exit(EXIT_FAILURE);
                    }
                } else {
                    if(worker_pid == 0) {
                        isChild = true;
                        break;
                    } else {
                        printf("Parent process spawns child (%d)\n", worker_pid);
                        worker_count += 1;
                    }
                }
            } else {
                int stop_status;
                worker_pid = waitpid(-1, &stop_status, WUNTRACED | WNOHANG);

                if(worker_pid > 0) {
                    printf("Parent sees worker PID=%d stop.\n", worker_pid);
                    // Reset failures, so we're going to try to recover workers that failed to start
                    failures = 0;
                    worker_count -= 1;

                    if(WIFSIGNALED(stop_status)) {
                        // Child stop due to a signal
                        printf("  Apparently someone killed our child: %d\n", WTERMSIG(stop_status));
                    } else if(WIFSTOPPED(stop_status)) {
                        // Child was stopped by delivering a signal
                        printf("  Child abended with signal %d\n", WSTOPSIG(stop_status));
                    }
                }
            }
        }
    }

    if(isChild) {
        // TODO: Use poll and select here
        worker_run();
    }

    return 0;
}