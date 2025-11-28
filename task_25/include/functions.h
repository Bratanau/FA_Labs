#include "status_codes.h"
#define PATH_MAX_LEN 256
#define MAX_LINE_BUFFER 1024
#define LINE_WIDTH 80
#include <stdio.h>

typedef struct {
    char* word;
    int len;
} Word;

StatusCode path_validator(const char* const filename);

StatusCode get_paths(char* inp, char* outp);

Word* split_line(char* const line, int* num_words);

void free_words(Word* words, int num_words);

StatusCode write_spaces(FILE* const fout, const int count);

StatusCode process_file(FILE* const fin, FILE* const fout);

void print_status(StatusCode s);

