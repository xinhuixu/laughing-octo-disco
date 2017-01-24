//#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include "networking.h"
#include "func.h"

typedef int bool;
#define true 1
#define false 0

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
  bool USER = false;
  int HOME;
  int PROJECT = -1;
  char buffer[MESSAGE_BUFFER_SIZE];
  char home[MESSAGE_BUFFER_SIZE];
  strcpy(home, "[0]New project\t[1]My projects");
  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login [%s]\n", pid, username);

  if ( user_exists(username) ){
    USER = true;
  } else {
    create_user(username);
    USER = true;
  }
    
  /* OPEN HOME */
  HOME = 999;
  strcpy(buffer, home);
  write(sd, buffer, sizeof(buffer));

  /* READ/WRITE TO SD LOOP */
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", pid, buffer );
    printf("HOME = %d\n",HOME);

    if ( strcmp(buffer, "home") == 0){
      strcpy(buffer, home);
      HOME = 999;
    } else if (HOME == 999) {
      HOME = home_process(buffer, username);      

      /*PROJECT CREATION LOOP*/  
    } else if (HOME == 0) {      
      char new_proj_name[MESSAGE_BUFFER_SIZE];
      strcpy(new_proj_name, buffer);
      if (USER && !proj_exists(username, new_proj_name)) {
	new_proj(new_proj_name, username);
	printf("[SERVER %d] new project [%s] created by [%s]\n",pid, new_proj_name, username);
	sprintf(buffer, "Project created.\n%s", home);
      } else {
	sprintf(buffer, "Project already exists.\n%s", home);
      }
      HOME = 999;

      /*PROJECT MANAGING LOOP*/
    } else if (HOME == 1) {
      if (PROJECT == -1) {
	//paste all projs into buffer, set PROJECT
	PROJECT = view_proj(buffer, username);
      } else if (PROJECT){
	PROJECT = proj_process(buffer, PROJECT, username);
      } else if (PROJECT == 2){

      }
    } else if ( strcmp(buffer, "exit") == 0){
      exit(0);
    }

    write(sd, buffer, sizeof(buffer));
  }
}
