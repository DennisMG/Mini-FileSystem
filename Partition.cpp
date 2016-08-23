//
// Created by Dennis Molina on 8/20/16.
//

#include "Partition.h"


void Partition::listPartitions() {

}

void Partition::createPartition(string name, string size_byte, string unit) {
    fstream myFile;
    Name = name;
    Path = "./../" + name + ".par";
    if(unit.compare("mb") == 0 || unit.compare("MB") == 0 )
        Size_Byte = stoi(size_byte) * 1024;
    else if(unit.compare("GB") == 0 || unit.compare("gb") == 0 )
        Size_Byte = stoi(size_byte) * 1024 * 1024;
    else{
        printf("ERROR: invalid unit");
        return;
    }
    string command = "dd if=/dev/zero of=" + Path + " bs=1024 count=" + to_string(Size_Byte);
    system(command.c_str());
    printf("Partition created successfully");


}

void Partition::deletePartition(string name) {

}

void Partition::formatPartition() {

}

void Partition::mountPartition(string partition_name) {
    printf("Mounting...\n");

    partition.open("./../" + partition_name + ".par", ios::out | ios::in | ios::binary);
    if (partition.is_open()){
        Name = partition_name;
        printf("Partition mounted...\n");
        Mounted = true;
        partitionManager();
    }else{
        printf("ERROR: Unable to mount partition.\n");
    }
}

void Partition::unMountPartition(string partition_name) {
    if (partition.is_open()){

        printf("Unmounting %s...\n",Name.c_str());
        Name = "";
        Mounted = false;
        partition.close();
    }else{
        printf("ERROR: Unable to unmount partition.\n");
    }
}

void Partition::partitionManager() {
    string command;
    while(Mounted){
        printf("%s >> ", Name.c_str());
        getline(cin, command);

    }
}
