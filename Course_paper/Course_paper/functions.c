#include "functions.h"

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

    for (size_t i = 0; i < len_sourse; i++) {
        char symbol = sourse[i];
        final_state = transition_cells[current_state][(int)symbol]; //?
        current_state = transition_cells[current_state][(int)symbol];
        
        if (final_state == len_sample)
            return(i - len_sample + 2); //поиск начинается с i - len_sample + 1
        
    }
    return -1;
}

// рекурсивный и нерекурсивный поиск - зависит от ключа -r в inputTerm. (ТЗ)

void recursion_search(char* path, int transition_cells[256][256], size_t len_sample, int* match_counter) {
    
    char* dynamic_path = (char*) calloc(256, sizeof(char)); //dynamic_path является направлением копирования при выполнении условий.
    DIR* dir;
    struct dirent *stream;
    if ((dir = opendir(path)) != NULL) {
        while((stream = readdir(dir)) != NULL) {
            char* file_name = (char*)calloc(256, sizeof(char)); //ЗАКРЫТЬ!
            strcpy(file_name, stream->d_name);
            int start_index = matches(file_name, transition_cells, len_sample);
            if(start_index >= 0) { // if matches function back non-negatice result;
                printf("%s  %sm %s% s0m - ", CSI, colors[10], fileName, CSI);
                *match_counter += 1;
                
            }
            
            
        }
    
    
}

