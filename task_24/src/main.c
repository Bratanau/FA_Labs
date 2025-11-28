#include <stdio.h>
#include "cust_scanf.h"

int main(void) {
    int roman_value;
    unsigned int zeck_value;
    int base_value;

    overfscanf(file, "%Ro", &roman_value);

    oversscanf("101001", "%Zr", &zeck_value);

    oversscanf("1a3f", "%Cv", &base_value, 16);

    oversscanf("1A3F", "%CV", &base_value, 16);

    return 0;
}
