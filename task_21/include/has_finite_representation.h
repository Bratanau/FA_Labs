#ifndef HAS_FINITE_REPRESENTATION_H
#define HAS_FINITE_REPRESENTATION_H

#include <stdarg.h>
#include <stdio.h>

typedef enum {
    FRS_SUCCESS = 0,           
    FRS_INVALID_BASE,          
    FRS_INVALID_COUNT,
    FRS_INVALID_NUMBER,
    FRS_MEMORY_ERROR,
    FRS_INVALID_EPSILON       
} frs_status_t;

frs_status_t check_finite_representation(int base, size_t count, double epsilon, ...);

int has_finite_representation(double num, int base, double epsilon);

#endif
