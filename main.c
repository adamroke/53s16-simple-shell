//Andrew Light
//Adam Roke (48266987)
//Lab 2 - shell
//Last Modified: 4/28/2016
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

//struct used for the informaation found in each command call
struct Command {
    char command_name[80];
    char *args[40];
    int arg_count;	//args must end with NULL, and you can't NULL an array
};
typedef struct Command Command;

int parseInput(Command *input, char *init_input); //takes a line of input, parses, and returns status (1 if ok, -1 if error)
int runCommand(int isBackground, Command *input); //interprets and runs the command, and if necessary forks a new process
void cleanup(); //reap and kill all children if not already dead

int numThreads = 0;
pid_t pidlist[80]; //assuming a max of 80 processes for now, probably change this

int main(int argc, char *argv[])
{

    int parseStatus, runStatus;
    Command input;
    int i;

    for(i = 0; i < 40; i++){
        input.args[i] = malloc(sizeof(char)*80); //allocate space for the arguments
    }

    int exitcheck = 1;
    while(exitcheck != 0) {
    	char *init_input;
    	init_input = malloc(sizeof(char)*80);

        printf(">");
        fgets(init_input, 80, stdin);  //obtains 1 line of input
        parseStatus = parseInput(&input, init_input); //input is parsed, with the return status placed in isBackground
        if(parseStatus == 1 || parseStatus == 0) { //if not error state
            runStatus = runCommand(parseStatus, &input); //run the command and put run status into runStatus

            if(runStatus == -1) { //if exit, cleanup and end main loop
                cleanup();
                exitcheck = 0;
            }
        }
        else if(parseStatus < 0) {
            printf("Error grabbing input. Retry.\n");
        }

    	free(init_input); //free memory used to grab line of output

    	for(i = 0; i < 40; i++) { //realloc memory for the next command about to be read
    		if(input.args[i] == NULL){
    			input.args[i] = malloc(sizeof(char)*80);
    		}
    	}
    }

    //FREE THE COMMAND OBJECT
    for(i = 0; i < 40; i++){
         if(input.args[i] != NULL) {
             free(input.args[i]);
         }
    }

    return 0;
}


int parseInput(Command *input, char *init_input){
    //takes line of input and fills in the Command struct with the command_name and args[]
    char *split_input;
    int count = 0;
    int background;
    split_input = strtok(init_input, " \n\t\r");
    if(split_input != NULL) {
        strcpy(input->command_name, split_input);
        strcpy(input->args[count],input->command_name);
        count++;
        split_input = strtok(NULL, " \n\t\r");
    } else {
        return -1; //no command given. exit
    }

    while(split_input != NULL) {
        //printf("inpt: %s\n", split_input);
        strcpy(input->args[count], split_input);
        split_input = strtok(NULL, " \n\t\r");
        //printf("|%s| ", input->args[count]);
        count++;
        input->arg_count = count;
    }
    free(input->args[count]);
	input->args[count] = NULL; //so you need to have the last argument as NULL otherwise it won't run

    background = strcspn(input->args[count-1],"&");
    if(input->args[count-1][background] == '&'){

    	if(background == 0){ //handles case where the & character is a separate token
    		free(input->args[count-1]); //take it out of the argument list
    		input->args[count-1] = NULL;
    	}

        else { //take out the & character from the arg being passed to execv
            memmove(input->args[count-1]+background, input->args[count-1]+strcspn(input->args[count-1],"\0"),1);
        }
        return 1;
    }

    return 0;
}

int runCommand(int isBackground, Command *input) {
    pid_t pid;
    int pidstatus;
    extern int numThreads;
    extern pid_t pidlist[];
    if( strcasecmp(input->command_name, "quit") == 0) {
        //printf("Exiting...\n");
        return -1;
    }
    else {
        if(isBackground == 1) {
            //run in background
            printf("change this message text!\n");
            printf("~~background entered, with command |%s|\n",input->args[0]);
            pid = fork();
    		if(pid == 0) {
    			execve(input->args[0],input->args);
    			printf("%s\n",strerror(errno));    //prints the error that went wrong with execve
    			exit(0);
    		}
    		else {
    			pidlist[numThreads]=pid;           //parent process just adds pid to array of pids and returns to command prompt
    			numThreads++;
    			//system("ps");                    //uncomment to display list of processes
    		}
        } else {
            //run then wait (foreground)
            pid = fork();
            if(pid == 0) {
            	execve(input->args[0],input->args);
            	printf("%s\n",strerror(errno));    //this should only ever run if there's some problem with running the program the user input
                exit(0);
            }
            else {
            	//printf("pid=%d\n",pid);
            	waitpid(pid,&pidstatus,0);
            	//printf("main thread here!\n");
            }

        }
    }


    return 0;
}

void cleanup() {
    extern int numThreads;
    extern pid_t pidlist[];
    int i,childstatus;

    for(i=0; i<numThreads;i++){  //kills any background processes still running
    	kill(pidlist[i],SIGINT);
    }

    for(i=0;i<numThreads;i++){  //loops through all killed background processes, reaping them
    	pid_t wpid = wait(&childstatus);        //note that
    	if(WIFEXITED(childstatus)) {
            //printf("child %d terminated %d\n", wpid, WEXITSTATUS(childstatus));
            continue;
        }
    }

    return;
}
