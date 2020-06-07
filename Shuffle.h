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
    explicit Shuffle(int max, short workersNum, Semaphore & full, Semaphore & empty);
    void addToBuffer(std::pair<std::string, int> * item);
    std::pair<std::string, int>* removeFromBuffer();
    void startWorkers();
    void waitForWorkers();
private:
    std::queue<std::pair<std::string, int>*> buffer;
    int size;
    int maxsize;
    short workersNum;
    void shuffleWorker();
    Semaphore & fullSemaphore;
    Semaphore & emptySemaphore;
    std::vector<std::thread> workers;
    std::mutex criticalRegion;
    constexpr inline void addToQueue(std::pair<std::string, int> * item);
    constexpr inline std::pair<std::string, int> * removeFromQueue();
};

#endif //MAPREDUCEWITHQUEUE_SHUFFLE_H
