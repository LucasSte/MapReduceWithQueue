//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Shuffle.h"

Shuffle::Shuffle(int max, short workersNum, std::shared_ptr<Semaphore> full, std::shared_ptr<Semaphore> empty){
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
    fullSemaphore = std::move(full);
    emptySemaphore = std::move(empty);
}

void Shuffle::addToBuffer(std::pair<std::string, int> *item) {
    criticalRegion.lock();
    if(size < maxsize)
    {
        buffer.push(item);
        size++;
       // std::cout << item->first << std::endl;
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

void Shuffle::startWorkers(int totalAttempts) {
    int itemsPerWorker = totalAttempts/workersNum;
    int surplus = totalAttempts % workersNum;

    void (Shuffle::*func)(int);
    func = &Shuffle::shuffleWorker;
    for(int i=1; i<workersNum; i++)
    {
        std::thread th(func, this, itemsPerWorker);
        workers.push_back(std::move(th));
    }

    std::thread th(func, this, itemsPerWorker + surplus);
    workers.push_back(std::move(th));


}

void Shuffle::shuffleWorker(int trials) {

    std::pair<std::string, int> * elem;
    for(int i=0; i<trials; i++)
    {
        //fullSemaphore->wait();
        //bool tried = fullSemaphore->tryWait();
        //std::cout << tried << "waited" << std::endl;
        //if(tried)
        //{
        fullSemaphore->wait();
            elem = removeFromBuffer();
            std::cout << "Removed" << std::endl;
            emptySemaphore->notify();
            std::cout << elem->first << std::endl;
            delete elem;
        //}
    }
}

void Shuffle::waitForWorkers() {
    for(short i =0; i<workersNum; i++)
    {
        workers[i].join();
    }
}

