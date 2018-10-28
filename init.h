// Name: Sagar Thakur
// Roll no: 20172103

#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <termios.h>

enum Modes {
  NORMAL,
  INSERT,
  COMMAND,
};

int columns, rows;
int current_mode;
int saved;

FILE *input, *output;

void init_termios();
void reset_vmin();
void set_vmin();
void set_canonical();
void set_echo();
void reset_canonical();
void reset_termios();
void reset_echo();
void get_window_size();

#endif
