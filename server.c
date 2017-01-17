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
  
  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login: %s\n", pid, username);
  USERNAME_SET = true;
  
  char instruct[MESSAGE_BUFFER_SIZE];
  strcpy(instruct, "New project[0]\tMy projects[1]");
  write(sd, instruct, sizeof(instruct)); 

  
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", pid, buffer );
    process( buffer, instruct );
    write( sd, buffer, sizeof(buffer));    
  }
  
}

void process( char* buffer, char* instruct){


}

/*
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

