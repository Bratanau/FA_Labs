#include "has_finite_representation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <limits.h>

static int is_in_range(double num, double epsilon) {
    if (epsilon <= 0.0) return 0;
    return (num > epsilon) && (num < (1.0 - epsilon)); // проверяем, что число находится в диапазоне (epsilon; 1-epsilon)
}

static long long gcd(long long a, long long b) { // алготитм Евклида
    if (a < 0) a = -a;
    if (b < 0) b = -b;    
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int has_finite_representation(double num, int base, double epsilon) {
    if (!is_in_range(num, epsilon) || base <= 1) {
        return -1; 
    }

    double temp = num;
    long long scale = 1;
    int precision = 0;
    const int MAX_PRECISION = 15;
    
    while (precision < MAX_PRECISION) {
        if (fabs(temp - round(temp)) < epsilon) {
            break;
        }
        temp *= 10;
        scale *= 10;
        precision++;
    }
    
    long long p = (long long)round(temp);
    long long q = scale;
    
    long long g = gcd(p, q);
    if (g == 0) return -1; 
    p /= g;
    q /= g;
    
    if (q <= 0) return -1;
    
    long long temp_q = q;
    int base_copy = base;

    while (temp_q > 1) {
        int found_factor = 0;
        for (int i = 2; i <= base; i++) {
            if (base_copy % i == 0) {
                while (base_copy % i == 0) {
                    base_copy /= i;
                    }
                while (temp_q % i == 0) {
                    temp_q /= i;
                    found_factor = 1;
                }
            }
        }
        
        if (temp_q == 1) {
            return 1;
        } else {
            return 0;
        }
    }
}

frs_status_t check_finite_representation(int base, size_t count, double epsilon, ...) {
    if (epsilon <= 0.0) {
        return FRS_INVALID_EPSILON;
    }
    
    if (base <= 1) {
        return FRS_INVALID_BASE;
    }
    
    if (count == 0) {
        return FRS_INVALID_COUNT;
    }
    
    va_list args;
    va_start(args, epsilon);
    
    double *numbers = malloc(count * sizeof(double));
    if (numbers == NULL) {
        va_end(args);
        return FRS_MEMORY_ERROR;
    }
    
    for (size_t i = 0; i < count; i++) {
        numbers[i] = va_arg(args, double);
        if (!is_in_range(numbers[i], epsilon)) {
            free(numbers);
            va_end(args);
            return FRS_INVALID_NUMBER;
        }
    }
    
    va_end(args);
    
    printf("Проверка %zu чисел в системе счисления с основанием %d:\n", count, base);
    for (size_t i = 0; i < count; i++) {
        int result = has_finite_representation(numbers[i], base, epsilon);
        if (result == -1) {
            printf("Ошибка при проверке числа %f\n", numbers[i]);
            free(numbers);
            return FRS_INVALID_NUMBER;
        }
        
        printf("  %f -> %s\n", numbers[i], result ? "конечное представление" : "бесконечное представление");
    }
    
    free(numbers);
    return FRS_SUCCESS;
}
