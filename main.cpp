#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Reduce.h"
#include "MapSync.h"
#include "MapNoBuffer.h"

#define BUFFER_SIZE 10000000
#define REDUCE_WORKERS 1
#define MAP_WORKERS 10


int main() {
    Semaphore empty(BUFFER_SIZE);
    Semaphore full(0);

    std::shared_ptr<Reduce> reduce(new Reduce(BUFFER_SIZE, REDUCE_WORKERS, full, empty, true));
    //The number of workers should not be greater than the number of files
    Map map("../Files", MAP_WORKERS, full, empty);

    map.setReducer(reduce);
    map.listFilesFromPath();

    std::chrono::steady_clock::time_point beginAsync = std::chrono::steady_clock::now();
    map.startParallelWorkers();
    reduce->startWorkers();

    map.waitForWorkers(REDUCE_WORKERS);
    reduce->waitForWorkers();
    std::chrono::steady_clock::time_point endAsync = std::chrono::steady_clock::now();


    MapSync mapSync("../Files", MAP_WORKERS, full, empty);
    mapSync.listFilesFromPath();
    mapSync.setReducer(reduce);

    std::chrono::steady_clock::time_point beginSync = std::chrono::steady_clock::now();
    mapSync.processSync();
    std::chrono::steady_clock::time_point endSync = std::chrono::steady_clock::now();

    MapNoBuffer mapNoBuffer("../Files", MAP_WORKERS, full, empty);
    mapNoBuffer.listFilesFromPath();
    mapNoBuffer.setReducer(reduce);

    std::chrono::steady_clock::time_point beginNoS = std::chrono::steady_clock::now();
    mapNoBuffer.startParallelWorkers();
    mapNoBuffer.waitForWorkers(0);
    std::chrono::steady_clock::time_point endNoS = std::chrono::steady_clock::now();


    std::cout << std::endl;
    std::cout << "\n\nTime Async = " << std::chrono::duration_cast<std::chrono::milliseconds>(endAsync - beginAsync).count() << "[ms]" << std::endl;
//    reduce->printOutMap();

    std::cout << "\n\nTime Sync  = " << std::chrono::duration_cast<std::chrono::milliseconds>(endSync - beginSync).count() << "[ms]" << std::endl;
//    mapSync.printOutMap();

    std::cout << "\n\nTime NoBuffer  = " << std::chrono::duration_cast<std::chrono::milliseconds>(endNoS - beginNoS).count() << "[ms]" << std::endl;
//    mapNoBuffer.printOutMap();

    return 0;
}
