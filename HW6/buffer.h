//buffer data structure from HW3 minor modification with array initilization
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 81

typedef struct{
    // curr char
    char buffer[BUFFER_SIZE];
    int size;
    // read new char
    int next_r;
    // write new char
    int next_w;
    // sem for checking the buffer size
    sem_t full;
    sem_t empty;
} Buffer;

void init(Buffer *buffer);
void deposit(Buffer *buffer,char c);
char remoove(Buffer *buffer);
void destroy(Buffer *buffer);

#endif