//
// Created by Dennis Molina on 8/17/16.
//

#ifndef PROYECTO_SO2_PROGRAM_H
#define PROYECTO_SO2_PROGRAM_H

#include <string>
#include <vector>
#include "Partition.h"

using namespace std;

enum CLI{
    LIST,
    MOUNT,
    DEL,
    CREATE,
    INVALID,
    EXIT
};

class Program {
public:
    Program();
    int Run();
private:
    Partition *partition;
    bool terminate = false;
    string command="";
    CLI getCommand(string command);
    void split(const string &s, char delim, vector<string> &elems);
    vector<string> split(const string &s, char delim);

    void executeCommand(string basic_string);
};


#endif //PROYECTO_SO2_PROGRAM_H
