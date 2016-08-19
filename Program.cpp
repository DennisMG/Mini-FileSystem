//
// Created by Dennis Molina on 8/17/16.
//
#include <iostream>
#include "Program.h"
using namespace std;

Program::Program() {


}

int Program::Run() {
    cout<<".------------Welcome to My File System-----------."<<endl;
    cout<<"|    What do you want to do?                     |  "<<endl;
    cout<<"|1. List Partitions:     list                    |"<<endl;
    cout<<"|2. Mount Partition:     mount <partition_name>  |"<<endl;
    cout<<"|3. Delete Partition:    del <partition_name>    |"<<endl;
    cout<<"|4. Create Partition:    create <partition_name> |"<<endl;
    cout<<".________________________________________________."<<endl<<endl;

    while(!terminate){
        printf(">>");
        getline(cin, command);
        executeCommand(command);

    }

    return 0;
}

CLI Program::parseCommand(string command) {
    //cout<<command<<endl;
    printf("%s\n",command.c_str());
    return INVALID;
}

void Program::executeCommand(string command) {
    switch (parseCommand(command)){
        case LIST:
            break;
        case MOUNT:
            break;
        case DEL:
            break;
        case CREATE:
            break;
        default:
            printf("Sorry, command not recognized\n");

    }
}
