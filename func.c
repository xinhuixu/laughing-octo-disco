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
    if( list_projs(buffer, username) )
      return 1;
    else
      return 0;
  } else {
    strcpy(buffer, "Invalid command.\n[0]New project\t[1]My projects");
    return 999;
  }
}

void create_user( char* username ){
  char user_dir[100];
  sprintf(user_dir, "users/%s", username);
  
  int err = mkdir(user_dir, 0700);
  if (err)
    perror("create_user: mkdir failed.\n");
}

bool user_exists( char* username ){
  DIR *d = NULL;
  struct dirent *de = NULL;
  d = opendir("users");
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
  sprintf(path, "users/%s", username);  
  d = opendir(path);
  //printf("%s's projects:\n", username);
  while ( (de = readdir(d)) ){
      if (strcmp(de->d_name, proj_name) == 0)
	return true;    
  }
  return false;
}

bool list_projs( char* buffer, char* username ){
  sprintf(buffer, "%s's projects:\n", username);
  
  char path[100];
  char *proj; proj = (char *)malloc(50 * sizeof(char *));

  bool PROJ_EXISTS = false;
  
  sprintf(path, "users/%s/pii.csv", username);
  
  int i=1;
  char arr[100][4][1024];
  int rows = parse_csv(path, arr), cols=1; int r=0, c=0;
  for(r=0; r<rows; r++) {
    for(c=0; c<cols; c++) {
      sprintf(proj, "\t[%d] %s\n", i, arr[r][c]);
      strcat(buffer, proj);
      i++;
    }
  }

  if (i > 1)
    PROJ_EXISTS = true;
  
  strcat(buffer, "Enter project number to view/edit.");

  return PROJ_EXISTS;
}

