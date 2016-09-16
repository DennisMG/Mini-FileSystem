//
// Created by Dennis Molina on 8/17/16.
//

#include "FreeBlockBitMap.h"

FreeBlockBitMap::FreeBlockBitMap() {

}

void FreeBlockBitMap::initializeBitmap(byte *bitmap, int size_in_bytes) {
    Bitmap = bitmap;
    this->size_in_bytes = size_in_bytes;
    printf("bool: %d\n",bitmapGet(&Bitmap[0], 0));
    printf("bool: %d\n",bitmapGet(&Bitmap[0], 1));
    printf("bool: %d\n",bitmapGet(&Bitmap[0], 2));
    printf("bool: %d\n",bitmapGet(&Bitmap[0], 3));
}
