// Name: Sagar Thakur
// Roll no: 20172103

#ifndef FILE_OP_H
#define FILE_OP_H

#include <stdio.h>

FILE * file_input;
FILE * file_output;

char *filename_w;

typedef struct file_buffer {
  char *line;
  int line_length;
  int line_num;
  struct file_buffer *next;
  struct file_buffer *prev;
} file_buffer;

file_buffer *fb;
file_buffer *first_line;
file_buffer *active_first_line;
file_buffer *current_line;

int number_of_lines;

void file_open(char *filename);
void file_read();
void file_write();

#endif
