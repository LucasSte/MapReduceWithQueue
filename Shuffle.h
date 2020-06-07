//
// Created by lucas on 6/7/20.
//

#include <queue>
#include <string>
#include <thread>
#include "Semaphore/Semaphore.h"

#ifndef MAPREDUCEWITHQUEUE_SHUFFLE_H
#define MAPREDUCEWITHQUEUE_SHUFFLE_H

class Shuffle
{
public:
    explicit Shuffle(int max, short workersNum, Semaphore * full, Semaphore * empty, bool * finished, Semaphore * finishedSem);
    void addToBuffer(std::pair<std::string, int> * item);
    std::pair<std::string, int>* removeFromBuffer();
    void startWorkers();
    void waitForWorkers();
private:
    std::queue<std::pair<std::string, int>*> buffer;
    int size;
    int maxsize;
    short workersNum;
    const bool * finished;
    void shuffleWorker();
    Semaphore * fullSemaphore;
    Semaphore * emptySemaphore;
    Semaphore * finishedSemaphore;
    std::vector<std::thread> workers;
    std::mutex criticalRegion;
};

#endif //MAPREDUCEWITHQUEUE_SHUFFLE_H
