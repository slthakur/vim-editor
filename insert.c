// Name: Sagar Thakur
// Roll no: 20172103

#include <file_ops.h>
#include <init.h>
#include <insert.h>
#include <render.h>

#include <string.h>
#include <stdlib.h>

void init_insert_mode() {
  temp = (char *)malloc(sizeof(char) * columns);
  temp2 = (file_buffer *)malloc(sizeof(file_buffer));
  temp3 = (file_buffer *)malloc(sizeof(file_buffer));
}

void exit_insert_mode() {
  free(temp);
  free(temp3);
}

void insert_key (char key) {
  saved = 0;
  if ((int)key == 10) { // Enter
    insert_new_line(current_line->line + (cursor_x - 1));
    current_line->line[cursor_x - 1] = '\0';
    current_line->line_length = strlen(current_line->line);
    cursor_x = 1;
    return;
  }
  memcpy(temp, current_line->line, cursor_x - 1);
  temp[cursor_x - 1] = '\0';
  strcat(temp, &key);
  strcat(temp, current_line->line + (cursor_x - 1));
  strcpy(current_line->line, temp);
  current_line->line_length = strlen(current_line->line);
  cursor_x++;
  return;
}

void backspace_key() {
  saved = 0;
  if (cursor_x == 1) { // Need to delete current line and append to prev
    if (current_line->prev != NULL) {
      strcat(current_line->prev->line, current_line->line);
      cursor_x = current_line->prev->line_length + 1;
      current_line->prev->line_length = strlen(current_line->prev->line);
      cursor_y--;
      if (current_line->next != NULL) {
        current_line->prev->next = current_line->next;
        current_line->next->prev = current_line->prev;
      } else {
        current_line->prev->next = NULL;
      }
      temp2 = current_line;
      temp3 = current_line->next;
      current_line = temp2->prev;
      free(temp2);
      while (temp3 != NULL) {
        temp3->line_num--;
        temp3 = temp3->next;
      }
      number_of_lines--;
    }
    return;
  }
  strcpy(temp, current_line->line + (cursor_x - 1));
  current_line->line[cursor_x - 2] = '\0';
  strcat(current_line->line, temp);
  current_line->line_length = strlen(current_line->line);
  cursor_x--;
  return;
}

void delete_key() {
  saved = 0;
  // Cannot use memcpy here since mem will be overlapping
  current_line->line[cursor_x - 1] = '\0';
  strcat(current_line->line, current_line->line + cursor_x);
  current_line->line_length = strlen(current_line->line);
  return;
}

void replace_key (char key) {
  saved = 0;
  current_line->line[cursor_x - 1] = key;
  return;
}

void insert_new_line(char * new_line) {
  saved = 0;
  file_buffer *temp = (file_buffer *) malloc(sizeof(file_buffer));
  if (current_line->next != NULL) {
    temp->next = current_line->next;
    temp->prev = current_line;
    current_line->next->prev = temp;
    current_line->next = temp;
  } else {
    temp->prev = current_line;
    temp->next = NULL;
    current_line->next = temp;
  }
  temp->line = (char *) malloc(sizeof(char) * columns);
  strcpy(temp->line, new_line);
  temp->line_num = current_line->line_num + 1;
  temp->line_length = strlen(temp->line);
  temp = temp->next;
  while (temp != NULL) {
    temp->line_num++;
    temp = temp->next;
  }
  if (cursor_y == rows - 1)
    active_first_line = active_first_line->next;
  number_of_lines++;
}
