#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

//ty stackoverflow: http://stackoverflow.com/questions/20013693/read-csv-file-to-a-2d-array-on-c

int main() {

  FILE *fd = fopen("test.txt", "r");
  char buf[1024];
  char *line, *record;
  int i=0, j=0;
  char arr[10][10];

  for(i=0; i<10; i++) {
    for(j=0; j<10; j++) {
      arr[i][j] = 0;
    }
  }
  
  if( fd == NULL ) {
    printf("Couldn't open file!\n");
    return -1;
  }
  while( (line = fgets(buf, sizeof(buf), fd)) != NULL ) {
    record = strtok(line, ",");
    while( record != NULL ) {
      printf("record: %s\n", record);
      arr[i][j++] = *record;
      printf("arr[i][j]: %s\n", arr[i][j]);
      record = strtok(NULL, ",");
    }
    ++i;
  }

  fclose(fd);
  /*
  for(i=0; i<10; i++) {
    printf("[");
    for(j=0; j<10; j++) {
      printf("  %d", arr[i][j]);
    }
    printf("  ]\n");
  }
  */
  return 0;
}
