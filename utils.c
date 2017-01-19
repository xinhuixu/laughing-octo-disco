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
  
  FILE *fd = fopen("test.txt", "r"); //open file
  char buf[1024]; //file will be read into this
  char *line, *record; //for reading into array purposes
  int i=0, j=0, k=0; //counters
  int rows=3, cols=0; //for printing purposes
  char arr[10][3][1024]; //array!

  //just in case data is missing, etc
  char *add = "none";
  for(i=0; i<10; i++) {
    for(j=0; j<3; j++) {
      for(k=0; k<strlen("none")+1; k++, add++) {
	strcpy(&arr[i][j][k], add);
      }
      add = "none";
    }
  }

  i=0; j=0; //reset counters
  //error handling for file opening
  if( fd == NULL ) {
    printf("Couldn't open file!\n");
    return -1;
  }
  //read file line-by-line
  while( (line = fgets(buf, sizeof(buf), fd)) != NULL ) {
    //split line by commas, strip newline
    record = strtok(strtok(line, "\n"), ",");
    //"iterate" thru values in record
    while( record != NULL ) {
      //and copy value char-by-char into the array
      add = record;
      for(k=0; k<strlen(record)+1; k++, add++) {
	strcpy(&arr[i][j][k], add);
      }
      //move to next value in record
      record = strtok(NULL, ",");
      j++;
    }
    j=0;
    i++;
    cols++;
  }

  //printy things
  for(i=0; i<cols; i++) {
    printf("[");
    for(j=0; j<rows; j++) {
      printf("  %s", arr[i][j]);
    }
    printf("  ]\n");
  }

  //close file
  fclose(fd);

  return 0;
}
