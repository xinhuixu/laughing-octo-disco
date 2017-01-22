#ifndef DEFINED1
#define DEFINED1

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
#include <dirent.h>
#include <errno.h>

typedef int bool;
#define true 1
#define false 0

int home_process( char*, char* );
void new_proj( char*, char* );
void parse( char*, char** );
void execute( char** );
bool user_exists( char* );
bool proj_exists( char*, char* );
void create_user( char* );
void list_projs( char* , char*);

#endif
