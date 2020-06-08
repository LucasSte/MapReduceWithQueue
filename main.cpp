#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"
#include "MapSync.h"

#define BUFFER_SIZE 10000
#define SHUFFLE_WORKERS 2
#define MAP_WORKERS 2


/*
 * Atualmente, a parte sincrona nao implementa shuffle, entao ela eh bem mais rapida.
 * Para comparar somente a parte implementada, comentar o shuffle e a funcao processInput Async deve ser:
 *
 * void Map::processInput(std::pair<std::string, int> *ptr) {
    delete ptr;
}
 *
 */

int main() {
    Semaphore empty(BUFFER_SIZE);
    Semaphore full(0);

    std::shared_ptr<Shuffle> shuffle( new Shuffle(BUFFER_SIZE, SHUFFLE_WORKERS, full, empty, true));
    //The number of workers should not be greater than the number of files
    Map map("../Files", MAP_WORKERS, full, empty);

    map.setShuffler(shuffle);
    map.listFilesFromPath();

    std::chrono::steady_clock::time_point beginAsync = std::chrono::steady_clock::now();
    map.startParallelWorkers();

    shuffle->startWorkers();

    map.waitForWorkers(SHUFFLE_WORKERS);
    shuffle->waitForWorkers();
    std::chrono::steady_clock::time_point endAsync = std::chrono::steady_clock::now();

    std::cout << "Synchronous section" << std::endl;

    MapSync mapSync("../Files", MAP_WORKERS, full, empty);
    mapSync.listFilesFromPath();

    std::chrono::steady_clock::time_point beginSync = std::chrono::steady_clock::now();
    mapSync.processSync();
    std::chrono::steady_clock::time_point endSync = std::chrono::steady_clock::now();

    std::cout << std::endl;
    std::cout << "Time Async = " << std::chrono::duration_cast<std::chrono::nanoseconds>(endAsync - beginAsync).count() << "[ns]" << std::endl;
//    shuffle->printOutMap();

    std::cout << "Time Sync  = " << std::chrono::duration_cast<std::chrono::nanoseconds>(endSync - beginSync).count() << "[ns]" << std::endl;
//    mapSync.printOutMap();
    return 0;
}
