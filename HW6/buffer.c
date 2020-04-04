//buffer data structure from HW3
#include "buffer.h"

void init(Buffer *buffer){
    buffer->size = BUFFER_SIZE;
    buffer->next_r = 0;
    buffer->next_w = 0;
    // initialize sem
    assert(sem_init(&(buffer->full), 1, 0) == 0);
    assert(sem_init(&(buffer->empty), 1, BUFFER_SIZE) == 0);
    // allocate memory array
    // buffer->buffer = (char*) malloc(size * sizeof(char));

}

char remoove(Buffer *buffer){
    char c;
    int counter;
    int decrements;
    //locks
    assert(sem_wait(&(buffer->full)) == 0);
    sem_getvalue(&(buffer->full), &counter);
    assert(counter < buffer->size);
    // takes the buffer out
    c = buffer->buffer[buffer->next_w];
    // find next pos for next buffer w
    buffer->next_w = (buffer->next_w + 1) % buffer->size;

    sem_getvalue(&(buffer->empty), &decrements);
    assert(decrements >= 0);
    //unlocks
    assert(sem_post(&(buffer->empty)) == 0);
    return c;
}

void deposit(Buffer *buffer, char c){
    int counter;
    int decrements;
    // locks
    assert(sem_wait(&(buffer->empty))==0);
    sem_getvalue(&(buffer->empty), &decrements);
    assert(decrements >= 0);
    // assign the input char to buffer
    buffer->buffer[buffer->next_r] = c;
    // find the next read position
    buffer->next_r = (buffer->next_r + 1) % buffer->size;
    
    sem_getvalue(&(buffer->full), &counter);
    assert(counter < buffer->size);
    //unlocks
    assert(sem_post(&(buffer->full))==0);
}

void destroy(Buffer *buffer){
    // destroying the buffer memory
    sem_destroy(&(buffer->full));
    sem_destroy(&(buffer->empty));
    free(buffer->buffer);
}