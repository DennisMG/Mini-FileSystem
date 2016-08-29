//
// Created by Dennis Molina on 8/20/16.
//

#include <sstream>
#include "Partition.h"
#include "INode.h"


Partition::Partition() {

}

void Partition::listPartitions() {

}

void Partition::createPartition(string name, string size_byte, string unit) {
    Name = name;
    Path = name + ".par";

    if(unit.compare("mb") == 0 || unit.compare("MB") == 0 )
        partition_size_bytes = stoi(size_byte) * 1024;
    else if(unit.compare("GB") == 0 || unit.compare("gb") == 0 )
        partition_size_bytes = stoi(size_byte) * 1024 * 1024;
    else{
        printf("ERROR: invalid unit");
        return;
    }
    string command = "dd if=/dev/zero of=" + Path + " bs=1024 count=" + to_string(partition_size_bytes);
    system(command.c_str());
    printf("Partition created successfully\n");

    formatPartition(name,stoi(size_byte));


}

void Partition::deletePartition(string name) {

}

void Partition::formatPartition(string partition_name, int size_in_bytes){

    int _FAT_pointer= 74;
    int bitmap_pointer = 1;
    int data_pointer = 1;
    int total_of_blockes = 1;

    partition_name.resize(50,' ');
    time_t t = time(0);

    writeMasterBlock(partition_name,_FAT_pointer,bitmap_pointer,data_pointer,total_of_blockes,t);
    writeFAT();
    partition.close();
    printf("Successfully formated.\n");


}

void Partition::formatPartition(string name) {
    printf("Formatting partition...\n");



}

void Partition::mountPartition(string partition_name) {
    printf("Mounting...\n");

    partition.open(partition_name + ".par", ios::out | ios::in | ios::binary);
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
    //if (partition.is_open()){

        printf("Unmounting %s...\n",Name.c_str());
        Name = "";
        Mounted = false;
        partition.close();
//    }else{
//        printf("ERROR: Unable to unmount partition.\n");
//    }
}

void Partition::partitionManager() {
    string command;
    while(Mounted){
        printf("%s >> ", Name.c_str());
        getline(cin, command);
        runCommand(command);

    }
}

void Partition::runCommand(string command) {
    int command_code = getCommandID(command);
    vector<string> tokens = split(command, ' ');
    switch (command_code){
        case 1:
            mountPartition(tokens[1]);
            printf("Finished mounting\n");
            break;
        case 2:
            unMountPartition(Name);
            printf("Finished unmounting.\n");
            break;
        case 3:
            formatPartition(Name,0);
            printf("Finished unmounting.\n");
            break;
        case 4:
            createFile(tokens[1]);
            printf("File created!!\n");
            break;
        default:
            printf("Command not recognized.\n");
    }
}

int Partition::getCommandID(string command) {
    vector<string> tokens = split(command, ' ');
    int command_id;

    if(tokens[0].compare("mount") == 0){
        command_id = 1;
    }else if(tokens[0].compare("unmount") == 0){
        command_id = 2;
    }else if(tokens[0].compare("format") == 0){
        command_id = 3;
    }else if(tokens[0].compare("touch") == 0){
        command_id = 4;
    }else{
        command_id = -1;
    }

    return command_id;

}

vector<string> Partition::split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void Partition::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (!item.empty())
            elems.push_back(item);
    }
}

void Partition::writeMasterBlock(string partitionName, int _FAT_pointer, int bitmap_pointer, int data_pointer,
                                 int total_of_blocks, long creation_date) {
    string _partition_name = partitionName + ".par";
    partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        partition.seekg (0, ios::beg);
        partition.write( _partition_name.c_str(),50);
        partition.write(reinterpret_cast<const char *>(&creation_date), sizeof(creation_date));
        partition.write(reinterpret_cast<const char *>(&_FAT_pointer), sizeof(_FAT_pointer));
        partition.write(reinterpret_cast<const char *>(&bitmap_pointer), sizeof(bitmap_pointer));
        partition.write(reinterpret_cast<const char *>(&data_pointer), sizeof(data_pointer));
        partition.write(reinterpret_cast<const char *>(&total_of_blocks), sizeof(total_of_blocks));
        partition.close();
    }else{
        printf("ERROR: Could not write master block\n");
    }



}

void Partition::readMasterBlock() {

}

void Partition::writeFAT() {
    INode newNode;
    newNode.status = -1;
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekp (FAT_LOCATION);
        printf("size of short int: %d\n",sizeof(newNode.status));
        partition.write( reinterpret_cast<const char *>(&newNode.status),sizeof(newNode.status));
        partition.close();
    }else{
        printf("ERROR: Could not write master block\n");
    }

}

void Partition::createFile(string file_name) {
    short int status;
    INode newNode;
    int entry = FAT_LOCATION;
    partition.open(Path, ios::out | ios::in | ios::binary);
    time_t t = time(0);
    if(partition.is_open()){
        int i;
        for(i = 0 ; i<MAX_FAT_ENTRIES ; i++){
            partition.seekg(entry);
            partition.read(reinterpret_cast<char *>(&status), sizeof(status));

            if(status == 1){

                printf("entry position occupied: %d\n",entry);
                entry += ENTRY_LENGTH;
                //Entry is occupied, move to the next.
            }else{
                printf("entry position available, writing file... entry: %d\n",entry);
                newNode.status = 1;

                partition.seekp(entry);
                partition.write( reinterpret_cast<const char *>(&newNode.status), sizeof(newNode.status));
                partition.write( file_name.c_str(),50);
                partition.write( reinterpret_cast<const char *>(&newNode.first_block), sizeof(newNode.first_block));
                partition.write( reinterpret_cast<const char *>(&newNode.last_block), sizeof(newNode.last_block));
                partition.write( reinterpret_cast<const char *>(&t), sizeof(t));
                partition.write( reinterpret_cast<const char *>(&newNode.size), sizeof(newNode.size));
                break;
            }

        }

        printf("Number of files: %d\n",i);



        partition.close();


    }else{
        printf("ERROR: Could not file master block\n");
    }
}




