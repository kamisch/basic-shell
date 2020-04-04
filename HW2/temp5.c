#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
//Name: Chase Wang
//Honor Code pledge signature: Chase Wang
//Program description: this is a simple shell program
#define NUM_OF_ARG 10
#define MAX_ARG_LEN 11
#define MAX_INPUT_LEN 5
int main (){
        fputs("% ",stdout);
        char input_command[MAX_INPUT_LEN];
        char commands[NUM_OF_ARG][MAX_ARG_LEN];
        int i = 0;
        int end = 0;
        int notTooLong = 1;
        while (1){
                // clear stdin
                fflush(stdin);
                // reinitialize input arr
                memset(input_command,'\0',sizeof(input_command));
                // reinitialize args in commands
                memset(commands,'\0',sizeof(commands));
                //get input commands
                //if input too big print error msg
                if (fgets(input_command, MAX_INPUT_LEN, stdin) == NULL) {
                    if (feof(stdin)) {
                        return 0;
                    }
                }
                else if (strchr(input_command, '\n') == NULL) {
                    notTooLong = 0;
                    fputs("Input over max input length.\n",stderr);
                }else{
                    // if (fgets(input_command,MAX_INPUT_LEN,stdin) == NULL) {
                    //     fputs("Input greater than max input length",stderr);
                    // };
                    int arg_head = 0;
                    int arg_tail = 0;
                    int arg_count = 0;
                    char temp = input_command[arg_head];
                    //edge case beginning 0s
                    while(temp == ' ' || temp == '\t'){
                            arg_head++;
                            temp = input_command[arg_head];
                    }
                    
                    // //set flag to check for program termination
                    // if(temp == EOF || temp == '\0'){
                    //         end=1;
                    //         break;
                    // }
                    int next_non_wtspace = 0;
                    while(temp != '\0'){
                            if(temp == EOF){
                                    end = 1;
                                    break;
                                    exit(0);
                            }
                            if(temp != ' ' && temp != '\t' && temp != '\n'){
                                    if (next_non_wtspace != 0){
                                            arg_count++;
                                            arg_tail = 0;
                                            next_non_wtspace = 0;
                                    }
                                    commands[arg_count][arg_tail] = temp;
                                    arg_tail += 1;
                            }
                            // loop through white spaces and tabs
                            else if (temp == ' ' || temp == '\t' || temp == '\n'){
                                    next_non_wtspace = 1;
                                    commands[arg_count][arg_tail] = '\0';
                            }
                            arg_head++;
                            temp = input_command[arg_head];
                            if(temp == '\0'){
                                    commands[arg_count][arg_tail++] = temp;
                            }
                    }
                
                    if(end){
                            break;
                            exit(0);
                    }

                    // for(int i = 0; i < arg_count+2; i++){
                    //         fputs(commands[i],stdout);
                    //         fputs("\n",stdout);
                    // }

                    int childPID = fork();
                    if (childPID == 0){
                            // convert input to one d arr for exec
                            char *pt_commands[NUM_OF_ARG];
                            for (int command = 0; command < NUM_OF_ARG; command++){
                                    if (commands[command] != NULL && commands[command][0] != '\n' && commands[command][0] != '\0'){
                                            pt_commands[command] = commands[command];
                                    }else{
                                            pt_commands[command] = '\0';
                                    }                       }
                            // exec and check for error
                            if (execvp(pt_commands[0],pt_commands) == -1){
                                    fputs("Error occured, caused by execvp.\n",stderr);
                                    exit(1);
                            }
                    }
                    else{
                            wait(NULL);
                            fputs("% ",stdout);
                    }
                }
        }

        return 0;

}

