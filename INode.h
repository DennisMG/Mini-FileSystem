//
// Created by Dennis Molina on 8/28/16.
//

#ifndef PROYECTO_SO2_INODE_H
#define PROYECTO_SO2_INODE_H

/*  STATUS CODES
 * Deleted          0
 * Exist            1
 * */



class INode {
public:
    bool status = false;
    char file_name[15];
    int first_block=0;
    int size=0;
    long creation_date;
};


#endif //PROYECTO_SO2_INODE_H
