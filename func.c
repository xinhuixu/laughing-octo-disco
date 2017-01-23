#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#include "func.h"
#include "utils.c"

int home_process( char* buffer, char* username ){
  if ( strcmp(buffer, "0") == 0 ){
    strcpy(buffer, "Enter title of new project:");
    return 0;
  } else if ( strcmp(buffer, "1") == 0) {
    list_projs(buffer, username);
    return 1;
  } else {
    strcpy(buffer, "Invalid command.\n[0]New project\t[1]My projects");
    return 999;
  }
}

void create_user( char* username ){
  char user_dir[100];
  sprintf(user_dir, "projects/%s", username);
  int err = mkdir(user_dir, 0700);
  if (err)
    perror("create_user: mkdir failed.\n");
}

bool user_exists( char* username ){
  DIR *d = NULL;
  struct dirent *de = NULL;
  d = opendir("projects");
  //printf("USER DIRECTORIES:\n");
  while ( (de = readdir(d)) ){
    //printf("\t%s\n",de->d_name);
    if (strcmp(de->d_name, username) == 0) {
      return true;
    }
  }
  closedir(d);
  return false;
}

bool proj_exists( char* username, char* proj_name ){
  DIR *d = NULL; struct dirent *de = NULL;
  char path[100];
  sprintf(path, "projects/%s", username);  
  d = opendir(path);
  //printf("%s's projects:\n", username);
  //TODO: CHECK ALL PROJECTS//
  while ( (de = readdir(d)) ){
      if (strcmp(de->d_name, proj_name) == 0)
	return true;    
  }
  return false;
}

void list_projs( char* buffer, char* username ){
  DIR *d = NULL; struct dirent *de = NULL;
  DIR *sub = NULL; struct dirent *subde = NULL;
  char path[100];
  char *proj; proj = (char *)malloc(50 * sizeof(char));
  int i = 1;

  sprintf(path, "projects");
  d = opendir(path);
  while( (de = readdir(d)) ) {
    printf("de->d_name: %s\n", de->d_name);
    if( (strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0) ) {
      
      printf("now i am here\n");
    }
    else if( (strcmp(de->d_name, username) == 0) ) {
      printf("hit my own folder!\n");
      sprintf(proj, "\t[%d]\%s\n", i, de->d_name);
      strcat(buffer, proj);
      free(proj);
      i++;
    }
    else {
      printf("hit someone else's folder!");
      printf("subde->d_name: %s\n", subde->d_name);
      while( (subde = readdir(sub)) ) {
	if( (strcmp(subde->d_name, ".") == 0) || (strcmp(subde->d_name, "..") == 0) ) {
	  ;
	}
	else {
	  sprintf(path, "projects/%s/%s/members.csv", de->d_name,subde->d_name);
	  printf("path: %s\n", path);
	  char arr[100][4][1024];
	  int cols=1, rows=parse_csv(path, arr); int r=0, c=0;
	  for(r=0; r<rows; r++) {
	    for(c=0; c<cols; c++) {
	      if( strncmp(username, arr[r][c], strlen(username)) == 0 ) {
		char *proj;
		sprintf(proj, "\t[%d]%s\n", i, subde->d_name);
		strcat(buffer, proj);
		free(proj);

		i++;
	      }
	    }
	  }
	}
      }
    }
  }
  
  strcat(buffer, "Enter project number to view/edit.");

}

void build_array( char array[100][100], char* username ){
  DIR *d = NULL; struct dirent *de = NULL;
  char path[100];
  int i = 1;
  sprintf(path, "projects/%s", username);  
  d = opendir(path);
  while ( (de = readdir(d)) ){
    if ( (strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0) ) {
      ;
    } else {
      strcpy(array[i], de->d_name);
      i++;
    }
  }
}

void get_proj_name( char* proj_name, char* username, char* proj_num ){
  char array[100][100];
  int num = atoi(proj_num);
  build_array(array, username);
  strcpy(proj_name, array[num]);
}
		    
void new_proj( char* new_proj_name, char* username ){
  char path[100];

  sprintf(path, "projects/%s/%s", username, new_proj_name);
  int err = mkdir(path, 0700);
  if (err)
    printf("new_proj: mkdir failed.\n");

  sprintf(path, "projects/%s/%s/members.csv", username, new_proj_name);
  
  int members = open(path, O_CREAT|O_RDWR, 0777);
  //printf("opened members: %d, path: %s\n", members, path);
  write(members, username, sizeof(username));
  write(members, "\n", sizeof(char));
  close(members);

  sprintf(path, "projects/%s/%s/tasks.csv", username, new_proj_name);
  int tasks = open(path, O_CREAT|O_RDWR, 0777);
  //printf("opened tasks\n");
  close(tasks);
  
}

