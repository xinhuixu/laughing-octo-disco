#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>

char* prompt(){
  char *p;
  p = (char *) malloc(sizeof(char)*3);
  strcpy(p, ">>");
  return p;
}

int main() {
  
  char *input;
  while ((input = readline(prompt())) != NULL) {
    if ((input[0] != 0)){
      printf("%d\n",strlen(input));
    }
  }
  return 0;
}
