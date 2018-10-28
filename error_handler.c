// Name: Sagar Thakur
// Roll no: 20172103

#include <error.h>
#include <stdio.h>
#include <stdlib.h>

#include <init.h>

void handle_error(char *msg) {
  reset_termios();
  perror(msg);
  exit(1);
}

