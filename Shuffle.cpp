//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Shuffle.h"

Shuffle::Shuffle(int max, short workersNum, const Semaphore & full, const Semaphore & empty){
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
    fullSemaphore = std::make_shared<Semaphore>(full);
    emptySemaphore = std::make_shared<Semaphore>(empty);
}

void Shuffle::addToBuffer(std::pair<std::string, int> *item) {
    criticalRegion.lock();
    if(size < maxsize)
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

void Shuffle::startWorkers(int totalItems) {
    int itemsPerWorker = totalItems/workersNum;
    int surplus = totalItems % workersNum;

    void (Shuffle::*func)(int);
    func = &Shuffle::shuffleWorker;
    for(int i=1; i<workersNum; i++)
    {
        std::thread th(func, itemsPerWorker);
        workers.push_back(std::move(th));
    }

    std::thread th(func, itemsPerWorker + surplus);
    workers.push_back(std::move(th));


}

void Shuffle::shuffleWorker(int expectedItems) {

    std::pair<std::string, int> * elem;
    for(int i=0; i<expectedItems; i++)
    {
        fullSemaphore->wait();
        elem = removeFromBuffer();
        emptySemaphore->notify();
        std::cout << elem->first << std::endl;
        delete elem;
    }
}