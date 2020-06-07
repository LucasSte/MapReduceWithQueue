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
            std::cout << elem->first << std::endl;
            //TODO: Aqui o Ocimar deve colocar uma função para trata as coisas do Ocimar
        }
    }

}

void Shuffle::waitForWorkers() {

    for(short i=0; i<workersNum; i++)
    {
        workers[i].join();
    }
}

