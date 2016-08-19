//
// Created by Dennis Molina on 8/17/16.
//
#include <iostream>
#include <sstream>
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
        printf("\n>>");
        getline(cin, command);
        executeCommand(command);

    }

    return 0;
}

CLI Program::parseCommand(string command) {
    vector<string> tokens = split(command, ' ');
    CLI token;
    if(tokens[0] == "list"){
        token = LIST;
    }else if(tokens[0] == "mount"){
        token = MOUNT;
    }else if(tokens[0] == "del"){
        token = DEL;
    }else if(tokens[0] == "create"){
        token = CREATE;
    }else if(tokens[0] == "exit"){
        token = EXIT;
    }else{
        token = INVALID;
    }

    return token;
}

void Program::executeCommand(string command) {
    switch (parseCommand(command)){
        case LIST:
            printf("listing all partitions...");
            break;
        case MOUNT:
            printf("Mounting partition...");
            break;
        case DEL:
            printf("Deleting partition...");
            break;
        case CREATE:
            printf("Creating partition...");
            break;
        case EXIT:
            terminate=true;
            printf("Exiting... Good Bye.\n");
            break;
        default:
            printf("Sorry, command not recognized\n");

    }
}

vector<string> Program::split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void Program::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
