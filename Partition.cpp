//
// Created by Dennis Molina on 8/20/16.
//

#include <sstream>
#include <sys/stat.h>
#include "Partition.h"
#include "INode.h"
#include "SuperBlock.h"
#include "FreeBlockBitMap.h"
#include "Block.h"

Partition::Partition() {
    freeBlocks = new FreeBlockBitMap();
}

void Partition::listPartitions() {

}

void Partition::createPartition(string name, string size_byte, string unit) {
    Name = name;
    Path = name + ".fs";

    if(unit.compare("mb") == 0 || unit.compare("MB") == 0 )
        partition_size_kb = stoi(size_byte) * 1024;
    else if(unit.compare("GB") == 0 || unit.compare("gb") == 0 )
        partition_size_kb = stoi(size_byte) * 1024 * 1024;
    else{
        printf("ERROR: invalid unit");
        return;
    }
    string command = "dd if=/dev/zero of=" + Path + " bs=1024 count=" + to_string(partition_size_kb);
    system(command.c_str());
    printf("Partition created successfully\n");

    formatPartition(name, partition_size_kb);


}

void Partition::deletePartition(string name) {

}

void Partition::formatPartition(string partition_name, int size_in_kb){
    string _partition_name = partition_name + ".fs";
    _partition_name.resize(15,' ');

    SuperBlock superBlock;

    strcpy(superBlock.Name, _partition_name.c_str());
    superBlock.size = size_in_kb*1024;
    superBlock.CreationDate = time(0);
    superBlock.block_number = size_in_kb/4;

    int total_blocks = size_in_kb/4;  /* size in kb/block size in kb (4Kb) */



    writeSuperBlock(superBlock);
    writeEmptyFAT();
    writeNewBitmapV2(total_blocks/8);
    printf("Successfully formated.\n");
}

void Partition::formatPartition(string name) {
    printf("Formatting partition...\n");



}

void Partition::mountPartition(string partition_name) {
    printf("Mounting...\n");

    partition.open(partition_name + ".fs", ios::out | ios::in | ios::binary);
    if (partition.is_open()){
        partition.close();
        Name = partition_name;
        Path = Name + ".fs";
        printf("Partition mounted...\n");
        Mounted = true;
        readSuperBlock();
        readFat();
        readBitmap();
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
            if(createFileV2(tokens[1], 0, 0)){
                printf("File created!!\n");
            }else{
                printf("Error creating file\n");
            }
            break;
        case 5:
            listFilesV2(Path);
            printf("\n");
            break;
        case 6:
            if (tokens.size() != 2)
                printf("ERROR: Invalid number of arguments! expected 1, received %d\n Ej. copy_from_fs <file_name>\n", ((int) tokens.size())-1);
            else
                copy_from_fs(tokens[1],"");
            printf("\n");
            break;
        case 7:
            if (tokens.size() != 3)
                printf("ERROR: Invalid number of arguments! expected 2, received %d\n Ej. copy_to_fs <file_name> <destination_path>\n", ((int) tokens.size())-1);
            else
                if(copy_to_fs(tokens[1], tokens[2])){
                    printf("Copy Successful\n");
                }else{
                    printf("Error Copying file\n");
                }
            printf("\n");
            break;
        case 8:
            if (tokens.size() != 3)
                printf("ERROR: Invalid number of arguments! expected 2, received %d\n Ej. rename <file_name> <new_file_name>\n", ((int) tokens.size())-1);
            else
                rename_file(tokens[1], tokens[2]);
            printf("\n");
            break;
        case 9:
            delete_file(tokens[1]);
            printf("\n");
            break;
        case 10:
            getInfo();
            printf("\n");
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
    }else if(tokens[0].compare("empty") == 0){
        command_id = 4;
    }else if(tokens[0].compare("ls") == 0){
        command_id = 5;
    }else if(tokens[0].compare("copy_from_fs") == 0){
        command_id = 6;
    }else if(tokens[0].compare("copy_to_fs") == 0){
        command_id = 7;
    }else if(tokens[0].compare("rename") == 0){
        command_id = 8;
    }else if(tokens[0].compare("delete") == 0){
        command_id = 9;
    }else if(tokens[0].compare("info") == 0){
        command_id = 10;
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

void Partition::writeSuperBlock(SuperBlock superBlock) {




    partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        partition.seekp (0, ios::beg);
        partition.write(reinterpret_cast<const char *>(&superBlock), BLOCK_SIZE);
        partition.close();
    }else{
        printf("1ERROR: Could not write master block\n");
    }



}


void Partition::readSuperBlock() {
    //if(!partition.good())
        partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        partition.seekg(0, ios::beg);
        partition.read(reinterpret_cast<char *>(&_superBlock), sizeof(SuperBlock));


        partition.close();
        //_superBlock.Name.resize(16,'');
        //printf("name: %s, Size: %d, block number: %d\n", string(_superBlock.Name).c_str(),  _superBlock.size,_superBlock.block_number);
    }else{
        printf("2ERROR: Could not read master block\n");
    }
}

