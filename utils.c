#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

//ty stackoverflow: http://stackoverflow.com/questions/20013693/read-csv-file-to-a-2d-array-on-c

int main() {
  FILE *fd = fopen("test.txt", "r");
  char buf[1024];
  char *line, *record;
  int i=0, j=0, k=0;
  int rows=3, cols=0;
  char arr[10][3][1024];

  char *add = "none";
  for(i=0; i<10; i++) {
    for(j=0; j<3; j++) {
      for(k=0; k<strlen("none")+1; k++, add++) {
	strcpy(&arr[i][j][k], add);
      }
      add = "none";
    }
  }
  
  i=0; j=0;
  if( fd == NULL ) {
    printf("Couldn't open file!\n");
    return -1;
  }
  while( (line = fgets(buf, sizeof(buf), fd)) != NULL ) {
    record = strtok(strtok(line, "\n"), ",");
    while( record != NULL ) {
      add = record;
      for(k=0; k<strlen(record)+1; k++, add++) {
	strcpy(&arr[i][j][k], add);
      }
      record = strtok(NULL, ",");
      j++;
    }
    j=0;
    i++;
    cols++;
  }
  
  for(i=0; i<cols; i++) {
    printf("[");
    for(j=0; j<rows; j++) {
      printf("  %s", arr[i][j]);
    }
    printf("  ]\n");
  }
  
  fclose(fd);

  return 0;
}
