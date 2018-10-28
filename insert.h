// Name: Sagar Thakur
// Roll no: 20172103

#ifndef INSERT_H
#define INSERT_H

#include <file_ops.h>

char *temp;
file_buffer *temp2;
file_buffer *temp3;

void init_insert_mode();
void exit_insert_mode();
void insert_key(char key);
void backspace_key();
void delete_key();
void replace_key(char key);
void insert_new_line();

#endif