void Partition::writeEmptyFAT() {
    INode newNode;
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekp (getBlockPosition(1));
        partition.write( reinterpret_cast<const char *>(&newNode.status),sizeof(newNode.status));
        partition.close();
    }else{
        printf("ERROR: Could not write FAT block\n");
    }

}



bool Partition::createFileV2(string file_name, int size, int pointer_to_first_block) {
    INode newNode;
    int entry = getBlockPosition(1);
//    newNode.creation_date = time(0);
//    newNode.size = size;
//    newNode.first_block = pointer_to_first_block;
//    strcpy(newNode.file_name,file_name.c_str());

    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekg(entry);
        partition.read(reinterpret_cast<char *>(&FAT), BLOCK_SIZE);

        vector<string> tokens = split(file_name,'/');
        string realName = tokens.at(tokens.size()-1);

        if(fileExist(FAT, realName).status){
            printf("Possible duplicate. There's a file with the same name\n");
            return false;
        }

        for(int i = 0 ; i < sizeof(FAT)/sizeof(INode) ; i++){

            if(FAT[i].status == 0 ){


                FAT[i].status = 1;
                vector<string> tokens = split(file_name,'/');
                string realName = tokens.at(tokens.size()-1);
                if(strlen(realName.c_str()) > 15)
                    realName.resize(14,' ');
                //printf("realName: %s\n",realName.c_str());
                //printf("length: %zu\n",strlen(realName.c_str()));
                strcpy(FAT[i].file_name,realName.c_str());
                FAT[i].creation_date = time(0);
                FAT[i].first_block = pointer_to_first_block;
                FAT[i].size = size;
                break;
            }
        }
        partition.seekp(entry);
        partition.write(reinterpret_cast<const char *>(&FAT), BLOCK_SIZE);
    }

    partition.close();
    return true;

}

void Partition::listFilesV2(string partition_name){
    int entry = getBlockPosition(1);

    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekg(entry);
        partition.read(reinterpret_cast<char *>(&FAT), BLOCK_SIZE);
        for(int i = 0 ; i < sizeof(FAT)/sizeof(INode) ; i++){
            if(FAT[i].status == 1){
                printf("%s\t\t",FAT[i].file_name);
                printf("%d BYTES\t\t\t\t",FAT[i].size);
                tm* localtm = localtime(&FAT[i].creation_date);
                printf("Created: %s", asctime(localtm));

            }
        }
    }

    partition.close();
}

int Partition::getBlockPosition(int block) {
    return block * BLOCK_SIZE;
}

void Partition::readBitmap() {
    int blockOffset = getBlockPosition(2);
    Bitmap = new byte[_superBlock.block_number/8];
    int i;
    for(i = 0 ; i < _superBlock.block_number/8; i++){
        Bitmap[i] = 0;
    }
    Bitmap[i] = '\0';
    partition.open(Path, ios::out | ios::in | ios::binary);
    if( partition.is_open() ){
        partition.seekg(blockOffset);
        partition.read(reinterpret_cast<char *>(Bitmap), _superBlock.block_number/8 );
        partition.close();
        freeBlocks->initializeBitmap(Bitmap, _superBlock.block_number/8);
    }else{
        printf("Error: could not read Bitmap");
    }

}





void Partition::writeNewBitmap(int bitmap_size_bytes) {
    //printf("number of bytes: %d\n", bitmap_size_bytes);

    int blockOffset = getBlockPosition(2);
    Bitmap = new byte[bitmap_size_bytes];
    int i;
    for(i = 0 ; i < bitmap_size_bytes; i++){
        Bitmap[i] = 0;
    }
    Bitmap[i] = '\0';
    bitmapSet( &Bitmap[0], 0);
    bitmapSet( &Bitmap[0], 1);

    if (bitmap_size_bytes <= 4096){
        bitmapSet(&Bitmap[0], 2);

    }else{ //8192
        int word = 0;
        int busy_blocks = bitmap_size_bytes/BLOCK_SIZE;//0
        int bit = 2;
        for(; bit <= 8 ; bit++){//bit = 2
            if(busy_blocks == 0)
                break;

            if(bit == 8 ){
                bit=0;
                word++;
            }else{
                bitmapSet(&Bitmap[word], bit);
                busy_blocks--;
            }

        }

    }
//    printf("Bitmap[0]: %c\n", Bitmap[0]);
//    printf("size of Bitmap: %d\n", sizeof(Bitmap));
//    printf("size of byte: %d\n", sizeof(byte));

    partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        partition.seekp(blockOffset);
        partition.write(reinterpret_cast<char*>(Bitmap),bitmap_size_bytes*sizeof(byte));

    }
    partition.close();

}

