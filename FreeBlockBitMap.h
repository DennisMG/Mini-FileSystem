//
// Created by Dennis Molina on 8/17/16.
//



#ifndef PROYECTO_SO2_FREEBLOCKBITMAP_H
#define PROYECTO_SO2_FREEBLOCKBITMAP_H

#include <bitset>         // std::bitset
#include <vector>
#include "bitmap.h"

using namespace std;

class FreeBlockBitMap {
public:
    FreeBlockBitMap();
    byte * Bitmap;
    int size_in_bytes;
    void initializeBitmap(byte *bitmap, int size_in_bytes);

    vector<int> getAvailableBlocks(int blocksNeeded);
    void fillBitmap(vector<int> blocksToFill);
};


#endif //PROYECTO_SO2_FREEBLOCKBITMAP_H
