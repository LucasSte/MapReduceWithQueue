//
// Created by lucas on 6/7/20.
//

#include "Map.h"

#ifndef MAPREDUCEWITHQUEUE_MAPSYNC_H
#define MAPREDUCEWITHQUEUE_MAPSYNC_H



class MapSync : public Map {
private:
    void processInput(std::pair<std::string, int> * ptr) final;
    std::map<std::string, int> outMap;

public:
    MapSync(std::string path, short workers, Semaphore & full, Semaphore & empty);
    void processSync();
    void printOutMap();

};

#endif //MAPREDUCEWITHQUEUE_MAPSYNC_H