void Partition::copy_from_fs(string source, string destination) {
    fstream SourceFile(source,ios::out | ios::in | ios::binary );
    fstream DestinationFile(Path, ios::out | ios::in | ios::binary);

    if(!SourceFile.is_open()){
        DestinationFile.close();
        printf("Source doesnt exist");
        return;
    }

//    if(!DestinationFile.good()){
//        SourceFile.close();
//        printf("Destination file doesnt exist");
//        return;
//    }

    //get file size
    SourceFile.seekg (0, SourceFile.end);
    int fileSize = SourceFile.tellg();
    SourceFile.seekg (0, SourceFile.beg);

    //get blocks needed to save the file
    int blocksNeeded = getBlocksNeeded(fileSize);
    vector<int> emptyBlocks = getEmptyBlocks(blocksNeeded);
    if(emptyBlocks.size()<=0){
        printf("Sorry, can't copy file. unsuficient space.");
        return;
    }

    copyFileData(SourceFile, DestinationFile, fileSize, blocksNeeded, emptyBlocks);
    //printf("pointer to first block: %d",emptyBlocks[0]);
    createFileV2(source,fileSize,emptyBlocks[0]);
    freeBlocks->fillBitmap(emptyBlocks);
    writeBitmap();


}

void Partition::copyFileData(fstream &SourceFile, fstream &DestinationFile, int fileSize, int blocksNeeded,
                             const vector<int> &emptyBlocks) {
    Block block;

    int bufferSize = 4092;
    int bytesRemaining = fileSize;

    for (int i = 0; i < blocksNeeded; ++i) {

        int nextBlock = i+1;
        if(nextBlock == blocksNeeded)
            block.pointer = -1;
        else
            block.pointer = emptyBlocks[nextBlock];

        if(bufferSize > bytesRemaining)
            bufferSize = bytesRemaining;

        SourceFile.read(block.buffer, bufferSize );
        //printf("index: %d\n",emptyBlocks[i]);
        DestinationFile.seekp(getBlockPosition(emptyBlocks[i]));
        DestinationFile.write(reinterpret_cast<const char *>(&block), BLOCK_SIZE );
        bytesRemaining -= bufferSize;
    }
    SourceFile.close();
    DestinationFile.close();
}

void Partition::writeBitmap() {
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        int blockOffset = getBlockPosition(2);
        partition.seekp(blockOffset);
        partition.write(reinterpret_cast<char*>(Bitmap),(_superBlock.block_number/8)*sizeof(byte));
        partition.close();
    }else{
        printf("Error writing bitmap");
    }


}

bool Partition::copy_to_fs(string source, string destination) {
    //fstream SourceFile(source, ios::out | ios::in | ios::binary );
    if(lite_FileExist(destination+'/'+source)){
        printf("A file with the same name already exist\n");
        return false;

    }

    INode file_entry = fileExist(FAT,source);
    if(!file_entry.status){
        printf("Could not find '%s'.\n",source.c_str());
        return false;
    }

    partition.open(Path, ios::out | ios::in | ios::binary);
    fstream DestinationPath(destination+'/'+source, ios::out | ios::binary | ios::app);
    if(partition.is_open()){

        copy_out(DestinationPath,file_entry);


    }else{
        printf("Error copying to fs");
        return false;
    }


    partition.close();
    DestinationPath.close();
    return true;


}
//de 3 - 18
void Partition::copy_out(fstream &DestinationPath, const INode &file_entry) {
    int bytesRemaining = file_entry.size;
    int bufferSize = BLOCK_SIZE- sizeof(int);
    Block buffer;
    int offset = getBlockPosition(file_entry.first_block);
    //printf("2pointer: %d\n",file_entry.first_block);
    //printf("size: %d\n",getBlocksNeeded(file_entry.size));
    for (int i = 0; i < getBlocksNeeded(file_entry.size); ++i) {
            if(bufferSize > bytesRemaining)
                bufferSize = bytesRemaining;

            this->partition.seekg(offset);
            this->partition.read(reinterpret_cast<char *>(&buffer), BLOCK_SIZE);
            //printf("pointer: %d\n",buffer.pointer);
            DestinationPath.write(reinterpret_cast<char *>(buffer.buffer),bufferSize);
            offset = getBlockPosition(buffer.pointer);
            bytesRemaining -= BLOCK_SIZE - sizeof(int);
        }
}

int Partition::getBlocksNeeded(int bytes) {
    int nBlocks = bytes/(BLOCK_SIZE - sizeof(int));//se resta los 4 bytes del apuntador
    if(bytes%(BLOCK_SIZE - sizeof(int)) > 0)
        nBlocks++;
    return nBlocks;
}


