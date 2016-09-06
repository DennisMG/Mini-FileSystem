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

#define MAX_NAME_SIZE_BYTES 50
#define BLOCK_SIZE 4096
using namespace std;



class Partition {


public:
    Partition();
    void createPartition(string name, string size_byte, string unit);
    void deletePartition(string name);
    void listPartitions();
    void formatPartition(string name);
    void mountPartition(string partition_name);
    void unMountPartition(string partition_name);
    void formatPartition(string partition_name, int size_in_bytes);

    void partitionManager();

private:
    const int FAT_LOCATION = 4096;
    const int MAX_FAT_ENTRIES = 128;
    const int ENTRY_LENGTH = 32;
    int getBlockPosition(int block);
    string Name, Path;
    int partition_size_bytes;
    bool Mounted;
    fstream partition;


    void runCommand(string command);

    int getCommandID(string command);
    void writeMasterBlock(string partitionName, int _FAT_pointer, int bitmap_pointer, int data_pointer,
                          int total_of_blocks, long creation_date);
    void readMasterBlock();
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);


    void writeFAT();

    void createFile(string file_name);

    void listFiles(string partition_name);
};


#endif //PROYECTO_SO2_PARTITION_H
