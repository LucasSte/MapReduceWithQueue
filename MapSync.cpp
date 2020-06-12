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
    outMap[ptr->first]++;
    delete ptr;
}

void MapSync::printOutMap(){
    printf("\n\nQuantidade de palavras nos arquivos:\n");
    for(auto & it : outMap){
        printf("%11s: %d\n", it.first.c_str(), it.second);
    }
}