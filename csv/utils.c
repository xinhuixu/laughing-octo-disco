#include "utils.h"

//ty stackoverflow: http://stackoverflow.com/questions/20013693/read-csv-file-to-a-2d-array-on-c

int parse_csv(char *filename, char arr[100][4][1024]) {
  FILE *fd = fopen(filename, "r"); //open file
  char buf[1024]; //file will be read into this
  char *line, *record; //for reading into array purposes
  int i=0, j=0, k=0; //counters
  int cols=4, rows=0; //for printing purposes

  //just in case data is missing, etc
  char *add = "none";
  for(i=0; i<100; i++) {
    for(j=0; j<cols; j++) {
      for(k=0; k<strlen("none"); k++, add++) {
	strcpy(&arr[i][j][k], add);
      }
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
      //remove leading space
      if(strncmp(&add[0], " ", 1) == 0)
	add++;
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

int add_row(char arr[100][4][1024], char *uname, char *task, char *dline, char *stat, int rows, int cols) {
  char add[4][1024];
  int i=0;
  
  for(i=0; i<strlen(uname)+1; i++, uname++) {
    strcpy(&add[0][i], uname);
  }
  for(i=0; i<strlen(task)+1; i++, task++) {
    strcpy(&add[1][i], task);
  }
  for(i=0; i<strlen(dline)+1; i++, dline++) {
    strcpy(&add[2][i], dline);
  }
  for(i=0; i<strlen(stat)+1; i++, stat++) {
    strcpy(&add[3][i], stat);
  }

  return add_helper(add, arr, rows, cols);
}

int add_helper(char add[4][1024], char arr[100][4][1024], int rows, int cols) {
  int i=0, j=0;
  
  for(i=0; i<cols; i++) {
    char *tba = add[i];
    for(j=0; j<strlen(tba)+1; j++, tba++) {
      strcpy(&arr[rows][i][j], tba);
    }
  }

  rows++;
  write_to_file("tasks.txt", arr, rows, cols);
  
  return rows;
}

int edit_dline(char arr[100][4][1024], char *uname, char *task, char *newdline, int rows, int cols) {

  int i=0, j=0, k=0;
  for(i=0; i<rows; i++) {
    if( strncmp(arr[i][0], uname, strlen(uname)) == 0 ) {
      if( strncmp(arr[i][1], task, strlen(task)) == 0 ) {
	for(k=0; k<strlen(newdline+1); k++, newdline++) {
	  strcpy(&arr[i][2][k], newdline);
	}
	write_to_file("tasks.txt", arr, rows, cols);
        return 1;
      }
    }
  }

  return -1;
}

int edit_status(char arr[100][4][1024], char *uname, char *task, char *newstat, int rows, int cols) {

  int i=0, j=0, k=0;
  for(i=0; i<rows; i++) {
    if( strncmp(arr[i][0], uname, strlen(uname)) == 0 ) {
      if( strncmp(arr[i][1], task, strlen(task)) == 0 ) {
	for(k=0; k<strlen(newstat+1); k++, newstat++) {
	  strcpy(&arr[i][3][k], newstat);
	}
	write_to_file("tasks.txt", arr, rows, cols);
        return 1;
      }
    }
  }

  return -1;
}

int remove_row(char arr[100][4][1024], char *uname, char *task, int rows, int cols) {

  int i=0, j=0;
  int start=0;

  printf("one\n");
  for(i=0; i<rows; i++) {
    if( strncmp(arr[i][0], uname, strlen(uname)) == 0 ) {
      if( strncmp(arr[i][1], task, strlen(task)) == 0 ) {
	start = i;
      }
    }
  }

  printf("two\n");
  for(i=start; i<rows; i++) {
    for(j=0; j<cols; j++) {
      strcpy(arr[i][j], arr[i+1][j]);
    }
  }
  
  rows--;
  write_to_file("tasks.txt", arr, rows, cols);
  
  return rows;
}

//for testing ONLY
void print_arr(char arr[100][4][1024], int rows, int cols) {
  int i=0, j=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<cols-1; j++) {
      printf("%s, ", arr[i][j]);
    }
    printf("%s\n", arr[i][j]);
  }
}
  
void write_to_file(char *filename, char arr[100][4][1024], int rows, int cols) {

  char write[rows*cols*1024];
  write[0] = 0;
  
  int i=0, j=0, k=0;
  for(i=0; i<rows; i++) {
    for(j=0; j<cols-1; j++) {
      strcat(write, arr[i][j]);
      strcat(write, ", ");
    }
    strcat(write, arr[i][j]);
    if(i != rows-1) {
      strcat(write, "\n");
    }
    j=0;
  }
  
  remove(filename);
  FILE *fd = fopen(filename, "w");
  fprintf(fd, "%s", write);
  fclose(fd);
  
}

int main() {
  char arr[100][4][1024]; //array!
  int i=0,j=0; int cols=4,rows=0;

  rows = parse_csv("tasks.txt", arr);
  
  printf("ADDING ROW...\n");
  char uname[1024]; strcpy(uname, "Jefff_star");
  char task[1024]; strcpy(task, "eat a banana");
  char dline[1024]; strcpy(dline, "01/31/17 1900");
  char stat[1024]; strcpy(stat, "In progress");
  rows = add_row(arr, uname, task, dline, stat, rows, cols);
  
  printf("EDITING DEADLINE...\n");
  char *newdline = "01/31/17 1930";
  if(edit_dline(arr, uname, task, newdline, rows, cols))
  
  printf("EDITING STATUS...\n");
  char *newstat = "Complete";
  if(edit_status(arr, uname, task, newstat, rows, cols))
  
  printf("REMOVING ROW...\n");
  strcpy(task, "take out the trash");
  rows = remove_row(arr, uname, task, rows, cols);
  
  return 0;
}
