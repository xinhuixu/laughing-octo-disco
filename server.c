#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );

int main() {
  int sd, connection;

  sd = server_setup();
    
  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {
  int pid;
  bool USERNAME_SET = false;
  bool HOME = false; //new project my project: 0, 1
  char buffer[MESSAGE_BUFFER_SIZE];
  char instruct[MESSAGE_BUFFER_SIZE];
  char invalid[MESSAGE_BUFFER_SIZE];
  strcpy(invalid, "Invalid command.");
  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login: %s\n", pid, username);
  USERNAME_SET = true;

  strcpy(instruct, "New project[0]\tMy projects[1]");
  write(sd, instruct, sizeof(instruct));
  
  while (HOME == false){
    read( sd, buffer, sizeof(buffer));
    if ( strcmp(buffer, "0") == 0 ){
      //new_proj();
      strcpy(instruct, "Enter title of new project:");
      write(sd, instruct, sizeof(instruct)); 
      HOME = true;
    }
    else if ( strcmp(buffer, "1") == 0) {
      //my_proj();
      strcpy(instruct, "[placeholder] List of projects");
      write(sd, instruct, sizeof(instruct)); 
      HOME = true;
    }
    else {
      //      write(sd, invalid, sizeof(invalid)); doesnt work
      write(sd, instruct, sizeof(instruct)); 
      
    }
  }
  
  
  //char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", pid, buffer );
    process( buffer);
    write( sd, buffer, sizeof(buffer));    
  }
  
}

/*
void process( char* buffer, char* instruct){


}*/

void process( char* s ){

  /* DIR* dir = opendir("projects/demo_proj");
  if (dir) {
    printf("dir exists\n");
  }
  else if (ENOENT == errno) {
    printf("dir does not exist\n");
  }
  else {
    perror("something wrong\n");
    }*/

  strncpy( s, "placeholder", 12);
  
}

/*
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
*/

