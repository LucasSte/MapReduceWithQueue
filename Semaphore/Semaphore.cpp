//
// Created by lucas on 6/6/20.
//

#include "Semaphore.h"

void Semaphore::notify() {
    std::lock_guard<decltype(mutex)> lock(mutex);
    ++count;
    condition.notify_one();
}

void Semaphore::wait() {
    std::unique_lock<decltype(mutex)> lock(mutex);
    while(!count)
        condition.wait(lock);
    --count;
}

bool Semaphore::tryWait() {
    std::lock_guard<decltype(mutex)> lock(mutex);
    if(count)
    {
        --count;
        return true;
    }

    return false;
}
