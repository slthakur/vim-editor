// Name: Sagar Thakur
// Roll no: 20172103

#include <error_handler.h>
#include <file_ops.h>
#include <init.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void file_open(char *filename) {
  filename_w = (char *)malloc(sizeof(char) * 100);
  strcpy(filename_w, filename);
  file_input = fopen(filename ,"r");
  return;
}

void file_read() {
  file_buffer *temp;
  first_line = (file_buffer *)malloc(sizeof(file_buffer));

  // Creating first line for a new file
  if (file_input == NULL) {
    first_line->line_length = 1;
    first_line->line_num = 1;
    number_of_lines = 1;
    first_line->prev = NULL;
    first_line->line = (char *)malloc(sizeof(char) * columns);
    strcpy(first_line->line, "");
    first_line->next = NULL;
  }
  if (file_input != NULL) {
    fb = first_line;
    while (!feof(file_input)) {
      fb->line = (char *)malloc(sizeof(char) * columns);
      fgets(fb->line, columns, file_input);
      fb->line_length = strlen(fb->line);
      fb->line_length--;
      fb->line[fb->line_length] = '\0';
      fb->line_num = number_of_lines + 1;
      if (feof(file_input))
        break;
      fb->next = (file_buffer *)malloc(sizeof(file_buffer));
      temp = fb;
      fb = fb->next;
      fb->prev = temp;
      number_of_lines++;
    }

    // Delete extra line added to list because of EOF
    fb->prev->next = NULL;
    free(fb);
    fb = NULL;
  }

  // Mark the first_line as active since it will be rendered on sceen
  active_first_line = first_line;
  current_line = first_line;

  return;
}

void file_write() {
  file_output = fopen(filename_w ,"w+");
  file_buffer *temp;
  temp = first_line;
  char *line = (char *)malloc(sizeof(char) * columns);
  if (temp->next == NULL) {
    strcpy(line, temp->line);
    fputs(line, file_output);
    fputs("\n", file_output);
  }
  while (temp != NULL) {
    strcpy(line, temp->line);
    fputs(line, file_output);
    fputs("\n", file_output);
    temp = temp->next;
  }

  fclose(file_output);
  return;
}
