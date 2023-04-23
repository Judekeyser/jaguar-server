#include <iostream>
#include <unistd.h>


int main()
{
    std::cout << "Jaguar Process Manager started" << std::endl;

    pid_t child_pid = fork();
    if(child_pid == -1) {
        perror("Unable to fork child process");
        exit(EXIT_FAILURE);
    } else if (child_pid > 0) {
        std::cout << "Parent just forked child process with PID " << child_pid << std::endl;
    } else {
        std::cout << "Child process is alive :-)" << std::endl;
    }

    return 0;
}