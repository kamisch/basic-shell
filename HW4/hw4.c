/*
Name: Chase Wang
Honor Code Pledge signature: Chase Wang
Program description: this is a simple consumer and producer system that reads 80 char input_arr and return a formatted input_arr
*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SIZE 80
void input_thread(int pipeFirst[]){
    char c = getchar();
    while(c){
        write(pipeFirst[1],&c,sizeof(char));
        c = getchar();
    }
    write(pipeFirst[1], &c, sizeof(char));
    exit(0);
}

void case_space_thread(int pipeFirst[],int pipeSecond[]){
    char c = 0;
    //while no error
    while(c != -1){
        // on error read return -1
        read(pipeFirst[0], &c, sizeof(char));
        if (c == '\n'){
            c = ' ';
        }
        write(pipeSecond[1], &c, sizeof(char));
    }
    write(pipeSecond[1], &c, sizeof(char));    
    exit(0);
}

void case_asterisks_thread(int pipeSecond[],int pipeThird[]){
    char c = 0, nextC = 0, carr_sign = '^';
    //while no error
    while(c != -1 && nextC != -1){
        read(pipeSecond[0],&c,sizeof(char));
        if (c == '*'){
            // check if next char is also *
            read(pipeSecond[0],&nextC,sizeof(char));
            if (nextC == '*'){
                // the output should have ** replaced
                write(pipeThird[1], &carr_sign, sizeof(char));
            }
            else {
                // else the output should be the same
                write(pipeThird[1], &c, sizeof(char));
                write(pipeThird[1], &nextC, sizeof(char));
            }
        }
        else {
            write(pipeThird[1], &c, sizeof(char));
        }
    }
}

void output_thread(int pipeThird[]){
    char input_arr[MAX_SIZE];
    int index = 0;
    while(true){
        if (index == MAX_SIZE){
            puts(input_arr);
            index = 0;
        }
        char c = 0;
        read(pipeThird[0],&c,sizeof(char));
        if (c == -1){
            exit(0);
        }
        input_arr[index] = c;
        index++;
    }
}

int main(){
    
    //initialize array for pipe
    int pipeFirst[2];
    int pipeSecond[2];
    int pipeThird[2];
    // creating pipes 
    pipe(pipeFirst);
    pipe(pipeSecond);
    pipe(pipeThird);

    //creating the message passing
    int process = fork();
    if (!process){
        case_space_thread(pipeFirst, pipeSecond);
    }
    process = fork();
    if (!process){
        case_asterisks_thread(pipeSecond, pipeThird);
    }
    process = fork();
    if (!process){
        input_thread(pipeFirst); 
    }
    output_thread(pipeThird);
    exit(0); 
    
}
