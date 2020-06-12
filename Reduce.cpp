//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Reduce.h"

Reduce::Reduce(int max, short workersNum, Semaphore & full, Semaphore & empty, bool reduce)
: fullSemaphore(full), emptySemaphore(empty)
{
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
    this->reduce = reduce;
}

void Reduce::addToBuffer(std::pair<std::string, int> *item) {

    criticalRegion.lock();
    addToQueue(item);
    criticalRegion.unlock();

}

constexpr inline void Reduce::addToQueue(std::pair<std::string, int> *item) {
    if(size <= maxsize)
    {
        buffer.push(item);
        size++;
    }
}

std::pair<std::string, int> * Reduce::removeFromBuffer() {
    std::lock_guard<std::mutex> lock(criticalRegion);
    return removeFromQueue();
}

constexpr inline std::pair<std::string, int> * Reduce::removeFromQueue() {
    if(size == 0)
    {
        return nullptr;
    }
    else
    {
        size --;
        std::pair<std::string, int> * elem = buffer.front();
        buffer.pop();
        return elem;
    }
}


void Reduce::startWorkers() {

    void (Reduce::*func)();
    func = &Reduce::reduceWorker;
    for(int i=0; i<workersNum; i++)
    {
        std::thread th(func, this);
        workers.push_back(std::move(th));
    }

}

void Reduce::reduceWorker() {

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
            reduceMutex.lock();
            outMap[elem->first]++;
            reduceMutex.unlock();
        }
    }

}

void Reduce::waitForWorkers() {

    for(short i=0; i<workersNum; i++)
    {
        workers[i].join();
    }
}

void Reduce::printOutMap(){
    printf("\n\nQuantidade de palavras nos arquivos:\n");
    for(auto & it : outMap){
        printf("%11s: %d\n", it.first.c_str(), it.second);
    }
}

