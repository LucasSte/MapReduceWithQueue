//
// Created by lucas on 6/6/20.
//

#include <mutex>
#include <condition_variable>

#ifndef MAPREDUCEWITHQUEUE_SEMAPHORE_H
#define MAPREDUCEWITHQUEUE_SEMAPHORE_H

class Semaphore
{
private:
    std::mutex mutex;
    std::condition_variable condition;
    unsigned long count = 0;

public:
    void notify();
    void wait();
    bool tryWait();
};

#endif //MAPREDUCEWITHQUEUE_SEMAPHORE_H
