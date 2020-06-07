//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "Shuffle.h"

Shuffle::Shuffle(int max, short workersNum, Semaphore * full, Semaphore * empty, bool * finished, Semaphore * finishedSem)
: finished(finished), fullSemaphore(full), emptySemaphore(empty), finishedSemaphore(finishedSem)
{
    maxsize = max;
    size = 0;
    this->workersNum = workersNum;
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
   // for(int i=0; i<trials; i++)
    //{
       // if(!*finished)
        //{
        while(!*finished || size > 0)
        {
            fullSemaphore->wait();
            elem = removeFromBuffer();
            std::cout << "Removed ";
            emptySemaphore->notify();
            if(elem != nullptr)
            {
                std::cout << elem->first << std::endl;
                delete elem;
            }
            else
            {
                std::cout << "Nada" << std::endl;
                fullSemaphore->notify();
            }
        }

//        }
//        else if(fullSemaphore->tryWait() && size > 0)
//        {
//            elem = removeFromBuffer();
//            std::cout << "Removed" << std::endl;
//            emptySemaphore->notify();
//            if(elem != nullptr)
//            {
//                std::cout << elem->first << std::endl;
//                delete elem;
//            }
//        }


//    }
}

void Shuffle::waitForWorkers() {
    //finishedSemaphore->wait();
    //fullSemaphore->waitForZero();
    std::cout << size << std::endl;
    for(short i =0; i<workersNum; i++)
    {
        std::cout << "Esperando" << std::endl;
        fullSemaphore->notify();
        workers[i].join();
    }
}

