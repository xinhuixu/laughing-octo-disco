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
  int HOME = -1;
  int PROJECT = -1;
  int TASK = -1;

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
    printf("HOME [before] = %d\t",HOME);
    printf("PROJECT [before] = %d\t",PROJECT);
    printf("TASK [before] = %d\n",TASK);
    
    if ( strcmp(buffer, "home") == 0){
      strcpy(buffer, home);
      HOME = 999;
      PROJECT = -1; //VERY IMPORTANT
      TASK = -1;

    } else if ((HOME == 999) || (PROJECT == 404)) {
      HOME = home_process(buffer, username);      
      TASK = -1;
      PROJECT = -1;

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
      PROJECT = -1;
      TASK = -1;

      /*PROJECT MANAGING LOOP*/
    } else if (HOME == 1) {
      
      if (PROJECT == -1) {
	//paste all projs into buffer, set PROJECT
	PROJECT = view_proj(buffer, username);

      } else if ((PROJECT == 1 )&& (TASK == -1)){
	PROJECT = proj_process(buffer, PROJECT, username);
	if (PROJECT == 1)
	  TASK = 0;
	
      } else if ( PROJECT && (TASK == -1 ) ) {
	PROJECT = proj_process(buffer, PROJECT, username);

      } else if (PROJECT == 1) {
	/*IN VIEWING TASKS MODE*/
	printf("TASK=%d\n", TASK);
	if (TASK == 0){
	  TASK =  task_view(buffer, atoi(buffer), username);
	} else {
	  TASK = task_process(buffer, TASK, username);
	  HOME = 999;
	  PROJECT = -1;
	}

 
      /* } else if ((PROJECT == 2) ) { */
      /* 	/\* ADD MEMBER *\/ */
      /* } else if ((PROJECT == 3) ){ */
      /* 	/\* MINI TASK-ASSSIGNMENT LOOP *\/ */
      /* 	char task[100][10]; */
      /* 	printf("TASK_ASS LOOP: buffer: %s\n", buffer); */
	
      } else {
	sprintf(buffer, "Something's wrong. Enter 'home' to return home.");
      }
      
    } else {

      	sprintf(buffer, "Something's wrong. Enter 'home' to return home.");
    }

    printf("HOME [after] = %d\t", HOME);
    printf("PROJECT [after] = %d\t", PROJECT);
    printf("TASK [after] = %d\n",TASK);
    write(sd, buffer, sizeof(buffer));
  }
}
