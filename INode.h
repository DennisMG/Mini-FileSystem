//
// Created by Dennis Molina on 8/28/16.
//

#ifndef PROYECTO_SO2_INODE_H
#define PROYECTO_SO2_INODE_H

/*  STATUS CODES
 * Deleted         -1
 * Exist            1
 * End of FAT       0
 * New file         2
 * */

class INode {
public:
    short int status = -1;
    char file_name[50];
    int first_block=0;
    int last_block=0;
    int size=0;
    long creation_date;
};


#endif //PROYECTO_SO2_INODE_H
