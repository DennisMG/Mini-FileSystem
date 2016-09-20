//
// Created by Dennis Molina on 8/20/16.
//

#include <sstream>
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
    _partition_name.resize(16,' ');

    SuperBlock superBlock;

    strcpy(superBlock.Name, _partition_name.c_str());
    superBlock.size = size_in_kb*1024;
    superBlock.CreationDate = time(0);
    superBlock.block_number = size_in_kb/4;

    int total_blocks = size_in_kb/4;  /* size in kb/block size in kb (4Kb) */



    writeSuperBlock(superBlock);
    writeFAT();
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
            createFileV2(tokens[1], 0, 0);
            printf("File created!!\n");
            break;
        case 5:
            listFilesV2(Path);
            printf("\n\n");
            break;
        case 6:
            copy_from_fs(tokens[1], tokens[2]);
            printf("\n\n");
            break;
        case 7:
            copy_to_fs(tokens[1], tokens[2]);
            printf("\n\n");
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
    }else if(tokens[0].compare("ls") == 0){
        command_id = 5;
    }else if(tokens[0].compare("copy_from_fs") == 0){
        command_id = 6;
    }else if(tokens[0].compare("copy_to_fs") == 0){
        command_id = 7;
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
        printf("name: %s, Size: %d, block number: %d\n", string(_superBlock.Name).c_str(),  _superBlock.size,_superBlock.block_number);
    }else{
        printf("2ERROR: Could not read master block\n");
    }
}

void Partition::writeFAT() {
    INode newNode;
    partition.open(Path, ios::out | ios::in | ios::binary);
    if(partition.is_open()){
        partition.seekp (getBlockPosition(1));
        partition.write( reinterpret_cast<const char *>(&newNode.status),sizeof(newNode.status));
        partition.close();
    }else{
        printf("ERROR: Could not write master block\n");
    }

}

void Partition::createFileV2(string file_name, int size, int pointer_to_first_block) {
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
        for(int i = 0 ; i < sizeof(FAT)/sizeof(INode) ; i++){
            if(FAT[i].status == 0){
                FAT[i].status = 1;
                strcpy(FAT[i].file_name,file_name.c_str());
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

void Partition::writeBitmap() {

}



void Partition::writeNewBitmap(int bitmap_size_bytes) {
    printf("number of bytes: %d\n", bitmap_size_bytes);

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
    printf("Bitmap[0]: %c\n", Bitmap[0]);
    printf("size of Bitmap: %d\n", sizeof(Bitmap));
    printf("size of byte: %d\n", sizeof(byte));

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

    if(!SourceFile.good()){
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
            //printf("size of BLOCK: %d\n", sizeof(Block));

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
        DestinationFile.seekp(getBlockPosition(emptyBlocks[nextBlock]));
        DestinationFile.write(reinterpret_cast<const char *>(&block), BLOCK_SIZE );
        bytesRemaining -= bufferSize;
    }


    createFileV2(source,fileSize,emptyBlocks[0]);
    freeBlocks->fillBitmap(emptyBlocks);
    writeBitmap();


}

void Partition::copy_to_fs(string source, string destination) {
    fstream SourceFile(source, ios::out | ios::in | ios::binary );
    fstream DestinationFile(destination, ios::out | ios::in | ios::binary);

    if(!SourceFile.good()){
        DestinationFile.close();
        printf("Source doesnt exist");
        return;
    }

    if(!DestinationFile.good()){
        SourceFile.close();
        printf("Destination file doesnt exist");
        return;
    }






}

int Partition::getBlocksNeeded(int bytes) {
    int nBlocks = bytes/(BLOCK_SIZE - sizeof(int));//se resta los 4 bytes del apuntador
    if(bytes%(BLOCK_SIZE - sizeof(int)) > 0)
        nBlocks++;
    return nBlocks;
}


vector<int> Partition::getEmptyBlocks(int blocksNeeded) {
    //int emptyBlocksArray[blocksNeeded]={0};
    vector<int> b = freeBlocks->getAvailableBlocks(blocksNeeded);

//    for(int i = 0; i< blocksNeeded ; i++){
        printf("vector size: %d\n",b.size());
//    }

//    printf("bbool: %d\n",bitmapGet(Bitmap, 10));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 11));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 12));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 13));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 14));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 15));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 16));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 17));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 18));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 19));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 20));
    return b;

}

void Partition::writeNewBitmapV2(int bitmap_size_bytes) {
    printf("number of bytes: %d\n", bitmap_size_bytes);


    Bitmap = new byte[bitmap_size_bytes];
    int i;
    for(i = 0 ; i < bitmap_size_bytes; i++){
        Bitmap[i] = 0;
    }
    Bitmap[i] = '\0';
    bitmapSet( Bitmap, 0);
    bitmapSet( Bitmap, 1);

    if (bitmap_size_bytes <= 4096){
        bitmapSet(Bitmap, 2);

    }else{
        int number_of_blocks = bitmap_size_bytes/BLOCK_SIZE;

        for(int i = 0 ; i< number_of_blocks; i++){
            bitmapSet(Bitmap, i+2);
        }
    }

    partition.open(Path, ios::out | ios::in | ios::binary);

    if(partition.is_open()){
        int blockOffset = getBlockPosition(2);
        partition.seekp(blockOffset);
        partition.write(reinterpret_cast<char*>(Bitmap),bitmap_size_bytes*sizeof(byte));

    }
    partition.close();
}




