//
// Created by lucas on 6/8/20.
//

#ifndef MAPREDUCEWITHQUEUE_MAPNOBUFFER_H
#define MAPREDUCEWITHQUEUE_MAPNOBUFFER_H

#include "Map.h"

class MapNoBuffer : public Map
{
private:
    void processInput(std::pair<std::string, int> * ptr) final;
    std::mutex mapMutex;

public:
    MapNoBuffer(std::string path, short workers, Semaphore & full, Semaphore & empty);
    void printOutMap();
};

#endif //MAPREDUCEWITHQUEUE_MAPNOBUFFER_H
