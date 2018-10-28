// Name: Sagar Thakur
// Roll no: 20172103

#include <file_ops.h>
#include <init.h>
#include <read_keypress.h>
#include <render.h>

int main (int argc, char *argv[]) {
  if (argc >= 2) {
    file_open(argv[1]);
  } else {
    printf("Please enter file name in the first argument\n");
    exit(1);
  }
  init_termios();
  get_window_size();
  file_read();

  while (1) {
    render_screen();
  }

  reset_termios();
  return 0;
}
