#include "func.h"
#define MAX_ABC 256
#define ASCII_SIZE 255
#include <stdbool.h>

int main(int argc, char **argv) {
  char colors[][5] = {"0;30", /* Black */ "1;30",  /* Dark Gray */
                      "0;31", /* Red */ "1;31",    /* Bold Red */
                      "0;32", /* Green */ "1;32",  /* Bold Green */
                      "0;33", /* Yellow */ "1;33", /* Bold Yellow */
                      "0;34", /* Blue */ "1;34",   /* Bold Blue */
                      "0;35", /* Purple */ "1;35", /* Bold Purple */
                      "0;36", /* Cyan */ "1;36" /*Bold Cyan */};
  char facecontrol[MAX_ABC];
  size_t i, j;
  bool recursion;

  if ((strcmp(argv[1], "-r") == 0) && argc == 4) {
    printf("Поиск в подкаталогах '%s%sm%s%s0m' в %s%sm%s%s0m\n", CSI,
           colors[2], argv[2], CSI, CSI, colors[10], argv[3], CSI);
    recursion = true;
  } else {
    if ((argc == 3) && ((strcmp(argv[1], "-r") != 0))) {
      printf("Поиск '%s%sm%s%s0m' в %s%sm%s%s0m\n", CSI,
             colors[2], argv[1], CSI, CSI, colors[10], argv[2], CSI);
      recursion = false;
    } else {
      if (argc == 3) {
        printf("Вы указали не все данные\n");
        return -1;
      } else {
        printf("Неправильный запрос\n");
        return -2;
      }
    }
  }
  size_t temp = strlen(argv[2 + recursion]);
  if (temp > MAX_ABC) {
    printf("Имя деректории превышает все разумные пределы\n");
    return -3;
  }
  temp = strlen(argv[1 + recursion]);
  if (temp > MAX_ABC) {
    printf("Имя шаблона превышает все разумные пределы\n");
    return -4;
  }
  temp = strlen(argv[1 + recursion]);
  if (temp < 1) {
    printf("Пустой шаблон поиска\n");
    return -5;
  }
  char *dir1 = (char *)calloc(sizeof(char), MAX_ABC);
  if (!dir1)
    return -7;
  char *sample = (char *)calloc(sizeof(char), MAX_ABC);
  if (!sample)
    return -7;

  for (i = 0; i < strlen(argv[2 + recursion]); i++) {
    dir1[i] = (argv[2 + recursion])[i];
  }
  for (i = 0; i < strlen(argv[1 + recursion]); i++) {
    sample[i] = (argv[1 + recursion])[i];
  }

  size_t len_sample = strlen(sample);
  int match_counter = 0;
  int *counter;
  counter = &match_counter;
  int transition_cells[MAX_ABC][MAX_ABC];
  DIR *dir;
  if ((dir = opendir(dir1)) == NULL) {
    printf("Нет такой директории\n");
    closedir(dir);
    return -6;
  }

  memset(transition_cells, 0, sizeof(transition_cells));
  memset(facecontrol, 0, sizeof(facecontrol));

  for (j = 0; j < len_sample; j++) {
    char symbol = sample[j];
    facecontrol[(int)symbol] = 1;
  }

  table_logic(sample, transition_cells, facecontrol);
  print_table(transition_cells, len_sample, facecontrol);
  if (recursion == true) {
    recursion_search(dir1, transition_cells, len_sample, counter);
  } else {
    no_recursion_search(dir1, transition_cells, len_sample, counter);
  }
  if (match_counter == 0) {
    printf("Нет совпадений для '%s%sm%s%s0m'\n", CSI, colors[10], sample,
           CSI);
  } else
    printf("%s%sm%d%s0m найденных совпадений для '%s%sm%s%s0m'\n", CSI, colors[10],
           match_counter, CSI, CSI, colors[10], sample, CSI);
  closedir(dir);
  free(sample);
  free(dir1);
  return 0;
}

