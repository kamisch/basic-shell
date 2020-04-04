/*
Name: Chase Wang
Honor Code Pledge signature: Chase Wang
Program description: this is a simple consumer and producer system
 that reads 80 char input_arr and return a formatted input_arr. 
 Part of the code is from the sample code provided by HW6 assignment instructions.

Cite:
 https://jameshfisher.com/2017/01/26/mmap/
*/
#include "buffer.h"
#include <assert.h>
// #include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 80
#define ERROR -1

// buffer state struct to reference two buffer at once
typedef struct {
    Buffer* input_buff;
    Buffer* output_buff;
} buffer_state;

Buffer* create_mmap(size_t size){
  //These are the neccessary arguments for mmap. See man mmap.
  void* addr = 0;
  int protections = PROT_READ|PROT_WRITE; //can read and write
  int flags = MAP_SHARED|MAP_ANONYMOUS; //shared b/w procs & not mapped to a file
  int fd = -1; //We could make it map to a file as well but here it is not needed.
  off_t offset = 0;

  //Create memory map
  Buffer* state =  mmap(addr, size, protections, flags, fd, offset);

  if (( void *) ERROR == state){//on an error mmap returns void* -1.
    perror("error with mmap");
    exit(EXIT_FAILURE);
  }

  return state;
}

void deleteMMAP(void* addr){
    //This deletes the memory map at given address. see man mmap
    if (ERROR == munmap(addr, sizeof(Buffer))){
        perror("error deleting mmap");
        exit(EXIT_FAILURE);
    }
}

pid_t forkChild(void (*function)(buffer_state*), buffer_state* state) {
     //This function takes a pointer to a function as an argument
     //and the functions argument. It then returns the forked child's pid.

    pid_t childpid;
        switch (childpid = fork()) {
            case ERROR:
                perror("fork error");
                exit(EXIT_FAILURE);
            case 0:
                (*function)(state);
            default:
                return childpid;
        }
}

void waitForChildren(pid_t* childpids){
    int status;
    while(ERROR < wait(&status)){ //Here the parent waits on any child.
        if(!WIFEXITED(status)){ //If the termination err, kill all children.
            kill(childpids[0], SIGKILL);
            kill(childpids[1], SIGKILL);
            kill(childpids[2], SIGKILL);
            kill(childpids[3], SIGKILL);
            break;
        }
    }
}

void input_thread(buffer_state* state) {
    char c;
    while(true){
        c = getchar();
        //saving char to the input buffer
        deposit(state->output_buff, c);
        if (c == EOF) {
            exit(0);
        }
    }
}

void case_space_thread(buffer_state* state) {
    char c;
    while(true){
        //getting char from input buffer
        c = remoove(state->input_buff);
        // replace for edge case of char == newline
        if (c == '\n'){
            c = ' ';
        } 
        //saving char to middle buffer
        deposit(state->output_buff, c);
        if (c == EOF){
            exit(0);
        }
    }
}

void case_asterisks_thread(buffer_state* state) {
    char c, nextC;
    while(true){
        c = remoove(state->input_buff);
        if (c == '*'){
            nextC = remoove(state->input_buff);
            // replace ** with ^ case
            if (nextC == '*'){
                deposit(state->output_buff, '^');
            }
            // case if both letters different
            else {
                deposit(state->output_buff, c);
                deposit(state->output_buff, nextC);
            }
        }
        // save everthing to output buffer to be print out
        else {
            deposit(state->output_buff, c);
        }
        if (c == EOF){
            exit(0);
        }
    }
}

void output_thread(buffer_state* state) {
    char input_arr[MAX_SIZE];
    int index = 0;
    while(true){
        // print the array if array is full
        if (index == MAX_SIZE){
            puts(input_arr);
            index = 0;
        }
        // get char from output buffer
        char c = remoove(state->input_buff);
        if (c == EOF){
            exit(0);
        }
        // saving char to the array to be printed
        input_arr[index] = c;
        index++;
    }
}

int main() {
    // initialize memory maps for the shared buffers
    // mmap allows programs to manipulate pages in the virtual memory.
    // Here create_mmap function starts at virtual address 0, so all processes are accesing the same memory location hence sharing the buffers. 
    Buffer* input_buff = create_mmap(sizeof(Buffer));
    Buffer* middle_buff = create_mmap(sizeof(Buffer));
    Buffer* output_buff = create_mmap(sizeof(Buffer));
    init(input_buff);
    init(middle_buff);
    init(output_buff);

    // creating states
    buffer_state input_state = {
        NULL,
        input_buff
    };

    buffer_state space_state = {
        input_buff,
        middle_buff
    };

    buffer_state asterisk_state = {
        middle_buff,
        output_buff
    };

    buffer_state output_state = {
        output_buff,
        NULL
    };
    // initializing child processes 
    pid_t child_pids[4];
    child_pids[0] = forkChild(input_thread, &input_state);
    child_pids[1] = forkChild(case_space_thread, &space_state);
    child_pids[2] = forkChild(case_asterisks_thread, &asterisk_state);
    child_pids[3] = forkChild(output_thread, &output_state);
    // wait for process to join back
    waitForChildren(child_pids);
    // delete shared memory map when finish
    deleteMMAP(input_buff);
    deleteMMAP(middle_buff);
    deleteMMAP(output_buff);

    return 0;
}

