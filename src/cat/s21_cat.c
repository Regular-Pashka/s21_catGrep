#include "s21_cat.h"

int main(int argc, char **argv) {
  flags_variants cat_flags_variants = {0};
  if (!parse_stdin(argc, argv, &cat_flags_variants)) {
    for (int i = optind; i < argc; i++) {
      file_print(argv[i], cat_flags_variants);
    }
  }
  return 0;
}

void file_print(char *file_name, flags_variants cat_flags_variants) {
  FILE *file;
  if (strcmp("-", file_name) == 0)
    file = stdin;
  else
    file = fopen(file_name, "r");
  if (file != NULL) {
    int prev_symb = '\n';
    int empty_l_printed = 0;
    int line_counter = 0;
    int symb = fgetc(file);  //возвращает номер ascii
    while (symb != EOF) {
      symb_print(symb, &prev_symb, cat_flags_variants, &line_counter,
                 &empty_l_printed);
      symb = fgetc(file);
    }
    fclose(file);
  } else {
    printf("No such file or directory");
  }
}

void symb_print(int symb, int *prev_symb,
                flags_variants /***/ cat_flags_variants, int *line_counter,
                int *empty_l_printed) {
  if (!(cat_flags_variants.s && *prev_symb == '\n' && symb == '\n' &&
        *empty_l_printed)) {
    if (symb == '\n' && *prev_symb == '\n') {
      *empty_l_printed = 1;
    } else {
      *empty_l_printed = 0;
    }
    if (((cat_flags_variants.b == 0 && cat_flags_variants.n) ||
         (cat_flags_variants.b && symb != '\n')) &&
        *prev_symb == '\n') {
      printf("%6d\t", ++(*line_counter));
    } else if (cat_flags_variants.b && symb == '\n' && *prev_symb == '\n' &&
               cat_flags_variants.e) {
      printf("      \t");  //если пустые строки то выводим шесть пробелов и
                           //табуляцию
    }
    if (cat_flags_variants.e && symb == '\n') printf("$");
    if (cat_flags_variants.t && symb == '\t') t_and_v_print(&symb);
    if (cat_flags_variants.v && ((symb <= 31 && symb >= 0) || symb == 127) &&
        symb != '\t' && symb != '\n')
      t_and_v_print(&symb);
    fputc(symb, stdout);
  }
  *prev_symb = symb;  //пропуск символа
}

void t_and_v_print(int *symb) {
  if (*symb == 127) {
    *symb = 63;
  } else {
    *symb += 64;
  }
  printf("^");
}

int parse_stdin(int argc, char **argv, flags_variants *cat_flags_variants) {
  int option;
  int no_V = 0;
  int flag_error = 0;
  static struct option long_options[] = {{"squeeze-blank", 0, 0, 's'},
                                         {"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {0, 0, 0, 0}};
  while ((option = getopt_long(argc, argv, "+benstvTE", long_options, NULL)) !=
             -1 &&
         !flag_error) {
    switch (option) {
      case 'b':
        cat_flags_variants->b = 1;
        break;
      case 'e':
        cat_flags_variants->v = 1;
        cat_flags_variants->e = 1;
        break;
      case 'E':
        cat_flags_variants->e = 1;
        no_V = 1;
        break;
      case 'T':
        cat_flags_variants->t = 1;
        no_V = 1;
        break;
      case 'n':
        cat_flags_variants->n = 1;
        break;
      case 's':
        cat_flags_variants->s = 1;
        break;
      case 't':
        cat_flags_variants->v = 1;
        cat_flags_variants->t = 1;
        break;
      case 'v':
        cat_flags_variants->v = 1;
        break;
      default:
        flag_error = 1;
        printf("s21_cat: illegal option");

        break;
    }
  }
  if (cat_flags_variants->b) cat_flags_variants->n = 0;
  if (no_V) cat_flags_variants->v = 0;
  return flag_error;
}
