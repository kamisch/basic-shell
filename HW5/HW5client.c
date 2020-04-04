//Name: Chase Wang
//Honor Code pledge signature: Chase Wang
//Program description: this is a simple shell program client with socket implementation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Socket.h"

#define MAX_SIZE 1000

int main(int argc, char* argv[]) {
    Socket socket;
    // Make sure we received the necessary arguments.
    if (argc < 3) {
        fprintf(stderr, "%s", "Too few arguments\n");
        exit(1);
    }
    // connecting server, atoi converts port input to int type 
    socket = Socket_new(argv[1], atoi(argv[2]));
    if (socket < 0) {
        fprintf(stderr, "%s", "Connection failed\n");
        exit(1);
    }
    while (1) {
        printf("%% ");
        // get input string
        char line[MAX_SIZE];
        // EOF
        if (fgets(line,sizeof(line),stdin) == NULL){
            break;
        }
        // sending characters to server
        int l;
        for (int i = 0; i < MAX_SIZE; i++) {
            l = line[i];
            Socket_putc(l, socket);
            if (l == '\0'){
                i = MAX_SIZE;
            }
        }
        // recieving characters from server
        char c;
        int recieving = 1;
        while (recieving) {
            c = Socket_getc(socket);
            if (c == '\0') {
                recieving = 0;
            }else{
                putc(c, stdout);
            }
        }
    }
    // exiting program
    Socket_putc(EOF,socket);
    Socket_close(socket);
    exit(0);
    return 0;
}