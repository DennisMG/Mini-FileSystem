//
// Created by Dennis Molina on 9/5/16.
//

#ifndef PROYECTO_SO2_BITMAP_H
#define PROYECTO_SO2_BITMAP_H



#define BIT (8*sizeof(byte))
#define BITMAP_NOTFOUND -1

typedef enum{
    False = 0,
    True = 1
} Bool;

typedef unsigned char byte;

bool bitmapGet   (byte *, int);
void bitmapSet   (byte *, int);
void bitmapReset (byte *, int);
int  bitmapSearch(byte *, bool, int, int);



#endif //PROYECTO_SO2_BITMAP_H
