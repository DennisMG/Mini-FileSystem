//
// Created by Dennis Molina on 8/20/16.
//

#include "Partition.h"
#

void Partition::listPartitions() {

}

void Partition::createPartition(string name, string size_byte, string unit) {
    fstream myFile;
    int size;
    if(unit.compare("mb") == 0 || unit.compare("MB") == 0 )
        size = stoi(size_byte) * 1024;
    else if(unit.compare("GB") == 0 || unit.compare("gb") == 0 )
        size = stoi(size_byte) * 1024 * 1024;
    else{
        printf("ERROR: invalid unit");
        return;
    }
    string command = "dd if=/dev/zero of=./../" + name + ".par bs=1024 count=" + to_string(size);
    system(command.c_str());
}

void Partition::deletePartition(string name) {

}
