#pragma once
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define CSI "\x1B\x5B"
#define MAX_ABC 256
#define ASCII_SIZE 255

int matches(const char *sourse, int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample);
void recursion_search(char *path, int transition_cells[MAX_ABC][MAX_ABC],const size_t len_sample, int *match_counter);
void no_recursion_search(char *path, int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample, int *match_counter);
void table_logic(const char *sample, int transition_cells[MAX_ABC][MAX_ABC], const char *facecontrol);
void fill_line(const int n);
void print_table(int transition_cells[MAX_ABC][MAX_ABC], const size_t len_sample, const char *facecontrol); 
