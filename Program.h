//
// Created by Dennis Molina on 8/17/16.
//

#ifndef PROYECTO_SO2_PROGRAM_H
#define PROYECTO_SO2_PROGRAM_H

#include <string>
using namespace std;

enum CLI{
    LIST,
    MOUNT,
    DEL,
    CREATE,
    INVALID
};

class Program {
public:
    Program();
    int Run();
private:
    bool terminate = false;
    string command="";
    CLI parseCommand(string command);

    void executeCommand(string basic_string);
};


#endif //PROYECTO_SO2_PROGRAM_H
