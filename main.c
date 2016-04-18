//Andrew Light
//Lab 2 - shell
//Last Modified: 4/18/2016
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

int parseInput(char* input, char *argv[]);  //takes single line of unchanged input in char* input, then outputs on argv, returns 0 if foreground, 1 if background, other values with errors
int runCommand(int isBackground, char *argv[]); //runs the command contained at argv[0], runs in background if isBackground=1, in foreground if isBackground=0 (& would be stripped from the last arg)
//returns -1 if "quit" is entered (maybe just do this in parseInput?)

void cleanup(); //runs at termination of program, should reap all children, perform any other necessary cleanup tasks that won't be done in main

int main()
{
    int numArgs=40; //80 column display, so max of 40 columns per row if each is one character long w/ one space inbetween
    int i, isBackground, runStatus;
    char *input;
    char *usrArgs[numArgs];
    input = malloc(sizeof(char)*80);    //allocates string for
    for(i = 0; i < numArgs; i++){   //allocates strings for arguments
        usrArgs[i]=malloc(sizeof(char)*80);
    }
    int exitcheck = 1;
    while(exitcheck != 0){
        //Main program loop, will loop indefinitely until user calls quit
        printf(">");
        fgets(input,80,stdin);  //obtains 1 line of input
        isBackground=parseInput(input,usrArgs); //input is parsed, with the return status placed in isBackground
        if(isBackground == 0 || isBackground == 1){ //if not error state
            runStatus=(runCommand(isBackground,usrArgs));   //run the command and put run status into runStatus
            if(runStatus == -1){    //if exit, cleanup and end main loop
                cleanup();
                exitcheck=0;
            }
        }
        else if(isBackground < 0){
            //if error state when parsing input, handle it (probably not necessary? could just loop back around)
        }
    }
    free(input);
    for(i=0; i<numArgs;i++){
        free(usrArgs[i]);
    }
    return 0;
}

int parseInput(char* input, char *argv[]){
    printf("output would be parsed here, but for now, the input is: %s\n", input);
    return 1;
}

int runCommand(int isBackground, char *argv[]){
    printf("command \"%s\" would be run here, with background state of %d\n", argv[0],isBackground);
    printf("since output hasn't been parsed, there's only gibberish in argv[0]\n");
    return -1;
}

void cleanup(){
    printf("everything would be cleaned up in here, with child processes reaped and such\n");
    return;
}
