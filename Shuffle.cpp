//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Shuffle.h"

Shuffle::Shuffle(int max, short workersNum, Semaphore & full, Semaphore & empty, bool reduce)
: fullSemaphore(full), emptySemaphore(empty)
{
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
    this->reduce = reduce;
}

void Shuffle::addToBuffer(std::pair<std::string, int> *item) {

    criticalRegion.lock();
    addToQueue(item);
    criticalRegion.unlock();

}

constexpr inline void Shuffle::addToQueue(std::pair<std::string, int> *item) {
    if(size <= maxsize)
    {
        buffer.push(item);
        size++;
    }
}

std::pair<std::string, int> * Shuffle::removeFromBuffer() {
    std::lock_guard<std::mutex> lock(criticalRegion);
    return removeFromQueue();
}

constexpr inline std::pair<std::string, int> * Shuffle::removeFromQueue() {
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
            if(reduce)
            {
                shuffleReduceMutex.lock();
                outMap[elem->first]++;
                shuffleReduceMutex.unlock();
            }
            else
            {
                // ToDo: Implementar Shuffle, sem Reduce
            }
        }
    }

}

void Shuffle::waitForWorkers() {

    for(short i=0; i<workersNum; i++)
    {
        workers[i].join();
    }
}

void Shuffle::printOutMap(){
    printf("\n\nQuantidade de palavras nos arquivos:\n");
    for(auto & it : outMap){
        printf("%11s: %d\n", it.first.c_str(), it.second);
    }
}

