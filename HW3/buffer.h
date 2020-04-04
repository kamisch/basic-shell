/*
buffer data structure template
*/
#ifndef _BUFFER_H_
#define _BUFFER_H_
#include <assert.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    // curr char
    char *buffer;
    int size;
    // read new char
    int next_r;
    // write new char
    int next_w;
    // sem for checking the buffer size
    sem_t full;
    sem_t empty;
} Buffer;

void init(int size, Buffer *buffer);
void deposit(Buffer *buffer,char c);
char remoove(Buffer *buffer);
void destroy(Buffer *buffer);

#endif