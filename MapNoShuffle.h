//
// Created by lucas on 6/8/20.
//

#ifndef MAPREDUCEWITHQUEUE_MAPNOSHUFFLE_H
#define MAPREDUCEWITHQUEUE_MAPNOSHUFFLE_H

#include "Map.h"

class MapNoShuffle : public Map
{
private:
    void processInput(std::pair<std::string, int> * ptr) final;
    std::mutex mapMutex;

public:
    MapNoShuffle(std::string path, short workers, Semaphore & full, Semaphore & empty);
    void printOutMap();
};

#endif //MAPREDUCEWITHQUEUE_MAPNOSHUFFLE_H
