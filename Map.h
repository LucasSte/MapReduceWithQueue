//
// Created by lucas on 6/6/20.
//
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

#ifndef MAPREDUCEWITHQUEUE_MAP_H
#define MAPREDUCEWITHQUEUE_MAP_H

using namespace std;

class Map
{
private:
    string pathName;
    short workers;
    vector<string> files;
    void readWorker(long startRange, long endRange);

public:
    Map(string path, short workers);
    void listFilesFromPath();
    void startParallelWorkers();
};

#endif //MAPREDUCEWITHQUEUE_MAP_H
