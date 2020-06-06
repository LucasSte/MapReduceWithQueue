#include <iostream>
#include "Map.h"

int main() {

    Map map = Map("../Files", 1);
    map.listFilesFromPath();
    map.startParallelWorkers();
    return 0;
}