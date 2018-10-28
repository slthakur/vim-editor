#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cursor_x, cursor_y;
int last_line_at_top;
char command[100];
pid_t pid;
char *args[3];
int previous_x, previous_y;

void render_screen();

#endif
