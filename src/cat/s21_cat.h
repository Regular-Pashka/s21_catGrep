#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flags_variants;

int parse_stdin(int argc, char **argv, flags_variants *cat_flags_variants);
void file_print(char *file_name, flags_variants cat_flags_variants);
void symb_print(int symb, int *prev_symb,
                flags_variants /***/ cat_flags_variants, int *line_counter,
                int *empty_l_printed);
void t_and_v_print(int *symb);

#endif