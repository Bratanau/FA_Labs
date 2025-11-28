#include "functions.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>


void print_status(StatusCode s) {
    printf("Статус: ");
    switch (s) {
        case STATUS_OK:
            printf("Успешно.\n");
            break;
        case INVALID_INPUT:
            printf("ОШИБКА: Неверный ввод.\n");
            break;
        case OPEN_ERROR:
            printf("ОШИБКА: Не удалось открыть входной файл.\n");
            break;
        case WRITE_ERROR:
            printf("ОШИБКА: Не удалось открыть/создать выходной файл.\n");
            break;
        case READ_ERROR:
            printf("ОШИБКА: Ошибка чтения из файла.\n");
            break;
        default:
            printf("Неизвестный статус.\n");
            break;
    }
}

StatusCode path_validator(const char* const filename) {
    if (filename == NULL) {
        return INVALID_INPUT;
    }
    if (strlen(filename) > PATH_MAX_LEN || strlen(filename) == 0) {
        return INVALID_INPUT;
    }
    return STATUS_OK;
}

StatusCode get_paths(char* inp, char* outp) {
    printf("Введите путь к входному файлу: ");
    if (fgets(inp, PATH_MAX_LEN, stdin) == NULL) {
        return READ_ERROR;
    }
    inp[strcspn(inp, "\n")] = 0;
    printf("Введите путь к выходному файлу: ");
    if (fgets(outp, PATH_MAX_LEN, stdin) == NULL) {
        return READ_ERROR;
    }
    outp[strcspn(outp, "\n")] = 0;

    StatusCode s_in = path_validator(inp);
    if (s_in != STATUS_OK) {
        return INVALID_INPUT;
    }
    StatusCode s_out = path_validator(outp);
    if (s_out != STATUS_OK) {
        return INVALID_INPUT;
    }
    return STATUS_OK;
}

Word* split_line(char* const line, int* num_words) {
    if (line == NULL || num_words == NULL) {
        return NULL;
    }
    *num_words = 0;
    int line_len = strlen(line);
    bool in_word = false;
    int count = 0;
    for (int i = 0; i < line_len; i++) {
        if (isspace((unsigned char)line[i])) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            count++;
        }
    }
    *num_words = count;
    if (count == 0) {
        return NULL;
    }
    Word* words = (Word* )malloc(count * sizeof(Word));
    if (words == NULL) {
        return NULL;
    }
    char* saveptr;
    char* token = strtok_r(line, " \t\n\r", &saveptr);
    int i = 0;
    while (token != NULL) {
        words[i].word = (char*)malloc(strlen(token) + 1); 
        if (words[i].word == NULL) {
            for (int j = 0; j < i; j++) {
                free(words[j].word);
            }
            free(words);
            return NULL; 
        }
        strcpy(words[i].word, token);
        words[i].len = strlen(token);
        i++;
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }
    return words;
}

void free_words(Word* words, int num_words) {
    if (words) {
        for (int i = 0; i < num_words; i++) {
            free(words[i].word); 
        }
        free(words); 
    }
}

StatusCode write_spaces(FILE* const fout, const int count) {
    if (fout == NULL) {
        return INVALID_INPUT;
    }
    for (int i = 0; i < count; i++) {
        if (fputc(' ', fout) == EOF) {
            return WRITE_ERROR;
        }
    }
    return STATUS_OK;
}

StatusCode process_file(FILE* const fin, FILE* const fout) {
    if (fin == NULL || fout == NULL) {
        return INVALID_INPUT;
    }
    
    char* buffer = (char*)malloc(MAX_LINE_BUFFER * sizeof(char));
    if (buffer == NULL) {
        return MEMORY_ERROR;
    }
    
    StatusCode st = STATUS_OK;

    while (fgets(buffer, MAX_LINE_BUFFER, fin) != NULL) {
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (strlen(buffer) == 0) {
            if (fputc('\n', fout) == EOF) {
                st = WRITE_ERROR;
                break;
            }
        }

        int words_num;
        Word* words = split_line(buffer, &words_num);
        if (words == NULL && words_num > 0) {
            st = MEMORY_ERROR;
            break;
        }
        if (words_num == 0) {
            if (fputc('\n', fout) == EOF) {
                st = WRITE_ERROR;
            }
        }

        int curr_ind = 0;
        while (curr_ind < words_num) {
            int words_in_line = 0;
            int current_line_length = 0;
            int start_ind = curr_ind;

            while (curr_ind < words_num) {
                int word_length = words[curr_ind].len;
                
                if (words_in_line == 0) {
                    if (word_length > LINE_WIDTH) {
                        words_in_line = 1;
                        curr_ind++;
                        break;
                    }
                    current_line_length = word_length;
                    words_in_line = 1;
                    curr_ind++;
                } else {
                    int new_length = current_line_length + 1 + word_length;
                    if (new_length > LINE_WIDTH) {
                        break;
                    }
                    current_line_length = new_length;
                    words_in_line++;
                    curr_ind++;
                }
            }
            int is_last_line = (curr_ind >= words_num);
            
            if (is_last_line && current_line_length < LINE_WIDTH) {
                for (int i = 0; i < words_in_line; i++) {
                    fprintf(fout, "%s", words[start_ind + i].word);
                    if (i < words_in_line - 1) {
                        fputc(' ', fout);
                    }
                }
                fputc('\n', fout);
            } else {
                int total_word_length = 0;
                for (int i = 0; i < words_in_line; i++) {
                    total_word_length += words[start_ind + i].len;
                }

                int needed_spaces = LINE_WIDTH - total_word_length;
                int num_gaps = words_in_line - 1;

                if (num_gaps > 0) {
                    int spaces_per_gap = needed_spaces / num_gaps;
                    int extra_spaces = needed_spaces % num_gaps;

                    for (int i = 0; i < words_in_line; i++) {
                        fprintf(fout, "%s", words[start_ind + i].word);
                        if (i < num_gaps) {
                            int spaces = spaces_per_gap + (i < extra_spaces ? 1 : 0);
                            for (int j = 0; j < spaces; j++) {
                                fputc(' ', fout);
                            }
                        }
                    }
                } else {
                    fprintf(fout, "%s", words[start_ind].word);
                    int remaining = LINE_WIDTH - words[start_ind].len;
                    for (int j = 0; j < remaining; j++) {
                        fputc(' ', fout);
                    }
                }
                fputc('\n', fout);
            }
        }
        
        free_words(words, words_num);
        if (st != STATUS_OK) {
            break;
        }
    }
    
    free(buffer);
    return st;
}