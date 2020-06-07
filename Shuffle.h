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
    explicit Shuffle(int max, short workersNum, std::shared_ptr<Semaphore> full, std::shared_ptr<Semaphore> empty);
    void addToBuffer(std::pair<std::string, int> * item);
    std::pair<std::string, int>* removeFromBuffer();
    void startWorkers(int totalAttempts);
    void waitForWorkers();
private:
    std::queue<std::pair<std::string, int>*> buffer;
    int size;
    int maxsize;
    short workersNum;
    void shuffleWorker(int trials);
    std::shared_ptr<Semaphore> fullSemaphore;
    std::shared_ptr<Semaphore> emptySemaphore;
    std::vector<std::thread> workers;
    std::mutex criticalRegion;
};

#endif //MAPREDUCEWITHQUEUE_SHUFFLE_H
