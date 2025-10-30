#ifndef PERSISTENCY
#define PERSISTENCY

#include <stdbool.h>

bool make_persistent(char *executable_name);
bool is_first_exec(char *executable_name);
bool priv_esc();
void copy_to_persistent_path(char *current_path, char *dest_path);

#endif