void build_array( char array[100][100], char* username ){

  char path[100];
  sprintf(path, "users/%s/pii.csv", username);

  int i=1;
  char arr[100][4][1024];
  int rows = parse_csv(path, arr), cols=1; int r=0, c=0;
  for(r=0; r<rows; r++) {
    for(c=0; c<cols; c++) {

      strcpy(array[i], arr[r][c]);

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

  sprintf(path, "users/%s/%s", username, new_proj_name);
  int err = mkdir(path, 0700);
  if (err)
    printf("new_proj: mkdir failed.\n");

  sprintf(path, "users/%s/%s/members.csv", username, new_proj_name);
  
  int members = open(path, O_CREAT|O_RDWR, 0777);
  //printf("opened members: %d, path: %s\n", members, path);
  write(members, username, sizeof(username));
  write(members, "\n", sizeof(char));
  close(members);

  sprintf(path, "users/%s/%s/tasks.csv", username, new_proj_name);
  int tasks = open(path, O_CREAT|O_RDWR, 0777);
  //printf("opened tasks\n");
  close(tasks);
  
}

int count_projs( char* username ){
  DIR *d = NULL; struct dirent *de = NULL;
  char path[100];
  int i = 0;
  sprintf(path, "users/%s", username);  
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
      strcat(buffer,"[0]All tasks\t[1]My tasks\t[2]Add member\t[3]Add task");
  
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
    return -1;
  } else {
    return j;
  }
}

int proj_process( char* buffer, int proj_num, char* username ){
  if (strcmp(buffer, "0") == 0){
    char *proj_name; proj_name = (char *)malloc(50 * sizeof(char *));
    char num[4]; sprintf(num, "%d", proj_num);
    get_proj_name( proj_name, username, num );
    all_tasks( buffer, username, proj_name );
    return 0;
  } else if (strcmp(buffer, "1") == 0){
    my_tasks(buffer, username);
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
    /*CHECK IS MANAGER LOOP*/
    char proj[100];
    char num[4]; sprintf(num, "%d", proj_num);
    get_proj_name(proj, username, num);
    if (is_manager(username, proj)){

      sprintf(buffer, "What task are you assigning?:");
      return 2;
    } else { //not a manager
      sprintf(buffer, "You are not authorized to use this command.\n");
      char num[500]; sprintf(num, "%d", proj_num);
      //pass proj_number to view_proj, we're goin back to viewing the proj!
      view_proj(num, username);
      strcat(buffer, num);
      return -1;
    }
  
  }

  return -1;
}

/*get_manager(manager, project name)
populate manager with the manager of the project*/
void get_manager( char* manager, char* proj_name ){



}

bool is_manager( char* username, char* proj_name ) {
  char path[100];
  sprintf(path, "users/%s/%s/members.csv", username, proj_name);  
  FILE * members = fopen(path, "r");  

  if (errno == -1){
    printf("[%s] is NOT the manager of project [%s]\n", username, proj_name);
    return false;
  }else{
    printf("[%s] is the manager of project [%s]\n", username, proj_name);
    return true;
  }
  /*char buf[20];  
  fgets(buf, 20, members); //fgets reads until first newline
  if (strcmp(buf, username) == 0){
    fclose(members);
    return true;
  }
  fclose(members);
  return false;*/
}


int task_view( char* buffer, int TASK, char* username){
  
  sprintf(buffer, "Mark task[%d]'s progress:\n\t[0]Not yet started\t[1]In progess\t[2]Complete", TASK);
  return -1;
}

int task_process( char*buffer, int TASK, char* username ){
  if (strcmp(buffer, "0") == 0){
    //MARK NOT YET STARTED;
    sprintf(buffer, "Marked.");
  }
    
  return 0;
}

void all_tasks( char* buffer, char* username, char* proj_name ) {

  char *path; path = (char *)malloc(50 * sizeof(char *));
  char *task; task = (char *)malloc(50 * sizeof(char *));

  if( is_manager(username, proj_name) ) {

    sprintf(path, "users/%s/tasks.csv", proj_name);
    printf("path: %s\n", path);
    char arr[100][4][1024];
    int rows=parse_csv(path, arr); int r=0;

    for(r=0; r<rows; r++) {
      sprintf(task, "%s - [%s] %s - %s", arr[r][0], arr[r][1], arr[r][2], arr[r][3]);
      strcat(buffer, task);
    }
    
  } else {
    strcat(buffer, "You are not authorized to use this command.");
  }

}

void my_tasks( char* buffer, char* username ) {
  int i = 1;
  char path[100];
  sprintf(path, "users/%s/pii.csv", username);
  sprintf(buffer, "%s's tasks:\n", username);
  
  char *task;
  task = (char *)malloc(50 * sizeof(char *));

  char arr[100][4][1024];
  int cols=1, rows=parse_csv(path, arr); int r=0, c=0;
  
  for(r=0; r<rows; r++) {
    for(c=0; c<cols; c++) {

      sprintf(path, "users/%s/tasks.csv", arr[r][c]);
      char arr2[100][4][1024];
      int rows2 = parse_csv(path, arr2); int r2=0;
      
      for(r2=0; r2<rows2; r2++) {
	if( strcmp(arr2[r2][0], username) == 0 ) {
	  sprintf(task, "\t[%d] [%s] [%s] %s\n", i, arr[r][c], arr2[r2][2], arr2[r2][1]);
	  strcat(buffer, task);
	  i++;
	}
      }
    }
  }
  strcat(buffer, "Enter the task number to edit.");
}

void add_task( char* buffer, char* proj_name, char* username, char *task, char *deadline ) {

  char path[100];
  sprintf(path, "users/%s/%s/tasks.csv", username, proj_name);
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
  sprintf(path, "users/%s/%s/tasks.csv", username, proj_name);
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


/* NOTES ON UPDATE_STATUS FOR XINHUI RE: SERVER SIDE:
   - if update_status is being called from a place that uses proj_num (I was sort unclear on when that happens), then use the second, currently commented out, sprintf line, otherwise keep the first one (username is required as a param regardless)
   - make it so that the user only has 2 choices for updating status: 
     [0] In progress
     [1] Complete
   - and pass that INT to this function, which error handles the rest
   - also prompt the user for the next thing at the end of this fxn where I indicated
*/

void update_status( char* buffer, char *username, char *proj_name, char *task, int new ) {

  char path[100];
  sprintf(path, "users/%s/%s/tasks.csv", username, proj_name);
  //sprintf(path, "users/%s/tasks.csv", proj_name);

  char arr[100][4][1024];
  int rows = parse_csv(path, arr); int r=0;
  char *curr; curr = (char *)malloc(50 * sizeof(char *));
  char *msg; msg = (char *)malloc(50 * sizeof(char *));
  int edit = 0;
  
  for(r=0; r<rows; r++) {
    if( strcmp(arr[r][1], task) == 0 ) {
      sprintf(curr, arr[r][3]);
      break;
    }
  }
  
  if( strcmp(curr, "Not yet started") == 0 ) {
    if( new == 0 )
      edit = edit_status(arr, username, task, "In progress", rows, 4);
    else if( new == 1 )
      edit = edit_status(arr, username, task, "Complete", rows, 4);
  } else if ( strcmp(curr, "In progress") == 0 ) {
    if( new == 1 )
      edit = edit_status(arr, username, task, "Complete", rows, 4);
  } else {
    ;
  }

  if( edit )
    sprintf(msg, "[%s] is [%s]\n", arr[r][1], arr[r][3]);
  else
    sprintf(msg, "Whoops! Something went wrong.\n");

  strcat(buffer, msg);
  //@XINHUI: also strcat to buffer whatever comes after edit status in ur control flow logicg
  
}

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

