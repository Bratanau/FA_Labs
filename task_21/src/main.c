#include "has_finite_representation.h"
#include <stdio.h>
#include <stdlib.h>

static void print_result(frs_status_t status) {
    switch (status) {
        case FRS_SUCCESS:
            printf("Успешно выполнено\n");
            break;
        case FRS_INVALID_BASE:
            printf("Ошибка: некорректное основание\n");
            break;
        case FRS_INVALID_COUNT:
            printf("Ошибка: некорректное количество аргументов\n");
            break;
        case FRS_INVALID_NUMBER:
            printf("Ошибка: некорректное число (должно быть в диапазоне (0;1))\n");
            break;
        case FRS_MEMORY_ERROR:
            printf("Ошибка: не удалось выделить память\n");
            break;
        case FRS_INVALID_EPSILON:
            printf("Ошибка: некорректное значение эпсилон (должно быть положительным)\n");
            break;
        default:
            printf("Неизвестная ошибка\n");
            break;
    }
}

int main(int argc, char **argv) {
    // Проверка минимального количества аргументов: программа, epsilon, base, и хотя бы одно число
    if (argc < 3) {
        fprintf(stderr, "Использование: %s <base> <epsilon>\n", argv[0]);
        fprintf(stderr, "Пример: %s 0.0001 10\n", argv[0]);
        return 1;
    }

    double epsilon = strtod(argv[2], NULL);
    int base = atoi(argv[1]);
    size_t count = 5;

    frs_status_t status = check_finite_representation(base, count, epsilon, 0.5, 0.125, 0.2,  0.25, 1.0 / 3);
    print_result(status);
    
    return 0;
}
