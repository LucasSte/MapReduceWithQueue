#include <iostream>
#include "Map.h"
#include "Semaphore/Semaphore.h"
#include "Shuffle.h"

int main() {
    Semaphore empty(2);
    Semaphore full(0);
    Semaphore finishedSem(0);


    bool finished = false;
    std::shared_ptr<Shuffle> shuffle( new Shuffle(1, 3, &full, &empty, &finished, &finishedSem));
    //The number of files should not be greater than the number of workers
    Map map("../Files", 2, &full, &empty);

    map.setShuffler(shuffle);
    map.listFilesFromPath();
    map.startParallelWorkers();
    shuffle->startWorkers();

    map.waitForWorkers(3);
    std::cout << "Passei" << std::endl;
    shuffle->waitForWorkers();

    std::cout << "Terminei" << std::endl;
    return 0;
}