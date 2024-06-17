#ifndef S21_GREP_H
#define S21_GREP_H
#define STR_SIZE 2048

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// напиши что делает каждый флаг
typedef struct {
  int i;
  int e;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char pattern[STR_SIZE];
  int p_len;
} flags_variants;

void parse_file(regex_t regx, char *file_name,
                flags_variants grep_flags_variants, char **argv,
                int files_counter, int file_index, int *matched_lines_counter,
                int *is_appropiate);
int parse_stdin(int argc, char **argv, flags_variants *grep_flags_variants);
void print_matches(regex_t regx, char *str);
void add_pattern(flags_variants *g_flags, char *pattern);
void add_patterns_from_file(flags_variants *g_flags, char *file_name);

void print_str(char *str, int str_len);
#endif