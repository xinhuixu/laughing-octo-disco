#ifndef DEFINED1
#define DEFINED1

#include "server.h"

typedef int bool;
#define true 1
#define false 0

int home_process( char* buffer, char* username );
void create_user( char* username );
bool user_exists( char* username );
bool proj_exists( char* username, char* proj_name );
bool list_projs( char* buffer, char* username );
void build_array( char array[100][100], char* username );
void get_proj_name( char* proj_name, char* username, char* proj_num );
void new_proj( char* new_proj_name, char* username );
int count_projs( char* username );
int view_proj( char* buffer, char* username);
int proj_process( char* buffer, int proj_num, char* username );
void get_manager( char* manager, char* proj_name );
bool is_manager( char* username, char* proj_name );
int task_view( char* buffer, int TASK, char* username);
int task_process( char*buffer, int TASK, char* username );
void all_tasks( char* buffer, char* username, char* proj_name );
void my_tasks( char* buffer, char* username, char* proj_name );
void add_task( char* buffer, char* proj_name, char* username, char *task, char *deadline );
void remove_task( char* proj_name, char* username, char *task, char* buffer );
void add_member( char* buffer, char* username, char* proj_name, char* new_member );
void update_status( char* buffer, char *username, char *proj_name, char *task, int new );
void parse_exec(char* cmd, char** argv);
void parse(char *line, char **argv);
void execute(char **argv);

#endif
