#include "functions.h"
#define MAX_ABC 256
#define ASCII_SIZE 255
char colors[][5] = {"0;30", /* Blacstate*/ "1;30", /* DarstateGray */
                    "0;31", /* Red */ "1;31",      /* Bold Red */
                    "0;32", /* Green */ "1;32",    /* Bold Green */
                    "0;33", /* Yellow */ "1;33",   /* Bold Yellow */
                    "0;34", /* Blue */ "1;34",     /* Bold Blue */
                    "0;35", /* Purple */ "1;35",   /* Bold Purple */
                    "0;36", /* Cyan */ "1;36" /*Bold Cyan */};

// в двумерном массиве transition_cells[Q][V] - первая ячейка соответсвует текущему состоянию автомата,
// вторая - символу из источника. Их пересечение в талице дает результат следующего состояния автомата.

int matches(const char *sourse, int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample) // поиск по шаблону
{
  size_t len_sourse = strlen(sourse);
  size_t final_state = 0;   // финальные состояния F
  size_t current_state = 0; // Состояния аавтоматов по A<V,Q,q,F,o>
  size_t i;
  for (i = 0; i < len_sourse; i++)
  {
    char symbol = sourse[i];
    final_state = transition_cells[current_state][(int)symbol];
    current_state = transition_cells[current_state][(int)symbol];
    if (final_state == len_sample)
      return (i - len_sample + 2); // поиск начинается с i - len_sample + 1
  }
  return -1;
}

// рекурсивный и нерекурсивный поиск - зависит от ключа -r в inputTerm. (ТЗ)
void recursion_search(char *path, int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample, int *match_counter)
{
  char *dynamic_path = (char *)calloc(sizeof(char), MAX_ABC);
  // dynamic_path - массив куда записывается путь для поиска
  // в нижних каталогах (путем склееивания) если таковые существуют.
  DIR *dir;
  struct dirent *stream;
  if ((dir = opendir(path)) != NULL)
  {
    while ((stream = readdir(dir)) != NULL)
    {
      char *file_name = (char *)calloc(sizeof(char), MAX_ABC);
      strcpy(file_name, stream->d_name);
      int start_index = matches(file_name, transition_cells, len_sample);
      if (start_index >= 0)
      { // if matches function back non-negatice result;
        printf("%s%sm%20s%s0m - ", CSI, colors[10], file_name, CSI);
        *match_counter += 1;
      }
      else
        printf("%20s - ", file_name);
      if (stream->d_type == DT_REG)
      { // проверка типа данных
        printf("Файл\n");
      }
      else
      {
        if ((stream->d_type == DT_DIR) && (strcmp(file_name, ".") != 0) &&
            (strcmp(file_name, "..") != 0))
        {
          printf("Папка\n");
          strcpy(dynamic_path, path);
          dynamic_path = strcat(dynamic_path, "/");
          dynamic_path = strcat(dynamic_path, file_name);
          recursion_search(dynamic_path, transition_cells, len_sample, match_counter);
        }
        else
          printf("Верхний каталог\n");
      }
      free(file_name);
    }
    closedir(dir);
    free(dynamic_path);
  }
  else
  {
    closedir(dir);
    printf("Директория указана неверно, проверьте введеную строку\n");
    return;
  }
}

void no_recursion_search(char *path, int transition_cells[MAX_ABC][MAX_ABC],
                         const size_t len_sample, int *match_counter)
{
  DIR *dir;
  struct dirent *stream;
  if ((dir = opendir(path)) != NULL)
  {
    while ((stream = readdir(dir)) != NULL)
    {
      char *file_name = (char *)calloc(sizeof(char), MAX_ABC);
      strcpy(file_name, stream->d_name);
      int start_index = matches(file_name, transition_cells, len_sample);
      if (start_index >= 0)
      {
        printf("%s%sm%20s%s0m - ", CSI, colors[10], file_name, CSI);
        *match_counter += 1;
      }
      else
        printf("%20s - ", file_name);
      if (stream->d_type == DT_REG)
      {
        printf("Файл\n");
      }
      else
      {
        printf("Папка\n");
      }
      free(file_name);
    }
    closedir(dir);
  }
  else
  {
    closedir(dir);
    printf("Директория указана неверно, проверьте введеную строку\n");
    return;
  }
}

void table_logic(const char *sample, int transition_cells[MAX_ABC][MAX_ABC],
                 const char *facecontrol)
{
  size_t len_sample = strlen(sample);
  size_t i, j; // i - проходит по длине образца, j - проходит по алфавиту V in func.
  int state;
  for (i = 0; i <= len_sample; i++)
  {
    for (j = 1; j <= ASCII_SIZE; j++)
    {
      if (facecontrol[j])
      {
        state = 0;
        if (len_sample >= i + 1) // условие устанавливает начальное состояние автомата.
          state = i + 1;
        else
          state = len_sample; // что бы не выйти за пределы sample
        while (state > 0)
        {
          state = state - 1;
          char symbol = sample[state];
          if ((state >= 0) && ((size_t)symbol == j))
          {
            int suffix = state, prefix = i;
            while ((suffix > 0) && (sample[suffix] == sample[prefix]))
            {
              suffix = suffix - 1;
              prefix = prefix - 1;
            }

            if (suffix == 0)
            {
              state++;
              break;
            }
          }
        }
        transition_cells[i][j] = state;
      }
    }
  }
}

// Графическое отображение таблицы

/*
 ________
|__|__|__|
|__|__|__|
|__|__|__|
 */

void fill_line(const int n)
{
  for (int i = 0; i <= n; i++)
    printf("─");
}

void print_table(int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample,
                 const char *facecontrol)
{
  printf("\nТаблица переходов:\n");
  size_t quantity = len_sample;
  size_t i, j, digits = 0;
  while (quantity > 0)
    quantity = quantity / 10; // понижение разрядности для вывода количесва символов образца

  printf("г");
  printf("─");
  for (j = 0; j <= len_sample; j++)
  {
    printf("т");
    fill_line(digits + 1);
  }
  printf("‾|\n");
  printf("│ ");
  for (i = 0; i <= len_sample + digits; i++)
    printf("│ %ld", i); // вывод колличества символов в верхнюю строку
  printf("│\n");
  for (i = 1; i <= ASCII_SIZE; i++)
  {
    if (facecontrol[i])
    {
      printf("|-");
      for (j = 0; j <= len_sample; j++)
      {
        printf("+");
        fill_line(digits + 1);
      }
      printf("-|\n");
      char c = i;
      printf("│%c", c); // вывод символов в правый столбец
      for (j = 0; j <= len_sample; j++)
        printf("│ %d", transition_cells[j][i]);
      printf("│\n");
    }
  }
  printf("L");
  printf("─");
  for (j = 0; j <= len_sample; j++)
  {
    printf("l");
    fill_line(digits + 1);
  }
  printf("j\n");
  printf("\n");
}
