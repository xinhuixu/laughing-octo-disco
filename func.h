#ifndef DEFINED1
#define DEFINED1

#include "server.h"

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
bool list_projs( char* , char*);
int view_proj( char*, char* );
int count_projs( char* );
bool is_manager( char*, char* );
void get_proj_name( char*, char*, char* );
int proj_process(char*, int, char* );

void all_tasks( char*, char*, char* );
void my_tasks( char*, char* );
void add_task( char*, char*, char*, char*, char* );
void remove_task( char*, char*, char*, char* );
void remove_member( char* );
void add_member( char* );
void update_status( char*, char*, char*, char* );
#endif
