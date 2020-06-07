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
    std::vector<std::string> files;
    void readWorker(long startRange, long endRange);
    Semaphore & fullSemaphore;
    Semaphore & emptySemaphore;
    std::vector<std::thread> workers;
    std::shared_ptr<Shuffle> shufflerPtr;

public:
    Map(std::string path, short workers, Semaphore & full, Semaphore & empty);
    void listFilesFromPath();
    void startParallelWorkers();
    void waitForWorkers(short shuffleWorkers);
    void setShuffler(std::shared_ptr<Shuffle> ptr);
};

#endif //MAPREDUCEWITHQUEUE_MAP_H
