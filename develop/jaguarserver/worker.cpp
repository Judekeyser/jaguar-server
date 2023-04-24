#include "worker.hpp"
#include <iostream>
#include <unistd.h>


void Worker::run()
{
    std::cout << "Worker runs! Yipiie. Now taking a nap" << std::endl;
    sleep(3);
    std::cout << "  Nap done!" << std::endl;
}

void Worker::handle_death()
{
    std::cout << "Worker death initialised." << std::endl;
}