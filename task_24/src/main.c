#include <stdio.h>
#include "cust_scanf.h"

int main(void) {
    int roman_value;
    unsigned int zeck_value;
    int base_value;

    overfscanf(stdin, "%Ro", &roman_value);

    oversscanf("101001", "%Zr", &zeck_value);

    oversscanf("1a3f", "%Cv", &base_value, 16);

    oversscanf("1A3F", "%CV", &base_value, 16);

    printf("Roman value: %d\n", roman_value);
    printf("Zeckendorf value: %u\n", zeck_value);
    printf("Base value: %d\n", base_value);

    return 0;
}
