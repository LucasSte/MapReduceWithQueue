//
// Created by lucas on 6/7/20.
//

#include <iostream>
#include "MapSync.h"

MapSync::MapSync(std::string path, short workers, Semaphore &full, Semaphore &empty)
: Map(path, workers, full, empty){}

void MapSync::processSync() {
    long range = files.size();
    readWorker(0, range-1);
}

void MapSync::processInput(std::pair<std::string, int> *ptr) {
    //TODO: Aqui o Ocimar deve colocar uma funcao para processar coisas sincronas
    //TODO: Nao eh necessario shuffle, ja que so existe uma thread
    //std::cout << "sync " << ptr->first << std::endl;
    std::pair<std::string, int> * elem;
//    emptySemaphore.wait();
//    shufflerPtr->addToBuffer(ptr);
//    fullSemaphore.notify();
//
//    fullSemaphore.wait();
//    elem = shufflerPtr->removeFromBuffer();
//    emptySemaphore.notify();

    outMap[ptr->first]++;
    delete ptr;
}

void MapSync::printOutMap(){
    for(auto & it : outMap){
        std::cout<<it.first<<":\t"<<it.second<<std::endl;
    }
}