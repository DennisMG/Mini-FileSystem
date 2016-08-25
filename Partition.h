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
#include <vector>

using namespace std;

class Partition {


public:
    Partition();

    void createPartition(string name, string size_byte, string unit);
    void deletePartition(string name);
    void listPartitions();
    void formatPartition();
    void mountPartition(string partition_name);
    void unMountPartition(string partition_name);


    void partitionManager();

private:
    string Name, Path;
    int Size_Byte;
    bool Mounted;
    fstream partition;


    void runCommand(string command);

    int getCommandID(string command);

    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);



};


#endif //PROYECTO_SO2_PARTITION_H