int count_projs( char* username ){
  DIR *d = NULL; struct dirent *de = NULL;
  char path[100];
  int i = 0;
  sprintf(path, "projects/%s", username);  
  d = opendir(path);  
  while ( (de = readdir(d)) ){
    if ( (strcmp(de->d_name, ".") == 0) || (strcmp(de->d_name, "..") == 0) ) {
      ;
    } else {
      i++;
    }
  }
  return i;
}

int view_proj( char* buffer, char* username){
  bool valid = false;
  int num_projs;
  char proj_name[100];
  num_projs = count_projs(username);

  char i[4];
  int j = 1;
  
  while (j <= num_projs){
    sprintf(i, "%d", j);

    if (strcmp(i, buffer) == 0) {

      /*TODO: paste LIST OF MEMBERS hto buffer*/

      get_proj_name(proj_name, username, i);
      sprintf(buffer, "Project[%s]: %s\n", i, proj_name);
      strcat(buffer,"[0]All tasks\t[1]My tasks\t[2]Add member\t[3]New task");
  
      valid = true;
      break;
    }
    j++;
      
  }
  if (strcmp(buffer, "home") == 0)
    valid = true;
  
  if (valid == false){
    char projs[1000];
    list_projs(projs, username);
    sprintf(buffer, "Invalid command.\n%s", projs);
    return 0;
  } else {
    return j;
  }
}

int proj_process( char* buffer, int proj_num, char* username ){
  if (strcmp(buffer, "0") == 0){
    /*TODO: PASTE ALL TASKS INTO BUFFER*/
    return 0;
  } else if (strcmp(buffer, "1") == 0){
    /*TODO: PASTE THIS username'S TASKS INTO BUFFER*/
    return 1;
  } else if (strcmp(buffer, "2") == 0){
    /*TODO: add member, prompt for new member name, manager only*/
    char proj[100];
    char num[4]; sprintf(num, "%d", proj_num);
    get_proj_name(proj, username, num);
    if (is_manager(username, proj)){
      sprintf(buffer, "Enter username of new member in [%s]:", proj);
      return 2;
    } else {
      sprintf(buffer, "You are not authorized to use this command.");
      return -1;
    }
    
  } else if (strcmp(buffer, "3") == 0){
    /*TODO: new task, manager only*/
  }

  return -1;
}

/*get_manager(manager, project name)
populate manager with the manager of the project*/
void get_manager( char* manager, char* proj_name ){



}

bool is_manager( char* username, char* proj_name ) {
  char path[100];
  sprintf(path, "projects/%s/%s/members.csv", username, proj_name);  
  FILE * members = fopen(path, "r");  

  if (errno)
    return false;
  else
    return true;
  
  /*char buf[20];  
  fgets(buf, 20, members); //fgets reads until first newline
  if (strcmp(buf, username) == 0){
    fclose(members);
    return true;
  }
  fclose(members);
  return false;*/
}

void all_tasks( char* buffer, char* username );
void my_tasks( char* buffer, char* username );
void add_task( char* buffer, char* proj_name, char* username, char *task, char *deadline ) {

  char path[100];
  sprintf(path, "projects/%s/%s/tasks.csv", username, proj_name);
  char arr[100][4][1024];
  int r = parse_csv(path, arr);

  int a = add_row(path, arr, username, task, deadline, "Not yet started", r, 4);
  if( a == r+1 )
    sprintf(buffer, "Great! You successfully added assigned %s [%s], due %s.\n", username, task, deadline);
  else
    sprintf(buffer, "Sorry! This task was not added.\n");

}

void remove_task( char* proj_name, char* username, char *task, char* buffer ) {

  char path[100];
  sprintf(path, "projects/%s/%s/tasks.csv", username, proj_name);
  char arr[100][4][1024];
  int r = parse_csv(path, arr);

  int rem = remove_row(path, arr, username, task, r, 4);
  if( rem == r-1 )
    sprintf(buffer, "Great! You successfully removed [%s] from %s.\n", task, username);
  else
    sprintf(buffer, "Sorry! This task was not removed.\n");
  
}

void remove_member( char* to_rem );
void add_member( char* new_member );
void update_status( char* buffer, char *username, char *task, char *new );

/* borrowed from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html */
/* lmao were not using this probably*/

void parse_exec(char* cmd, char** argv){
  parse(cmd, argv);
  execute(argv);
}

void parse(char *line, char **argv){
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

