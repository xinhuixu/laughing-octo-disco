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
      printf("HOME = %d\n", HOME);
      HOME = home_process(buffer);
      
    } else if (HOME == 0) {
      
      char new_proj_name[MESSAGE_BUFFER_SIZE];
      strcpy(new_proj_name, buffer);
      new_proj(new_proj_name);

      printf("[SERVER %d] new project [%s] created by [%s]\n",pid, new_proj_name, username);
      strcpy(buffer, "Project created.");
      HOME = 999;
    }

    write(sd, buffer, sizeof(buffer));
  }
}
int home_process( char* buffer ){
  
  if ( strcmp(buffer, "0") == 0 ){
    printf("home_process(0)\n");
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
    return 999;
  }

}

void new_proj( char* buffer ){
  printf("new_proj: buffer: %s\n", buffer);
  char cmd[1000];
  char** argv;
  sprintf(cmd, "mkdir projects/%s", buffer);
  parse(cmd, argv);
  execute(argv);
  printf("executed argv\n");
}

/* borrowed from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html */
void  parse(char *line, char **argv){
  while (*line != '\0') {       /* if not the end of line ....... */ 
    while (*line == ' ' || *line == '\t' || *line == '\n')
      *line++ = '\0';     /* replace white spaces with 0    */
    *argv++ = line;          /* save the argument position     */
    while (*line != '\0' && *line != ' ' && 
	   *line != '\t' && *line != '\n') 
      line++;             /* skip the argument until ...    */
  }
  *argv = '\0';                 /* mark the end of argument list  */
}

void  execute(char **argv)
{
  pid_t  pid;
  int    status;

  if ((pid = fork()) < 0) {     /* fork a child process           */
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  }
  else if (pid == 0) {          /* for the child process:         */
    if (execvp(*argv, argv) < 0) {     /* execute the command  */
      printf("*** ERROR: exec failed\n");
      exit(1);
    }
  }
  else {                                  /* for the parent:      */
    while (wait(&status) != pid)       /* wait for completion  */
      ;
  }
}

