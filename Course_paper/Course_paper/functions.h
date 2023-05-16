#pragma once
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define CSI "\x1B\x5B"

int matches(char *sourse, int transition_cells[256][256], const size_t len_sample);
void recursion_search(char *path, int transition_cells[256][256], size_t len_sample, int *match_counter);
void no_recursion_search(char *path, int transition_cells[256][256], size_t len_sample, int *match_counter);


