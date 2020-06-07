//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Shuffle.h"

Shuffle::Shuffle(int max, short workersNum, Semaphore & full, Semaphore & empty)
: fullSemaphore(full), emptySemaphore(empty)
{
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
}

void Shuffle::addToBuffer(std::pair<std::string, int> *item) {

    criticalRegion.lock();
    if(size <= maxsize)
    {
        buffer.push(item);
        size++;
    }
    criticalRegion.unlock();

}

std::pair<std::string, int> * Shuffle::removeFromBuffer() {
    criticalRegion.lock();
    if(size == 0)
    {
        criticalRegion.unlock();
        return nullptr;
    }
    else
    {
        size --;
        std::pair<std::string, int> * elem = buffer.front();
        buffer.pop();
        criticalRegion.unlock();
        return elem;
    }
}


void Shuffle::startWorkers() {

    void (Shuffle::*func)();
    func = &Shuffle::shuffleWorker;
    for(int i=0; i<workersNum; i++)
    {
        std::thread th(func, this);
        workers.push_back(std::move(th));
    }

}

void Shuffle::shuffleWorker() {

    std::pair<std::string, int> * elem;
    int num = 1;
    while(num & 1)
    {
        fullSemaphore.wait();
        elem = removeFromBuffer();
        emptySemaphore.notify();
        num = elem->second;
        if(num & 2)
        {
            delete elem;
            break;
        }
        else
        {
            std::cout << elem->first << std::endl;
        }
    }

}

void Shuffle::waitForWorkers() {

    for(short i=0; i<workersNum; i++)
    {
        workers[i].join();
    }
}

