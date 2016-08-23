//
// Created by Dennis Molina on 8/20/16.
//

#ifndef PROYECTO_SO2_PARTITION_H
#define PROYECTO_SO2_PARTITION_H
#include <fstream>
#include <iterator>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
class Partition {
private:
    string Name, Path;
    int Size_Byte;
    bool Mounted;
    fstream partition;
public:


    void createPartition(string name, string size_byte, string unit);
    void deletePartition(string name);
    void listPartitions();
    void formatPartition();
    void mountPartition(string partition_name);
    void unMountPartition(string partition_name);


    void partitionManager();
};


#endif //PROYECTO_SO2_PARTITION_H
