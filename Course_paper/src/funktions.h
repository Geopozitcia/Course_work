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
void print_table(int transition_cells[256][256], const size_t len_sample, const char *facecontrol);
void fill_line(const size_t n);
void table_logic(const char* sample, int transition_cells[256][256], const char* facecontrol);

