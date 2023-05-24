#include "functions.h"
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
    printf("\nПоиск в подкаталогах '%s%sm%s%s1m' в %s%sm%s%s0m\n", CSI,
           colors[10], argv[2], CSI, CSI, colors[10], argv[3], CSI);
    recursion = true;
  } else {
    if ((argc == 3) && ((strcmp(argv[1], "-r") != 0))) {
      printf("\nПоиск '%s%sm%s%s0m' в %s%sm%s%s0m\n", CSI,
             colors[10], argv[1], CSI, CSI, colors[10], argv[2], CSI);
      recursion = false;
    } else {
      if (argc == 3) {
        printf("\nВы указали не все данные\n");
        return -1;
      } else {
        printf("\nНеправильный запрос\n");
        return -2;
      }
    }
  }
  size_t temp = strlen(argv[2 + recursion]);
  if (temp > MAX_ABC) {
    printf("\nИмя деректории превышает все разумные пределы\n");
    return -3;
  }
  temp = strlen(argv[1 + recursion]);
  if (temp > MAX_ABC) {
    printf("\nИмя шаблона превышает все разумные пределы\n");
    return -4;
  }
  temp = strlen(argv[1 + recursion]);
  if (temp < 1) {
    printf("\nПустой шаблон поиска\n");
    return -5;
  }
short errors = 0;
  char *dir1 = (char *)calloc(sizeof(char), MAX_ABC);
  if (!dir1)
    errors++;
  char *sample = (char *)calloc(sizeof(char), MAX_ABC);
  if (!sample)
    errors++;
  if(errors > 0) {
        printf("Не удалось выделить память\n");
        return -7;
    }
      

  for (i = 0; i < strlen(argv[2 + recursion]); i++) {
    dir1[i] = (argv[2 + recursion])[i];                   //имя директории, если рекурсия включена, или имя файла/каталога, если рекурсия выключена
  }
  for (i = 0; i < strlen(argv[1 + recursion]); i++) {
    sample[i] = (argv[1 + recursion])[i];                 //содЭржит шаблон >.<
  }

  size_t len_sample = strlen(sample);
  int match_counter = 0;
  int *counter;
  counter = &match_counter;
  int transition_cells[MAX_ABC][MAX_ABC];
  DIR *dir;
  if ((dir = opendir(dir1)) == NULL) {
    printf("\nНет такой директории\n");
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
    printf("\nНет совпадений для '%s%sm%s%s0m'\n", CSI, colors[10], sample,
           CSI);
  } else
    printf("\n%s%sm%d%s0m найденных совпадений для '%s%sm%s%s0m'\n", CSI, colors[10],
           match_counter, CSI, CSI, colors[10], sample, CSI);
  closedir(dir);
  free(sample);
  free(dir1);
  return 0;
}

