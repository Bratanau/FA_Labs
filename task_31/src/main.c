#include <stdio.h>
#include <string.h>
#include "binary.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Должно быть 2 аргумента: число и основание\n");
        return 1;
    }

    char buffer[30];

    int num = atoi(argv[1]);
    int base = atoi(argv[2]);

    convert_to_base(buffer, num, base);

    printf("Result: %s\n", buffer);
    return 0;
}