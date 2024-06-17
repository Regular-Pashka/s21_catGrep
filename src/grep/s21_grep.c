#include "s21_grep.h"
#define _GNU_SOURCE

int main(int argc, char **argv) {
  flags_variants g_flags = {0};
  if (!parse_stdin(argc, argv, &g_flags)) {
    regex_t regx;
    if (regcomp(&regx, g_flags.pattern,
                (g_flags.i ? REG_ICASE : 0) | REG_EXTENDED))
      perror("Error");
    int files_counter = 0;
    int matched_lines_counter;
    int is_appropiate;
    for (int i = optind; i < argc; i++) files_counter++;
    for (int i = optind; i < argc; i++) {
      is_appropiate = 0;
      matched_lines_counter = 0;
      parse_file(regx, argv[i], g_flags, argv, files_counter, i,
                 &matched_lines_counter, &is_appropiate);
      if (g_flags.c) {
        if (files_counter > 1 && !g_flags.h) printf("%s:", argv[i]);
        if (g_flags.l && matched_lines_counter) {
          matched_lines_counter = 1;
          printf("%d\n", matched_lines_counter);
        } else {
          printf("%d\n", matched_lines_counter);
        }
      }
      if (g_flags.l && is_appropiate) {
        printf("%s\n", argv[i]);
      }
    }
    regfree(&regx);
  }
  return 0;
}

int parse_stdin(int argc, char **argv, flags_variants *g_flags) {
  int option;
  int flag_error = 0;

  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", 0, 0)) != -1 &&
         !flag_error) {
    switch (option) {
      case 'i':
        g_flags->i = 1;
        break;
      case 'v':
        g_flags->v = 1;
        break;
      case 'e':
        g_flags->e = 1;
        add_pattern(g_flags, optarg);
        break;
      case 'c':
        g_flags->c = 1;
        break;
      case 'l':
        g_flags->l = 1;
        break;
      case 'n':
        g_flags->n = 1;
        break;
      case 'h':
        g_flags->h = 1;
        break;
      case 's':
        g_flags->s = 1;
        break;
      case 'f':
        g_flags->f = 1;
        add_patterns_from_file(g_flags, optarg);
        break;
      case 'o':
        g_flags->o = 1;
        break;
      default:
        flag_error = 1;
        printf("s21_cat: illegal option");
        break;
    }
  }
  if (g_flags->c || g_flags->l) {
    g_flags->n = 0;
    g_flags->o = 0;
  }
  if (g_flags->p_len == 0) add_pattern(g_flags, argv[optind++]);
  return flag_error;
}

void parse_file(regex_t regx, char *file_name, flags_variants g_flags,
                char **argv, int files_counter, int file_index,
                int *matched_lines_counter, int *is_appropiate) {
  FILE *file;
  file = fopen(file_name, "r");
  if (!file) {
    if (!g_flags.s) printf("No such file or directory");
    exit(1);
  }
  size_t str_len = 0;
  char *str = NULL;
  int line_counter = 0;
  int symbols_read;
  while ((symbols_read = getline(&str, &str_len, file)) != -1) {
    ++line_counter;
    int res = regexec(&regx, str, 0, NULL, 0);
    if (!res && !g_flags.v) {
      *is_appropiate = 1;
      if (files_counter > 1 && !g_flags.c && !g_flags.l && !g_flags.h)
        printf("%s:", argv[file_index]);
      if (g_flags.n) printf("%d:", line_counter);
      if (g_flags.o) {
        print_matches(regx, str);
      }
      if (!g_flags.c && !g_flags.l && !g_flags.o) print_str(str, symbols_read);
      (*matched_lines_counter)++;
    } else if (res && g_flags.v) {
      *is_appropiate = 1;
      if (files_counter > 1 && !g_flags.c && !g_flags.l && !g_flags.h)
        printf("%s:", argv[file_index]);
      if (g_flags.n) printf("%d:", line_counter);
      if (!g_flags.c && !g_flags.l) print_str(str, symbols_read);
      (*matched_lines_counter)++;
    }
  }
  free(str);
  fclose(file);
}

void print_matches(regex_t regx, char *str) {
  regmatch_t matches;
  int offset = 0;
  while (1) {
    int matches_res = regexec(&regx, str + offset, 1, &matches, 0);
    if (matches_res != 0) break;
    for (int i = matches.rm_so; i < matches.rm_eo; i++) {
      putchar(str[i]);
    }
    putchar('\n');
    offset += matches.rm_eo;
  }
}

void print_str(char *str, int str_len) {
  for (int i = 0; i < str_len; i++) {
    printf("%c", str[i]);
  }
  if (str[str_len - 1] != '\n') printf("%c", '\n');
}

void add_pattern(flags_variants *g_flags, char *pattern) {
  if (g_flags->p_len != 0) {
    strcat(g_flags->pattern + g_flags->p_len, "|");
    g_flags->p_len++;
  }
  g_flags->p_len += sprintf(g_flags->pattern + g_flags->p_len, "(%s)", pattern);
}

void add_patterns_from_file(flags_variants *g_flags, char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    if (!g_flags->s) printf("No such file or directory");
    exit(1);
  }
  size_t str_len = 0;
  char *str = NULL;
  int symbols_read;
  while ((symbols_read = getline(&str, &str_len, file)) != -1) {
    if (str[symbols_read - 1] == '\n') str[symbols_read - 1] = '\0';
    add_pattern(g_flags, str);
  }
  free(str);
  fclose(file);
}
// на гетлайне нужно добавлять макрос

//-e, -i, -v, -c, -l, -n

//-h, -s, -f, -o

//осталось -f и -o