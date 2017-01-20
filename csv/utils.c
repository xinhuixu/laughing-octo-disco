#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

//ty stackoverflow: http://stackoverflow.com/questions/20013693/read-csv-file-to-a-2d-array-on-c

int parse_csv(char arr[100][3][1024]) {
  FILE *fd = fopen("test.txt", "r"); //open file
  char buf[1024]; //file will be read into this
  char *line, *record; //for reading into array purposes
  int i=0, j=0, k=0; //counters
  int cols=3, rows=0; //for printing purposes

  //just in case data is missing, etc
  char *add = "none";
  for(i=0; i<100; i++) {
    for(j=0; j<cols; j++) {
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
    rows++;
  }

  fclose(fd);

  return rows;
}

int main() {
  char arr[100][3][1024]; //array!
  int i=0,j=0; int cols=3,rows=parse_csv(arr);
 
  //testy printy things
  for(i=0; i<rows; i++) {
    printf("[");
    for(j=0; j<cols; j++) {
      printf("  %s", arr[i][j]);
    }
    printf("  ]\n");
  }

  return 0;
}
