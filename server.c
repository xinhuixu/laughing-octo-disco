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
  bool HOME = false;
  bool INVALID = false;
  char buffer[MESSAGE_BUFFER_SIZE];

  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login: %s\n", pid, username);
  USERNAME_SET = true;

  HOME = true;
  strcpy(buffer, "New project[0]\tMy projects[1]");
  write(sd, buffer, sizeof(buffer));

  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", pid, buffer );

    if (HOME){
      HOME = home_process( buffer);
    }
    
    write( sd, buffer, sizeof(buffer));    
  }
  
}

bool home_process( char* buffer ){
  
  if ( strcmp(buffer, "0") == 0 ){
    //new_proj();
    strcpy(buffer, "Enter title of new project:");
    return false;
  }
  else if ( strcmp(buffer, "1") == 0) {
    //my_proj();
    strcpy(buffer, "[placeholder] List of projects");
    return false;
  }
  else {
    strcpy(buffer, "New project[0]\tMy projects[1]\nInvalid command.");
    return true;
  }

}

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

