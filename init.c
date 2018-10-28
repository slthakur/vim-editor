// Name: Sagar Thakur
// Roll no: 20172103

#include <error_handler.h>
#include <init.h>
#include <render.h>

#include <sys/ioctl.h>

struct termios initial_settings, new_settings;

void init_termios() {
  saved = 1;
  cursor_x = 1;
  cursor_y = 1;
  input = fopen("/dev/tty", "r");
  output = fopen("/dev/tty", "w");
  if(!input || !output)
    handle_error("I/O error");

  tcgetattr(0, &initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 1;
  tcsetattr(0, TCSANOW, &new_settings);
  current_mode = NORMAL;
  return;
}

// Reset vmin to 0 when read should return immidiately.
// Required in case where escape sequence need to be read.
void reset_vmin() {
  tcgetattr(0, &new_settings);
  new_settings.c_cc[VMIN] = 0;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void set_vmin() {
  tcgetattr(0, &new_settings);
  new_settings.c_cc[VMIN] = 1;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void reset_canonical() {
  tcgetattr(0, &new_settings);
  new_settings.c_lflag &= ~ICANON;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void set_canonical() {
  tcgetattr(0, &new_settings);
  new_settings.c_lflag &= ICANON;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void reset_echo() {
  tcgetattr(0, &new_settings);
  new_settings.c_lflag &= ~ECHO;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void set_echo() {
  tcgetattr(0, &new_settings);
  new_settings.c_lflag &= ECHO;
  tcsetattr(0, TCSANOW, &new_settings);
  return;
}

void reset_termios() {
  write(fileno(output), "\x1b[2J", 4);
  tcsetattr(0, TCSANOW, &initial_settings);
  return;
}

void get_window_size() {
  struct winsize ws;
  if (ioctl(fileno(output), TIOCGWINSZ, &ws) == -1)
    handle_error("Could not read screen size");
  columns = ws.ws_col;
  rows = ws.ws_row;
}
