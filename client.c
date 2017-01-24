#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"


void first_prompt(){
  char *p;
  p = (char *) malloc(sizeof(char)*100);
  strcpy(p, "\nWelcome to Grape^tm (beta)! To get started, type in a username. Then follow the prompts!\n>> ");
  printf("%s", p);
  free(p);
}

void prompt(){
  char *p;
  p = (char *) malloc(sizeof(char)*3);
  strcpy(p, ">> ");
  printf("%s", p);
  free(p);
}

int main( int argc, char *argv[] ) {
  int first = 1;
  
  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  int sd;

  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];
  
  
  while (1) {
    if (first) {
      first_prompt();
      first = 0;
    }
    else {
      prompt();
    }
    fgets( buffer, sizeof(buffer), stdin );

    char *p = strchr(buffer, '\n');
    *p = 0;
  
    write( sd, buffer, sizeof(buffer) );

    read( sd, buffer, sizeof(buffer) );
    printf( "%s\n", buffer );
   
  }
  
  return 0;
}


/*
int main() {
  
  char *input;
  while ((input = readline(prompt())) != NULL) {
    if ((input[0] != 0)){
      printf("%d\n",strlen(input));
    }
  }
  return 0;
}
*/
