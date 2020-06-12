//
// Created by lucas on 6/7/20.
//

#include <queue>
#include <map>
#include <string>
#include <thread>
#include "Semaphore/Semaphore.h"

#ifndef MAPREDUCEWITHQUEUE_REDUCE_H
#define MAPREDUCEWITHQUEUE_REDUCE_H

class Reduce
{
public:
    explicit Reduce(int max, short workersNum, Semaphore & full, Semaphore & empty, bool reduce=false);
    void addToBuffer(std::pair<std::string, int> * item);
    std::pair<std::string, int>* removeFromBuffer();
    void startWorkers();
    void waitForWorkers();
    void printOutMap();
private:
    std::queue<std::pair<std::string, int>*> buffer;
    std::map<std::string, int> outMap;
    int size;
    int maxsize;
    short workersNum;
    bool reduce;
    void reduceWorker();
    Semaphore & fullSemaphore;
    Semaphore & emptySemaphore;
    std::vector<std::thread> workers;
    std::mutex criticalRegion;
    std::mutex reduceMutex;
    constexpr inline void addToQueue(std::pair<std::string, int> * item);
    constexpr inline std::pair<std::string, int> * removeFromQueue();
};

#endif //MAPREDUCEWITHQUEUE_REDUCE_H
