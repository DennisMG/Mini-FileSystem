//
// Created by Dennis Molina on 8/17/16.
//
#include <iostream>
#include <sstream>
#include "Program.h"
#include "INode.h"

using namespace std;

Program::Program() {
    partition = new Partition();

}

int Program::Run() {
    printf("iNode size: %d\n",sizeof(INode) );
    cout<<".---------------------Welcome to My File System--------------------."<<endl;
    cout<<"|    What do you want to do?                                       |  "<<endl;
    cout<<"|1. List Partitions:     ls                                        |"<<endl;
    cout<<"|2. Mount Partition:     mount <partition_name>                    |"<<endl;
    cout<<"|3. Unmount Partition:   unmount                                   |"<<endl;
    cout<<"|4. Delete Partition:    del <partition_name>                      |"<<endl;
    cout<<"|5. Create Partition:    create <partition_name> <size_mb> <MB/GB> |"<<endl;
    cout<<"|                                                                  |"<<endl;
    cout<<".__________________________________________________________________."<<endl<<endl;

    while(!terminate){
        printf("\n>>");
        getline(cin, command);
        executeCommand(command);

    }

    return 0;
}

CLI Program::getCommand(string command) {
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
    }else if(tokens[0] == "format"){
        token = FORMAT;
    }else if(tokens[0] == "clear"){
        token = CLEAR;
    }else{
        token = INVALID;
    }

    return token;
}

void Program::executeCommand(string command) {

    vector<string> parameters = split(command,' ');
    switch (getCommand(command)){
        case LIST:
            printf("listing all partitions...\n");
            break;
        case MOUNT:
            printf("Mounting partition...\n");
            partition->mountPartition(parameters[1]);
            break;
        case DEL:
            printf("Deleting partition...\n");
            break;
        case CLEAR:

            break;
        case CREATE: {
            printf("Creating partition...\n");
            //int size = stoi(parameters[2]);
            //printf("%d",size);

            if (parameters.size() > 4)
                printf("ERROR: Invalid number of arguments! expected 3, received %d\n", ((int) parameters.size())-1);
            else
                partition->createPartition(parameters[1], parameters[2], parameters[3]);
            break;
        }
        case EXIT:
            terminate=true;
            printf("Exiting... Good Bye.\n");
            break;
        case FORMAT:
            partition->formatPartition(parameters[1],0);
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
        if (!item.empty())
            elems.push_back(item);
    }
}
