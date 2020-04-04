// Name: Chase Wang
// Honor Code pledge signature: Chase Wang
// Program description: this is a simple shell program server with socket implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include "Socket.h"

#define MAX_SIZE 10000
#define TOK_CHAR " \t\n\0"
#define ARG_LEN 32
#define MAX_NAME_LEN 40

ServerSocket serverSocket;
Socket mainSocket;
char temp_file[MAX_NAME_LEN];

void get_message(char *line){
  for (int i = 0; i < MAX_SIZE; ++i){
    // loops through to get 
    int c = Socket_getc(mainSocket);
    // quit when EOF  
    if (c == EOF){
	    remove(temp_file);       
	    exit(1);
    }
    line[i] = c;
    // break when recieved all the chars
    if (c == '\0'){
        return;
    }
  }
}

int parsing_input(char *line, char **commands){
  int count = 0;
  char *token = strtok(line, TOK_CHAR);
  while ( token != NULL ){
      *commands = token;
      commands++;
      token = strtok(NULL, TOK_CHAR);
      count++;
    }
    *commands = token;
  if(count > ARG_LEN){
      return -1;
  }
  return 0;
}

void send_to_client(){
  FILE *temp;
  // check if the file exist
  if ((temp = fopen(temp_file, "r")) == NULL){
      printf("cannot open file\n");
      exit(1);
    }
  // send chars from the file to client via socket
  int c;
  do{
    c = getc(temp);
    if (c == EOF){
        break;
    }
    Socket_putc(c, mainSocket);
  }while (c != EOF);
  // terminate the message with null char
  Socket_putc('\0', mainSocket);
  fclose(temp);
  // remove file after finishing
  remove(temp_file);
}

int main(int argc, char *argv[]){
  int child_status;
  pid_t child;
  char line[MAX_SIZE];
  FILE *temp;
  if (argc < 2){
    fprintf(stdout,"%s", "too few arguments\n");
    exit(1);
  }
  // get incoming connection
  serverSocket = ServerSocket_new(atoi(argv[1]));
  if (serverSocket < 0){
    fprintf(stdout, "%s", "server socket connection failed\n");
    exit(1);
  }
  mainSocket = ServerSocket_accept(serverSocket);
  if (mainSocket < 0){
    fprintf(stdout, "%s", "socket connection failed\n");
    exit(1);
  }
  Socket_close(serverSocket); 

  while (1){
    // opens a temp file to redirect output to
    sprintf(temp_file, "child_file_%d", (int)getpid()); 
    temp = freopen(temp_file, "a", stdout); 
    // get client's input from socket
    get_message(line);
    // run fork to execute child program
    child = fork(); 
    if (child < 0){
      perror("no child process");
      exit(1);
    }else if (child == 0){
    int arg_flag = parsing_input(line, argv);
    //   printf(argv); 
      if (arg_flag == -1){
        printf("Error occured, caused by too many arguments.\n");
        exit(1);
      }
      if (execvp(*argv, argv) == -1 ){
	    printf("Error occured, caused by execvp.\n");
        exit(1);
      }      
    }else{
      // wait for all processes to terminate
      if (waitpid(-1, &child_status, 0) == -1) {
	    printf("Server halting error\n");
      }
      // when child finishes close the temp file and return result back to child
      fclose(temp); 
      send_to_client();
    }
  }
}



