//
// Created by lucas on 6/6/20.
//
#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>


using namespace boost::filesystem;

Map::Map(string path, short workers) {
    pathName = path;
    this->workers = workers;
}

void Map::listFilesFromPath() {
    path p(pathName);

    for(auto i  = directory_iterator(p); i != directory_iterator(); i++)
    {
        if(!is_directory(i->path()))
        {
            files.push_back(i->path().filename().string());
        }
    }
}

void Map::startParallelWorkers()
{
    long numberOfFiles = files.size();
    long filesPerWorker = numberOfFiles / workers;
    long lastWorkerSurplus = filesPerWorker % workers;

    readWorker(0, numberOfFiles-1);
}


void Map::readWorker(long startRange, long endRange)
{
    pair<string, int> * pointer;
    std::ifstream file;
    string line;
    string word;

    unsigned long size;
    for(long i= startRange; i<=endRange; i++)
    {
        cout << i << endl;
        file.open(pathName + "/" + files[i]);
        while(getline(file, line))
        {
            istringstream ss(line);
            do
            {
                word = "";
                ss >> word;
                std::transform(word.begin(), word.end(), word.begin(), [] (unsigned char c) { return std::tolower(c);});
                size = word.size();
                if(size > 0)
                {
                    if(word[size-1] >= 'z' || word[size-1] <= 'a')
                    {
                        word = word.substr(0, size-1);
                    }

                    pointer = new pair<string, int>();
                    pointer->second = 1;
                    pointer->first = word;

                    std::cout << pointer->first << endl;
                    delete pointer;

                }

            }while (ss);
        }
        file.close();
    }
}