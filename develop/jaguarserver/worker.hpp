#ifndef _JAGUAR_WORKER_
#define _JAGUAR_WORKER_

#include <iostream>


class Worker
{
    private:
        void handle_death();

    public:
        void run();

        ~Worker()
        {
            this -> handle_death();
        }
};

#endif