#include "server.h"
#include <dirent.h>
#include <sys/stat.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );


int main() {
  int sd, connection;
  sd = server_setup();
  while (1) {
    connection = server_connect( sd );
    int f = fork();
    if ( f == 0 ) {
      close(sd);
      sub_server( connection );
      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {
  int pid;
  bool USER = false;
  int HOME;
  bool INVALID = false;
  char buffer[MESSAGE_BUFFER_SIZE];
  char home[MESSAGE_BUFFER_SIZE];
  strcpy(home, "New project[0]\tMy projects[1]");
  pid = getpid();
  
  /* RETRIEVE USERNAME */
  char username[MESSAGE_BUFFER_SIZE];
  read( sd, username, sizeof(username) );
  printf("[SERVER %d] new login: %s\n", pid, username);

  if ( user_exists(username) ){
    USER = true;
  } else {
    create_user(username);
  }
    
  /* OPEN HOME */
  HOME = 999;
  strcpy(buffer, home);
  write(sd, buffer, sizeof(buffer));
  
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", pid, buffer );

    if (HOME == 999) {
      HOME = home_process(buffer);      

    } else if (HOME == 0) {      
      char new_proj_name[MESSAGE_BUFFER_SIZE];
      strcpy(new_proj_name, buffer);
      if (USER) {
	new_proj(new_proj_name, username);
      }
      printf("[SERVER %d] new project [%s] created by [%s]\n",pid, new_proj_name, username);
      sprintf(buffer, "Project created.\n%s", home);
      HOME = 999;
      
    } else if ( strcmp(buffer, "home") == 0){
      strcpy(buffer, home);
      HOME = 999;
    } else if ( strcmp(buffer, "exit") == 0){
      exit(0);
    }

    write(sd, buffer, sizeof(buffer));
  }
}

int home_process( char* buffer ){
  if ( strcmp(buffer, "0") == 0 ){
    strcpy(buffer, "Enter title of new project:");
    return 0;
  } else if ( strcmp(buffer, "1") == 0) {
    //my_proj();
    strcpy(buffer, "[placeholder] List of projects");
    return 1;
  } else {
    strcpy(buffer, "Invalid command.\nNew project[0]\tMy projects[1]");
    return 999;
  }
}

void parse_exec(char* cmd, char** argv){
  parse(cmd, argv);
  execute(argv);
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
  
  printf("USER DIRECTORIES:\n");

  while ( (de = readdir(d)) ){
    printf("\t%s\n",de->d_name);
    if (strcmp(de->d_name, username) == 0) {
      return true;
    }
  }
  /*  printf("FILES:\n");
  while (de = readdir(d)){
    printf("\t%s\n",de->d_name);
    }*/
  closedir(d);
  return false;
}

void new_proj( char* new_proj_name, char* username ){
  char cmd[1000];
  char* argv[10];
  char path[100];

  sprintf(path, "projects/%s/%s", username, new_proj_name);
  int err = mkdir(path, 0700);
  if (err)
    printf("new_proj: mkdir failed.\n");

  
  
  sprintf(path, "projects/%s/%s/members.csv", username, new_proj_name);
  
  int members = open(path, O_CREAT|O_RDWR, 0777);
  printf("opened members: %d, path: %s\n", members, path);
  write(members, username, sizeof(username));
  write(members, "\n", sizeof(char));
  close(members);

  sprintf(path, "projects/%s/%s/tasks.csv", username, new_proj_name);
  int tasks = open(path, O_CREAT|O_RDWR, 0777);
  printf("opened tasks\n");
  close(tasks);
  
}

/* borrowed from http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html */
void  parse(char *line, char **argv){
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

