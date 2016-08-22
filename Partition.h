//
// Created by Dennis Molina on 8/20/16.
//

#ifndef PROYECTO_SO2_PARTITION_H
#define PROYECTO_SO2_PARTITION_H
#include <fstream>
#include <string>
using namespace std;
class Partition {
public:
    string Name;

    void createPartition(string name, int size_byte);
    void deletePartition(string name);
    void listPartitions();
};


#endif //PROYECTO_SO2_PARTITION_H
