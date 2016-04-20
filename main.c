//Andrew Light
//Lab 2 - shell
//Last Modified: 4/18/2016
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

//struct used for the informaation found in each command call
struct Command {
    char command_name[80];
    char args[40][40];
    int arg_count;
};
typedef struct Command Command;

int parseInput(Command *input, char *init_input); //takes a line of input, parses, and returns status (1 if ok, -1 if error)
int runCommand(int isBackground, Command *input); //interprets and runs the command, and if necessary forks a new process
void cleanup(); //reap and kill all children if not already dead


int main(int argc, char *argv[])
{

    int parseStatus, runStatus;
    char *init_input;
    init_input = malloc(sizeof(char) * 80);
    Command input;

    int exitcheck = 1;
    while(exitcheck != 0) {
        //Main program loop,
        printf("> ");
        fgets(init_input, 80, stdin);  //obtains 1 line of input
        parseStatus = parseInput(&input, init_input); //input is parsed, with the return status placed in isBackground

        if(parseStatus == 1){ //if not error state
            runStatus = runCommand(parseStatus, &input); //run the command and put run status into runStatus

            if(runStatus == -1){ //if exit, cleanup and end main loop
                cleanup();
                exitcheck = 0;
            }
        }
        else if(parseStatus < 0){
            printf("Error grabbing input. Retry.\n");
        }
    }
    free(init_input);
    //FREE THE COMMAND STRUCT

    return 0;
}

int parseInput(Command *input, char *init_input){
    //takes line of input and fills in the Command struct with the command_name and args[]
    char *split_input;
    int count = 0;
    split_input = strtok(init_input, " \n\t\r");
    if(split_input != NULL) {
        strcpy(input->command_name, split_input);
        split_input = strtok(NULL, " \n\t\r");
        printf("~~~COMMAND: %s ~~~", input->command_name);
    } else {
        return -1;
    }
    printf("ARGS: ");

    while(split_input != NULL) {
        //printf("inpt: %s\n", split_input);
        strcpy(input->args[count], split_input);
        split_input = strtok(NULL, " \n\t\r");
        printf("%s ", input->args[count]);
        count++;
        input->arg_count = count;
    }
    printf("\n");

    return 1;
}

int runCommand(int isBackground, Command *input) {
    if( strcasecmp(input->command_name, "quit") == 0) {
        //printf("Exiting...\n");
        return -1;
    }
    //
    //else if( strcasecmp(..) == 0) { }

    return 0;
}

void cleanup(){
    //printf("everything would be cleaned up in here, with child processes reaped and such\n");
    return;
}
