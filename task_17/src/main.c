#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#include "number_utils.h"

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define access _access
    #define F_OK 0
#else
    #include <sys/stat.h>
    #include <unistd.h>
#endif

int files_are_same(const char* path1, const char* path2) {
    struct stat st1, st2;

    if (stat(path1, &st1) != 0) {
        return 0;
    }

    if (stat(path2, &st2) != 0) {
        return 0;
    }

    return (st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino);
}

char* read_line(FILE *file) {
    size_t buffer_size = 128;
    char *line = malloc(buffer_size);
    if (!line) {
        return NULL;
    }

    size_t pos = 0;
    int c;

    while ((c = fgetc(file)) != EOF && c != '\n') {
        if (pos + 1 >= buffer_size) {
            buffer_size *= 2;
            char *new_line = realloc(line, buffer_size);
            if (!new_line) {
                free(line);
                return NULL;
            }
            line = new_line;
        }
        line[pos++] = (char) c;
    }

    if (c == EOF && pos == 0) {
        free(line);
        return NULL;
    }

    line[pos] = '\0';
    return line;
}

char* clean_token(const char* token, size_t* out_len) {
    size_t len = strlen(token);
    char* cleaned = malloc(len + 1);
    if (!cleaned) {
        return NULL;
    }

    size_t clean_len = 0;
    for (size_t i = 0; i < len; i++) {
        if (token[i] == '-') {
            cleaned[clean_len] = '-';
            clean_len++;
        }
        if (isalnum((unsigned char)token[i])) {
            cleaned[clean_len] = toupper((unsigned char)token[i]);
            clean_len++;
        }
    }
    cleaned[clean_len] = '\0';
    *out_len = clean_len;
    return cleaned;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong amount of arguments.\n");
        return 1;
    }

    if (files_are_same(argv[1], argv[2])) {
        fprintf(stderr, "Error: Input and output file are the same: '%s'.\n", argv[1]);
        return 2;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Cannot open input file '%s'.\n", argv[1]);
        return 2;
    }

    FILE *output_file = fopen(argv[2], "w");
    if (!output_file) {
        fclose(input_file);
        fprintf(stderr, "Error: Cannot open output file '%s'.\n", argv[2]);
        return 3;
    }

    char *line = NULL;
    size_t line_count = 0;
    int total_errors = 0;

    while ((line = read_line(input_file)) != NULL) {
        line_count++;

        char *token = strtok(line, " \t\n");
        while (token) {
            size_t clean_len;
            char* cleaned = clean_token(token, &clean_len);
            if (!cleaned) {
                fprintf(stderr, "Error: Memory allocation failed for token at line %zu.\n", line_count);
                total_errors++;
                token = strtok(NULL, " \t\n");
                continue;
            }

            if (clean_len == 0) {
                free(cleaned);
                token = strtok(NULL, " \t\n");
                continue;
            }

            int min_base = find_minimal_base(cleaned);
            if (min_base == -1) {
                fprintf(stderr, "Error: Invalid number format at line %zu: '%s'\n", line_count, token);
                total_errors++;
                free(cleaned);
                token = strtok(NULL, " \t\n");
                continue;
            }

            long long decimal_value = convert_to_decimal(cleaned, min_base);
            if (decimal_value == LLONG_MIN) {
                fprintf(stderr, "Error: Overflow during conversion at line %zu: '%s'\n", line_count, token);
                total_errors++;
                free(cleaned);
                token = strtok(NULL, " \t\n");
                continue;
            }

            fprintf(output_file, "%s %d %lld\n", cleaned, min_base, decimal_value);
            free(cleaned);
            token = strtok(NULL, " \t\n");
        }

        free(line);
        line = NULL;
    }

    if (ferror(input_file)) {
        fprintf(stderr, "Error: Failed to read input file.\n");
        fclose(input_file);
        fclose(output_file);
        return 5;
    }

    fclose(input_file);
    fclose(output_file);

    if (total_errors > 0) {
        fprintf(stderr, "Total errors encountered: %d\n", total_errors);
        return 4;
    }

    return 0;
}
