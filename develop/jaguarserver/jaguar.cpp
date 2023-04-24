#include <iostream>
/* Used for `fork` method */
#include <unistd.h>
/* Used for `waitpid` method */
#include <sys/wait.h>
/* Used for `prctl` method */
#include <sys/prctl.h>
/* Used for signal definitions */
#include <signal.h>

#include "worker.hpp"


int main()
{
    std::cout << "Jaguar Process Manager started" << std::endl;

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
                        /* How to link end of program in parent to end of program for child?
                            # noZombie
                        */
                        break;
                    } else {
                        worker_count += 1;
                    }
                }
            } else {
                int stop_status;
                worker_pid = waitpid(-1, &stop_status, WUNTRACED | WNOHANG);

                if(worker_pid > 0) {
                    std::cout << "Parent sees worker PID=" << worker_pid << " stop." << std::endl;
                    // Reset failures, so we're going to try to recover workers that failed to start
                    failures = 0;
                    worker_count -= 1;

                    if(WIFSIGNALED(stop_status)) {
                        // Child stop due to a signal
                        std::cout << "  Apparently someone killed our child: " << WTERMSIG(stop_status) << std::endl;
                    } else if(WIFSTOPPED(stop_status)) {
                        // Child was stopped by delivering a signal
                        std::cout << "  Child abended with signal " << WSTOPSIG(stop_status) << std::endl;
                    }
                }
            }
        }
    }

    if(isChild) {
        Worker worker;
        worker.run();
    }

    return 0;
}