vector<int> Partition::getEmptyBlocks(int blocksNeeded) {
    return freeBlocks->getAvailableBlocks(blocksNeeded);

}

void Partition::writeNewBitmapV2(int bitmap_size_bytes) {
    //printf("number of bytes: %d\n", bitmap_size_bytes);


    Bitmap = new byte[bitmap_size_bytes];
    int i;
    for(i = 0 ; i < bitmap_size_bytes; i++){
        Bitmap[i] = 0;
    }
    Bitmap[i] = '\0';
    bitmapSet( Bitmap, 0);
    bitmapSet( Bitmap, 1);
    int number_of_blocks = getBlocksNeeded(bitmap_size_bytes);
    printf("number_of_blocks for bitmap: %d\n", number_of_blocks);
    for(int i = 0 ; i < number_of_blocks; i++){
        bitmapSet(Bitmap, i+2);
    }


    partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        int blockOffset = getBlockPosition(2);
        partition.seekp(blockOffset);
        partition.write(reinterpret_cast<char*>(Bitmap),bitmap_size_bytes*sizeof(byte));
        partition.close();
    }else{
        printf("Error writing new bitmap\n");
    }

}

INode Partition::fileExist(INode *FAT, string fileName) {
    INode file_entry;
    for(int i = 0 ; i < sizeof(this->FAT)/sizeof(INode) ; i++){
        if(FAT[i].status==1 && strcmp(FAT[i].file_name,fileName.c_str()) == 0){
            return FAT[i];
        }
    }
    return file_entry;
}

void Partition::rename_file(string old_name, string new_name) {
    if(fileExist(FAT,old_name).status == 0){
        printf("File doesnt exist");
        return;
    }
    //new_name.resize(14,' ');
    int i;
    for( i = 0 ; i < sizeof(this->FAT)/sizeof(INode) ; i++){
        if(FAT[i].status==1 && strcmp(FAT[i].file_name,old_name.c_str()) == 0){
            strcpy(FAT[i].file_name,new_name.c_str());
            break;
        }
    }

    writeFAT();





}

bool Partition::delete_file_from_directory(INode *FAT, string fileName) {
    for(int i = 0 ; i < sizeof(this->FAT)/sizeof(INode) ; i++){
        if(FAT[i].status==1 && strcmp(FAT[i].file_name,fileName.c_str()) == 0){
             FAT[i].status = 0;
            return true;
        }
    }
    return false;
}

void Partition::delete_file(string file_name) {
    INode inode = fileExist(FAT,file_name);
    if(!delete_file_from_directory(FAT,file_name)){
        printf("File '%s' doesnt  exist\n",file_name.c_str());
    }

    writeFAT();
    vector<int> ocupiedBlocks = getFileBlocks(inode);
    freeBlocks->resetBytes(ocupiedBlocks);
    writeBitmap();

}

void Partition::readFat() {
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        int offset = getBlockPosition(1);
        partition.seekg(offset);
        partition.read(reinterpret_cast<char *>(&FAT), BLOCK_SIZE);
        partition.close();
    }else{
        printf("Error: Could not read File Allocation Table\n");
    }
}

void Partition::writeFAT() {
    int entry = getBlockPosition(1);
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekp(entry);
        partition.write(reinterpret_cast<const char *>(&FAT), BLOCK_SIZE);
        partition.close();
    }else{
        printf("Error writing File Allocation Table\n");
    }


}

vector<int> Partition::getFileBlocks(INode node) {
    vector<int> blocks;
    vector<int> empty;
    blocks.push_back(node.first_block);
    //printf("pointer: %d\n",node.first_block);
    Block buffer;
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        int offset = getBlockPosition(node.first_block);
        for (int i = 0; i < getBlocksNeeded(node.size)-1; ++i) {
            //printf("offset: %d\n",offset);
            this->partition.seekg(offset);
            this->partition.read(reinterpret_cast<char *>(&buffer), BLOCK_SIZE);

            if(buffer.pointer <= 0)
                break;
            blocks.push_back(buffer.pointer);
            //printf("pointer: %d\n",buffer.pointer);

            offset = getBlockPosition(buffer.pointer);
        }
        partition.close();
        return blocks;
    }else{
        printf("Error opening partition to read ocupied blocks");
    }
    return empty;

}

void Partition::getInfo() {
    int space = freeBlocks->getFreeBlocksCount();
    float size_in_bytes = space * BLOCK_SIZE;
    printf("Space available: %fMB\n",((size_in_bytes/1024)/1024 ) );
}

bool Partition::lite_FileExist(string file_path) {
    struct stat buf;
    if (stat(file_path.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}








