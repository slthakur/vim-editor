// Name: Sagar Thakur
// Roll no: 20172103

#include <cursor_movement.h>
#include <error_handler.h>
#include <file_ops.h>
#include <init.h>
#include <insert.h>
#include <read_keypress.h>
#include <render.h>

#include <sys/types.h>
#include <sys/wait.h>

void read_keypress() {
  char key;

  if (read(fileno(input), &key, 1) == -1)
    handle_error("Could not read key!!");

  if (replace_mode == 1) {
    replace_key(key);
    replace_mode = 0;
    return;
  }

  if (go_to_top == 1) {
    if ((int)key == 103)
      cursor_to_first_line();
    go_to_top = 0;
    return;
  }
 
  switch ((int)key) {
    case 32 ... 57:
    case 59 ... 70:
    case 72 ... 102:
    case 109 ... 113:
    case 115 ... 126:
      if (current_mode == INSERT)
        insert_key(key);
      break;
    case 103: // for g keypress
      if (current_mode == INSERT)
        insert_key(key);
      else if (current_mode == NORMAL)
        go_to_top = 1; // In normal mode if we read one more 'g' the go to top
      break;
    case 71: // for G keypress
      if (current_mode == INSERT)
        insert_key(key);
      else if (current_mode == NORMAL)
        cursor_to_last_line();
      break;
    case 104:
      if (current_mode == INSERT) {
        insert_key(key);
        break;
      }
    CURSOR_LEFT:
      cursor_x--;
      break;
    case 106:
      if (current_mode == INSERT) {
        insert_key(key);
        break;
      }
    CURSOR_UP:
      if (current_line->prev != NULL)
        current_line = current_line->prev;
      cursor_y--;
      break;
    case 107:
      if (current_mode == INSERT) {
        insert_key(key);
        break;
      }
    CURSOR_DOWN:
      if (current_line->next != NULL)
        current_line = current_line->next;
      cursor_y++;
      break;
    case 108:
      if(current_mode == INSERT) {
        insert_key(key);
        break;
      }
    CURSOR_RIGHT:
      cursor_x++;
      break;
    case 10:
      if (current_mode == INSERT) {
        insert_key(key);
        goto CURSOR_DOWN;
      } else if (current_mode == NORMAL) {
        goto CURSOR_DOWN;
      }
      break;
    case 27:
      // Got escape seq, now reading '['
      // Resetting vmin is required to distinguish between read of arrow key
      // esc seq and the read of the key to be replaced.
      reset_vmin();
      if (read(fileno(input), &key, 1) == -1)
        handle_error("Could not read key!!");
      if ((int)key != 91) { // 91 for '['
        if (current_mode == INSERT) { // Only esc pressed, mode need to be changed
          exit_insert_mode();
          current_mode = NORMAL;
        } else if (current_mode == COMMAND) {
          current_mode = NORMAL;
        }
        // Set vmin back to 1 to make read wait on next keypress.
        set_vmin();
        break;
      }
      // Reading the terminal command
      if (read(fileno(input), &key, 1) == -1)
        handle_error("Could not read key!!");
      switch ((int) key) {
        case 51: // '3' delete key
          if (read(fileno(input), &key, 1) == -1)
            handle_error("Could not read key!!");
          set_vmin();
          if ((int)key != 126) // '~'
            break;
          delete_key();
          break;
        case 65:
          set_vmin();
          goto CURSOR_UP;
        case 66:
          set_vmin();
          goto CURSOR_DOWN;
        case 67:
          set_vmin();
          goto CURSOR_RIGHT;
        case 68:
          set_vmin();
          goto CURSOR_LEFT;
      }
      break;
    case 105: // insert mode 'i'
      if(current_mode == NORMAL) {
        init_insert_mode();
        current_mode = INSERT;
      } else {
        insert_key(key);
      }
      break;
    case 58: // command line mode ':'
      if(current_mode == NORMAL) { // Switch to command mode
        current_mode = COMMAND;
      } else if (current_mode == INSERT) {
        insert_key(key);
      }
      break;
    case 114:
      if(current_mode == INSERT) {
        insert_key(key);
      } else if (current_mode == NORMAL) { // r for replace key
        replace_mode = 1;
      }
      break;
    case 127: // backspace
      if (current_mode == INSERT)
        backspace_key();
      break;
  }
  return;
}
