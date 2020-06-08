//
// Created by lucas on 6/6/20.
//
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <thread>
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"

#ifndef MAPREDUCEWITHQUEUE_MAP_H
#define MAPREDUCEWITHQUEUE_MAP_H


class Map
{
private:
    std::string pathName;
    short workersNumber;
    std::vector<std::thread> workers;

protected:
    void readWorker(long startRange, long endRange);
    virtual void processInput(std::pair<std::string, int> * ptr);
    std::vector<std::string> files;
    std::shared_ptr<Shuffle> shufflerPtr;
    Semaphore & fullSemaphore;
    Semaphore & emptySemaphore;

public:
    Map(std::string path, short workers, Semaphore & full, Semaphore & empty);
    void listFilesFromPath();
    void startParallelWorkers();
    void waitForWorkers(short shuffleWorkers);
    void setShuffler(std::shared_ptr<Shuffle> ptr);
};

#endif //MAPREDUCEWITHQUEUE_MAP_H
