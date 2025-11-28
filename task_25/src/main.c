#include "functions.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    printf("Введите пути к файлам ввода и вывода\n");
    char input_filename[PATH_MAX_LEN];
    char output_filename[PATH_MAX_LEN];
    FILE* fin = NULL;
    FILE* fout = NULL;

    StatusCode s = get_paths(input_filename, output_filename);
    if (s != STATUS_OK) {
        print_status(s);
        return INVALID_INPUT;
    }
    fin = fopen(input_filename, "r");
    if (fin == NULL) {
        s = OPEN_ERROR;
        print_status(s);
        return OPEN_ERROR;
    }
    fout = fopen(output_filename, "w");
    if (fout == NULL) {
        fclose(fin);
        s = OPEN_ERROR;
        print_status(s);
        return OPEN_ERROR;
    }
    printf("Начало обработки: \n");
    
    s = process_file(fin, fout);
    if (fin != NULL) {
        fclose(fin);
    }
    if (fout != NULL) {
        fclose(fout);
    }

    printf("Обработка завершена");


}