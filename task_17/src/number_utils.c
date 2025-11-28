#include "number_utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int find_minimal_base(const char *num_str) {
    if (!num_str || strlen(num_str) == 0) {
        return -1;
    }

    int max_digit = 0;

    size_t i = 0;
    if (num_str[0] == '-') {
        i = 1;
    }
    for (i; num_str[i]; i++) {
        char c = num_str[i];
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            digit = c - 'a' + 10;
        } else {
            return -1;
        }
        if (digit > max_digit) {
            max_digit = digit;
        }
    }

    int minimal_base = max_digit + 1;
    if (minimal_base < 2) {
        minimal_base = 2;
    }
    if (minimal_base > 36) {
        return -1;
    }

    return minimal_base;
}

long long convert_to_decimal(const char *num_str, int base) {
    if (!num_str || strlen(num_str) == 0 || base < 2 || base > 36) {
        return LLONG_MIN;
    }

    long long result = 0;
    size_t len = strlen(num_str);

    size_t negative = 0;
    if (num_str[0] == '-') {
        negative = 1;
    }

    for (size_t i = negative; i < len; i++) {
        char c = num_str[i];
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            digit = c - 'A' + 10;
        } else {
            return LLONG_MIN;
        }

        if (digit >= base) {
            return LLONG_MIN;
        }

        if (result > (LLONG_MAX - digit) / base) {
            return LLONG_MIN;
        }

        result = result * base + digit;
    }

    if (negative) result = -result;
    
    return result;
}
