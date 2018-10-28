// Name: Sagar Thakur
// Roll no: 20172103

#include <file_ops.h>
#include <init.h>
#include <render.h>

void cursor_to_first_line() {
  current_line = first_line;
  active_first_line = first_line;
  cursor_y = 1;
  cursor_x = 1;
  return;
}

void cursor_to_last_line() {
  if (last_line_at_top <= 0) { // text file fits in the sceen length wise
    active_first_line = first_line;
    while (current_line->line_num != number_of_lines)
      current_line = current_line->next;
    cursor_y = number_of_lines;
    cursor_x = 1;
    return;
  }
  while (active_first_line->line_num != last_line_at_top)
    active_first_line = active_first_line->next;
  current_line = active_first_line;
  while (current_line->line_num != number_of_lines)
    current_line = current_line->next;
  cursor_y = rows - 1;
  cursor_x = 1;
  return;
}
