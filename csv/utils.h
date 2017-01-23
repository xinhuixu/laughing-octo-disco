#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

int parse_csv(char *filename, char arr[100][4][1024]);
int add_row(char arr[100][4][1024], char *uname, char *task, char *dline, char *stat, int rows, int cols);
int add_helper(char add[4][1024], char arr[100][4][1024], int rows, int cols);
int edit_dline(char arr[100][4][1024], char *uname, char *task, char *newdline, int rows, int cols);
int edit_status(char arr[100][4][1024], char *uname, char *task, char *newstat, int rows, int cols);
int remove_row(char arr[100][4][1024], char *uname, char *task, int rows, int cols);
void print_arr(char arr[1024][4][1024], int rows, int cols);
void write_to_file(char *filename, char arr[100][4][1024], int rows, int cols);
