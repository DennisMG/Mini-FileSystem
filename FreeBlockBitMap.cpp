//
// Created by Dennis Molina on 8/17/16.
//

#include "FreeBlockBitMap.h"
#include "Partition.h"
#include <vector>

FreeBlockBitMap::FreeBlockBitMap() {

}

void FreeBlockBitMap::initializeBitmap(byte *bitmap, int size_in_bytes) {
    Bitmap = bitmap;
    this->size_in_bytes = size_in_bytes;
//    printf("bbool: %d\n",bitmapGet(Bitmap, 0));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 1));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 2));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 3));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 4));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 5));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 6));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 7));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 8));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 9));
//    printf("bbool: %d\n",bitmapGet(Bitmap, 10));

}

vector<int> FreeBlockBitMap::getAvailableBlocks(int blocksNeeded) {
    vector<int> blocks;
    vector<int> empty;
    int i;
    //printf("size_in_bytes %d \n",size_in_bytes);


    int start = 0;
    for(i = 0 ; i<blocksNeeded ; i++){
        int available = bitmapSearch(Bitmap,0,size_in_bytes,start);
        start = available;
        if(available == BITMAP_NOTFOUND){
            printf("1ERROR: No space available. Blocks needed %d bytes and available space found %d\n",blocksNeeded*4096,blocks.size()*BLOCK_SIZE);
            return empty;
        }

        //bitmapSet(Bitmap,available);
        if(!(available<= 0)) {
            //printf("block: %d\n",available);
            blocks.push_back(available);
        }
    }
    //printf("Space needed %d and space found %d\n",blocksNeeded*4096,blocks.size()*BLOCK_SIZE);
    if(size_in_bytes*8*BLOCK_SIZE<blocksNeeded*BLOCK_SIZE){
        printf("2ERROR: No space available. Space needed %d and space found %d\n",blocksNeeded*BLOCK_SIZE,size_in_bytes*8*BLOCK_SIZE);
        return empty;
    }

    return blocks;

}

void FreeBlockBitMap::fillBitmap(vector<int> blocksToFill) {
    for(int i = 0 ; i<blocksToFill.size() ; i++){
        bitmapSet(Bitmap,blocksToFill[i]);
    }
}

void FreeBlockBitMap::resetBytes(vector<int> blocksToReset) {

    for(int i = 0 ; i<blocksToReset.size() ; i++){
        //printf("block: %d\n",blocksToReset[i]);
        bitmapReset(Bitmap,blocksToReset[i]);
    }
}

int FreeBlockBitMap::getFreeBlocksCount() {
    int count=0;
    int start = 0;

    int available = bitmapSearch(Bitmap,0,size_in_bytes,start);
    while( available != BITMAP_NOTFOUND ){
        count++;
        available = bitmapSearch(Bitmap,0,size_in_bytes,start);
        start = available;
    }

    return count;
}


