#include "functions.h"
#define ASCII_SIZE 255

char colors[][5] = {
    "0;30", /* Blacstate*/ "1;30",                 /* DarstateGray */
    "0;31", /* Red */ "1;31",                      /* Bold Red */
    "0;32", /* Green */ "1;32",                    /* Bold Green */
    "0;33", /* Yellow */ "1;33",                   /* Bold Yellow */
    "0;34", /* Blue */ "1;34",                     /* Bold Blue */
    "0;35", /* Purple */ "1;35",                   /* Bold Purple */
    "0;36", /* Cyan */ "1;36"                      /*Bold Cyan */
};
//в двумерном массиве transition_cells[Q][V] - первая ячейка соответсвует текущему состоянию автомата, вторая - символу из источника. Их пересечение в талице дает результат следующего состояния автомата.

int matches(char *sourse, int transition_cells[256][256], const size_t len_sample) { //поиск совпадений с шаблоном (return 0/-1)
    size_t len_sourse = strlen(sourse);
    size_t current_state = 0; //Состояния аавтоматов по A<V,Q,q,F,o>
    size_t final_state = 0;   //Текущее и финальное состояние

    for (size_t i = 0; i <= len_sourse; i++) { //ТЕСТ ТЕСТ ТЕСТ <=  / <
        char symbol = sourse[i];
        final_state = transition_cells[current_state][(int)symbol]; //?
        current_state = transition_cells[current_state][(int)symbol];

        if (final_state == len_sample) {
            return(i - len_sample + 2); //поиск начинается с i - len_sample + 1
        }
    }

    return -1;
}

// рекурсивный и нерекурсивный поиск - зависит от ключа -r в inputTerm. (ТЗ)

void recursion_search(char *path, int transition_cells[256][256], size_t len_sample, int *match_counter) {
    
    char *dynamic_path = (char *)calloc(256, sizeof(char)); //dynamic_path - массив куда записывается путь для поиска в нижних каталогах (путем склееивания (или как сказал бы дезоморфиновый наркоман писавший ТЗ - конкатенации)) если таковые существуют.
    DIR *dir;
    struct dirent *stream;

    if ((dir = opendir(path)) != NULL) {
        while ((stream = readdir(dir)) != NULL) {
            char *file_name = (char *)calloc(256, sizeof(char)); //ЗАКРЫТЬ!
            strcpy(file_name, stream->d_name);
            int start_index = matches(file_name, transition_cells, len_sample);

            if (start_index >= 0) { // if matches function back non-negatice result;
                printf("%s %sm %s %s0m - ", CSI, colors[10], file_name, CSI);
                *match_counter += 1;
            } else {
                printf("%20s - \n", file_name);
            }

            //проверка типа данных
            if (stream->d_type == DT_REG) {
                printf("Файл\n");
            } else {
                if ((stream->d_type == DT_DIR) && ((strcmp(file_name, ".") || strcmp(file_name, "..")) != 0)) {
                    printf("Папка\n");
                    strcpy(dynamic_path, path);
                    dynamic_path = strcat(dynamic_path, "/"); dynamic_path = strcat(dynamic_path, file_name);
                    recursion_search(dynamic_path, transition_cells, len_sample, match_counter);
                } else {
                    printf("Неизвестный тип данных\n");
                }
            }

            free(file_name);
        }
        closedir(dir);
        free(dynamic_path);
    } else {
        closedir(dir);
        printf("Директория указана неверно, проверьте поданую строку\n");
        return;
    }
}

void no_recursion_search(char *path, int transition_cells[256][256], size_t len_sample, int *match_counter) {
    DIR *dir;
    struct dirent *stream;

    if ((dir = opendir(path)) != NULL) {
        while ((stream = readdir(dir)) != NULL) {
            char *file_name = (char *)calloc(256, sizeof(char)); //ЗАКРЫТЬ!
            strcpy(file_name, stream->d_name);
            int start_index = matches(file_name, transition_cells, len_sample);

            if (start_index >= 0) {
                printf("%s %sm %20s %s0m - ", CSI, colors[10], file_name, CSI);
                *match_counter += 1;
            } else {
                printf("%20s - ", file_name);
            }

            if (stream->d_type == DT_REG) {
                printf("Файл\n");
            } else {
                printf("Папка\n");
            }

            free(file_name);
        }
        closedir(dir);
    } else {
        closedir(dir);
        printf("Директория указана неверно, проверьте поданую строку\n");
        return;
    }
}

void print_table(const char* sample, int transition_cells[256][256], const char* facecontrol) {
    
    size_t len_sample = strlen(sample);
    size_t i, j; // i - проходит по длине образца, j - проходит по алфавиту V in func.
    for(i = 0; i <= len_sample; i++) {
        for(j = 0; j <= ASCII_SIZE; j++) {
            if(facecontrol[j]) {
                size_t current_state = 0;
                if(len_sample >= 1 + i)
                    current_state = i + 1;
                else
                    current_state = len_sample;
                while(current_state > 0) {
                    current_state = current_state - 1;
                    char symbol = sample[current_state];
                    if((current_state > 0) && ((size_t)symbol == j)) {
                        size_t suffix = current_state; size_t prefix = i;
                        while((suffix > 0) && (sample[suffix] == sample[prefix])) {
                            suffix = suffix - 1;
                            prefix = prefix - 1;
                        }
                        if(suffix == 0) {
                            current_state++;
                            break;
                        }
                    }
                }
                transition_cells[i][j] = current_state; //да знаю я
            }
        }
    }
    
    return;
}

/*
 ________
|__|__|__|
|__|__|__|
|__|__|__|
 */


