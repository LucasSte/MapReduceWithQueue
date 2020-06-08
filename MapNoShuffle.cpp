//
// Created by lucas on 6/8/20.
//

#include "MapNoShuffle.h"

MapNoShuffle::MapNoShuffle(std::string path, short workers, Semaphore &full, Semaphore &empty)
        : Map(path, workers, full, empty)
{}

void MapNoShuffle::processInput(std::pair<std::string, int> *ptr) {
    mapMutex.lock();
    outMap[ptr->first]++;
    mapMutex.unlock();
}
