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
  int HOME;
  bool INVALID = false;
  char buffer[MESSAGE_BUFFER_SIZE];

  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login: %s\n", pid, username);
  USERNAME_SET = true;

  /* OPEN HOME */
  HOME = 999;
  strcpy(buffer, "New project[0]\tMy projects[1]");
  write(sd, buffer, sizeof(buffer));

  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", pid, buffer );

    if (HOME == 999) {
      HOME = home_process( buffer);      
    } else if (HOME == 0) {
      char new_proj_name[MESSAGE_BUFFER_SIZE];
      strcpy(new_proj_name, buffer);
      printf("[SERVER %d] new project [%s] created by [%s]\n",pid, new_proj_name, username);
      strcpy(buffer, "Project created.");
      HOME = 999;
    }

    write(sd, buffer, sizeof(buffer));
  }
}
int home_process( char* buffer ){
  
  if ( strcmp(buffer, "0") == 0 ){
    //new_proj();
    strcpy(buffer, "Enter title of new project:");
    return 0;
  }
  else if ( strcmp(buffer, "1") == 0) {
    //my_proj();
    strcpy(buffer, "[placeholder] List of projects");
    return 1;
  }
  else {
    strcpy(buffer, "New project[0]\tMy projects[1]\nInvalid command.");
    return -1;
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

