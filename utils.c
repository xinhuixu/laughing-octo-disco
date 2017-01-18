#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

int main() {

  FILE *fd = fopen("test.txt", "r");
  char buf[256];
  char*** lines = (char***)malloc(50);
  int j=0;
  
  while( fgets(buf, 256, fd) ) {
    char** line = (char**)malloc(50);
    int i = 0;
    char *x = buf;
    while((line[i] = strsep(&x, ","))) i++;
    i=0;
    printf("");
    while(line[i]) {
      printf("%s ", line[i]); i++;
    }
    lines[j] = line;
    j++;
  }

  int i=0;
  while(lines[i]) {
    printf("line[0]%s\n", lines[i][0]);
    
    //printf("[%s, %s, %s]\n", line[0], line[1], line[2]);
    i++;
  }

  fclose(fd);
  return 0;
}
