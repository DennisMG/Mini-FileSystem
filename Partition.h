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
#include "INode.h"
#include "bitmap.h"

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
    void formatPartition(string partition_name, int size_in_kb);

    void partitionManager();

private:
    const int FAT_LOCATION = 4096;
    const int MAX_FAT_ENTRIES = 128;
    const int ENTRY_LENGTH = 32;
    int getBlockPosition(int block);
    string Name, Path;
    int partition_size_kb;
    bool Mounted;
    fstream partition;
    INode FAT[128];
    vector<byte> Bitmap;


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

    void createFileV2(string file_name);

    void listFilesV2(string partition_name);

    void readBitmap();

    void writeBitmap();

    void writeNewBitmap(int bitmap_size_bytes);
};


#endif //PROYECTO_SO2_PARTITION_H
