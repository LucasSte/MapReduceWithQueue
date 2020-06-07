#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"

int main() {
    //Semaphore full(0);
    //Semaphore empty(2);
    std::shared_ptr<Semaphore> emptyPtr(new Semaphore(2));
    std::shared_ptr<Semaphore> fullPtr(new Semaphore(0));
    std::shared_ptr<Shuffle> shuffle( new Shuffle(2, 2, fullPtr, emptyPtr));
    bool finished = false;
    Map map("../Files", 2, fullPtr, emptyPtr);

    map.setShuffler(shuffle);
    map.listFilesFromPath();
    map.startParallelWorkers();
    shuffle->startWorkers(50);

    map.waitForWorkers(finished);
    shuffle->waitForWorkers();

    return 0;
}