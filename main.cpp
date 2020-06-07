#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"

#define BUFFER_SIZE 2
#define SHUFLLE_WORKERS 2
#define MAP_WORKERS 2

int main() {
    Semaphore empty(2);
    Semaphore full(0);

    std::shared_ptr<Shuffle> shuffle( new Shuffle(BUFFER_SIZE, SHUFLLE_WORKERS, full, empty));
    //The number of files should not be greater than the number of workers
    Map map("../Files", MAP_WORKERS, full, empty);

    map.setShuffler(shuffle);
    map.listFilesFromPath();
    map.startParallelWorkers();
    shuffle->startWorkers();

    map.waitForWorkers(SHUFLLE_WORKERS);

    shuffle->waitForWorkers();


    return 0;
}