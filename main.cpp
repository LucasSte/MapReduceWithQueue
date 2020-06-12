#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"
#include "MapSync.h"
#include "MapNoShuffle.h"

#define BUFFER_SIZE 10000000
#define SHUFFLE_WORKERS 1
#define MAP_WORKERS 10


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


    MapSync mapSync("../Files", MAP_WORKERS, full, empty);
    mapSync.listFilesFromPath();
    mapSync.setShuffler(shuffle);

    std::chrono::steady_clock::time_point beginSync = std::chrono::steady_clock::now();
    mapSync.processSync();
    std::chrono::steady_clock::time_point endSync = std::chrono::steady_clock::now();

    MapNoShuffle mapNoShuffle("../Files", MAP_WORKERS, full, empty);
    mapNoShuffle.listFilesFromPath();
    mapNoShuffle.setShuffler(shuffle);

    std::chrono::steady_clock::time_point beginNoS = std::chrono::steady_clock::now();
    mapNoShuffle.startParallelWorkers();
    mapNoShuffle.waitForWorkers(0);
    std::chrono::steady_clock::time_point endNoS = std::chrono::steady_clock::now();


    std::cout << std::endl;
    std::cout << "\n\nTime Async = " << std::chrono::duration_cast<std::chrono::milliseconds>(endAsync - beginAsync).count() << "[ms]" << std::endl;
    shuffle->printOutMap();

    std::cout << "\n\nTime Sync  = " << std::chrono::duration_cast<std::chrono::milliseconds>(endSync - beginSync).count() << "[ms]" << std::endl;
    mapSync.printOutMap();

    std::cout << "\n\nTime NoS  = " << std::chrono::duration_cast<std::chrono::milliseconds>(endNoS - beginNoS).count() << "[ms]" << std::endl;
    mapNoShuffle.printOutMap();

    return 0;
}
