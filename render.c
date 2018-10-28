#include <init.h>
#include <read_keypress.h>
#include <render.h>
#include <error_handler.h>
#include <file_ops.h>

#include <sys/types.h>
#include <sys/wait.h>

void render_screen() {
  char *cursor_esc_seq = (char *)malloc(sizeof(char) * 6);

  get_window_size();

  // Clr screen
  write(fileno(output), "\x1b[2J", 4);
 
  // Reposition cursor 
  write(fileno(output), "\x1b[H", 4);

  // Render file text from text buffer
  int line_number = 1, line_length;

  file_buffer *fb = active_first_line;
  while (fb != NULL) {
    write(fileno(output), fb->line, fb->line_length);
    // Capture the line length where the cursor is currently to restrict 
    // cursor_x to the line lenght
    if(line_number == cursor_y)
      line_length = fb->line_length;
    if ((rows - 1) == line_number)
      break;
    write(fileno(output), "\n", 2);
    line_number++;
    fb = fb->next;
  }

  // Restrict cursor_x to the test printed on screen
  if(cursor_x > line_length && current_mode == NORMAL)
    cursor_x = line_length;
  else if((cursor_x > (line_length + 1)) && current_mode == INSERT) {
    cursor_x = line_length + 1;
  }

  if(cursor_y > number_of_lines)
    cursor_y = number_of_lines;

  if(cursor_x > columns)
    cursor_x = columns;

  if(cursor_x < 1)
    cursor_x = 1;

  // The last line that can be displayed on top of screen
  last_line_at_top = number_of_lines - (rows - 1) + 1;

  for(int i = line_number; i != rows; i++)
    write(fileno(output), "\n", 2);
  if (current_mode == NORMAL) {
    write(fileno(output), "<NORMAL_MODE>", 13);
  } else if (current_mode == INSERT) {
    write(fileno(output), "<INSERT_MODE>", 13);
  } else if (current_mode == COMMAND) {
    char esc_seq[6];
    previous_x = cursor_x;
    previous_y = cursor_y;
    cursor_x = 2;
    cursor_y = rows;
    set_canonical();
    write(fileno(output), ":", 1);
    sprintf(esc_seq, "\x1b[%d;%dH", cursor_y, cursor_x);
    write(fileno(output), esc_seq, strlen(esc_seq));
    scanf("%s", command);
    if (strcmp(command, "wq") == 0) {
      file_write();
      reset_termios();
      exit(0);
    } else if (command[0] == 'w') {
      file_write();
      saved = 1;
      cursor_x = previous_x;
      cursor_y = previous_y;
      current_mode = NORMAL;
    } else if (strcmp(command, "q!") == 0) {
      reset_termios();
      exit(0);
    } else if (command[0] == 'q') {
      if (!saved) {
        write(fileno(output), "File not saved: Press q! to quit forcefully", 
              43);
        cursor_x = previous_x;
        cursor_y = previous_y;
        current_mode = NORMAL;
      } else {
        reset_termios();
        exit(0);
      }
    } else if (command[0] == '!') {
      args [0] = command + 1;
      args [1] = NULL;
      pid = fork(); 
      if (pid == 0) {
        if (execvp (args[0], args)) {
          handle_error("exec");
        }
        exit(0);
      } else {
        // Make the parent wait
        wait(0);
      }
    }
    reset_canonical();
    cursor_x = previous_x;
    cursor_y = previous_y;
    current_mode = NORMAL;
  }

  // Let y co-ordinate be less than equal to row - 1. -1 is for status bar
  if(cursor_y > rows - 1) {
    if (active_first_line->line_num != last_line_at_top)
      active_first_line = active_first_line->next;
    cursor_y = rows - 1;
  }

  if(cursor_y < 1) {
    if(active_first_line->prev != NULL)
      active_first_line = active_first_line->prev;
    cursor_y = 1;
  }

  // instead of (x,y)
  sprintf(cursor_esc_seq, "\x1b[%d;%dH", cursor_y, cursor_x);
  write(fileno(output), cursor_esc_seq, strlen(cursor_esc_seq));

  read_keypress();
  return;
}
