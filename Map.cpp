//
// Created by lucas on 6/6/20.
//
#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace boost::filesystem;

Map::Map(std::string path, short workers, std::shared_ptr<Semaphore> full, std::shared_ptr<Semaphore> empty) {
    pathName = path;
    this->workersNumber = workers;
    fullSemaphore = std::move(full);
    emptySemaphore = std::move(empty);
}

void Map::listFilesFromPath() {
    path p(pathName);

    for(auto i  = directory_iterator(p); i != directory_iterator(); i++)
    {
        if(!is_directory(i->path()))
        {
            files.push_back(i->path().filename().string());
        }
    }
}

void Map::startParallelWorkers()
{
    long numberOfFiles = files.size();
    long filesPerWorker = numberOfFiles / workersNumber;
    long lastWorkerSurplus = filesPerWorker % workersNumber;

    long startRange = 0;
    long endRange = filesPerWorker-1;
    void (Map::*func)(long, long);
    func = &Map::readWorker;
    for(int i=1; i< workersNumber; i++)
    {

        std::thread th(func, this, startRange, endRange);
        workers.push_back(std::move(th));
        startRange += filesPerWorker;
        endRange += filesPerWorker;
    }

    endRange += lastWorkerSurplus;
    std::thread th(func, this, startRange, endRange);
    workers.push_back(std::move(th));

}

void Map::setShuffler(std::shared_ptr<Shuffle> ptr) {
    shufflerPtr = std::move(ptr);
}

void Map::readWorker(long startRange, long endRange)
{
    std::pair<std::string, int> * pointer;
    std::ifstream file;
    std::string line;
    std::string word;

    unsigned long size;
    for(long i= startRange; i<=endRange; i++)
    {
        file.open(pathName + "/" + files[i]);
        while(getline(file, line))
        {
            std::istringstream ss(line);
            do
            {
                word = "";
                ss >> word;
                std::transform(word.begin(), word.end(), word.begin(), [] (unsigned char c) { return std::tolower(c);});
                size = word.size();
                if(size > 0)
                {
                    if(word[size-1] >= 'z' || word[size-1] <= 'a')
                    {
                        word = word.substr(0, size-1);
                    }

                    pointer = new std::pair<std::string, int>();
                    pointer->second = 1;
                    pointer->first = word;
                    emptySemaphore->wait();
                    //criticalRegion.lock();
                    shufflerPtr->addToBuffer(pointer);
                    //std::cout << pointer->first << std::endl;
                    //criticalRegion.unlock();
                    fullSemaphore->notify();

                }

            }while (ss);
        }
        file.close();
    }
}

void Map::waitForWorkers(bool & finished) {
    for(short i=0; i<workersNumber; i++)
    {
        workers[i].join();
    }

    finished = true;
}