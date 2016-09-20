//
// Created by Dennis Molina on 8/17/16.
//

#include "FreeBlockBitMap.h"

FreeBlockBitMap::FreeBlockBitMap() {

}

void FreeBlockBitMap::initializeBitmap(byte *bitmap, int size_in_bytes) {
    Bitmap = bitmap;
    this->size_in_bytes = size_in_bytes;
    printf("bbool: %d\n",bitmapGet(Bitmap, 0));
    printf("bbool: %d\n",bitmapGet(Bitmap, 1));
    printf("bbool: %d\n",bitmapGet(Bitmap, 2));
    printf("bbool: %d\n",bitmapGet(Bitmap, 3));
}

int FreeBlockBitMap::getAvailableBlocks(int blocksNeeded) {
    //int blocks[blocksNeeded] = {0};
    int byte = bitmapSearch(Bitmap,0,size_in_bytes,0);
    printf("available in pos: %d,\n",byte);
    bitmapSet(Bitmap,byte);
    byte = bitmapSearch(Bitmap,0,size_in_bytes,0);
    printf("available in pos: %d,\n",byte);
//    for(int i = 0 ; i<size_in_bytes ; i++){
//        if()
//    }
}
