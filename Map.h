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
    void readWorker(long startRange, long endRange, Shuffle & shuffler);
    std::shared_ptr<Semaphore> fullSemaphore;
    std::shared_ptr<Semaphore> emptySemaphore;
    //std::mutex criticalRegion;
    std::vector<std::thread> workers;

public:
    Map(std::string path, short workers, const Semaphore & full, const Semaphore & empty);
    void listFilesFromPath();
    void startParallelWorkers(Shuffle & shuffler);
    void waitForWorkers();
};

#endif //MAPREDUCEWITHQUEUE_MAP_H
