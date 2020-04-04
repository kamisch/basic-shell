/*
Name: Chase Wang
Honor Code Pledge signature: Chase Wang
Program description: this is a simple consumer and producer system that reads 80 char input_arr and return a formatted input_arr
*/
#include "buffer.h"
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_SIZE 80
Buffer input_buff, middle_buff, output_buff;

void *input_thread(void *arg){
    while(true){
        deposit(&input_buff, getchar());
    }
}

void *case_space_thread(void *arg){
    char c;
    while(true){
        c = remoove(&input_buff);
        if (c == '\n'){
            c = ' ';
        }
        deposit(&middle_buff, c);
    }
}

void *case_asterisks_thread(void *arg){
    char c, nextC;
    while(true){
        c = remoove(&middle_buff);
        if (c == '*'){
            nextC = remoove(&middle_buff);
            if (nextC == '*'){
                deposit(&output_buff, '^');
            }
            else {
                deposit(&output_buff, c);
                deposit(&output_buff, nextC);
            }
        }
        else {
            deposit(&output_buff, c);
        }
    }
}

void *output_thread(void *arg){
    char input_arr[MAX_SIZE];
    int index = 0;
    while(true){
        if (index == MAX_SIZE){
            puts(input_arr);
            index = 0;
        }
        char c = remoove(&output_buff);
        if (c == -1){
            pthread_exit(NULL);
        }
        input_arr[index] = c;
        index++;
    }
}

int main(){
    pthread_t   t_input, t_space, t_asterisk, t_output;

    init(MAX_SIZE, &input_buff);
    init(MAX_SIZE, &middle_buff);
    init(MAX_SIZE, &output_buff);

    pthread_create(&t_input,NULL, &input_thread,NULL);
    pthread_create(&t_space,NULL, &case_space_thread,NULL);
    pthread_create(&t_asterisk,NULL, &case_asterisks_thread,NULL);
    pthread_create(&t_output,NULL, &output_thread,NULL);

    pthread_join(t_output, NULL);

    destroy(&input_buff);
    destroy(&middle_buff);
    destroy(&output_buff);
}
