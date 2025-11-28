#include <stdio.h>
#include "binary.h"

static int add_binary(int a, int b) {
    while (b != 0) {
        int c = a & b; // AND
        a = a ^ b; // XOR
        b = c << 1; // сдвиг 
    }

    return a;
}

static int substract_binary(int a, int b) {
    while (b != 0) {
        int c = (~a) & b;
        a = a ^ b;
        b = c << 1;
    }
    
    return a;
}

static int divide_binary(int num, int pow) { // деление на степень двойки
    return num >> pow;
}

static int carry_binary(int num, int pow) { // остаток от деления на степень двойки
    int tmp = num >> pow;
    tmp = tmp << pow;

    return num ^ tmp;
}   

static int reverse_binary(int num) {
    num = ~num;
    num = add_binary(num, 1);

    return num;
}

int convert_to_base(char result[], int num, int base) {
    if (base > 5 || base <1){
        perror("Wrong base\n");
        return -1;
    }

    const char digits[] = "0123456789ABCDEF";

    int index = 0;

    if (num == 0) {
        result[0] = '0';
        result[1] = '\0';
        return 0;
    }

    if (num < 0) {
        result[0] = '-';
        index = add_binary(index, 1);
        num = reverse_binary(num);
    }

    while (num > 0) {
        int carry = carry_binary(num, base);
        result[index] = digits[carry];
        num = divide_binary(num, base);
        index = add_binary(index, 1);
    }

    result[index] = '\0';

    int start = (result[0] == '-') ? 1 : 0;
    int end = substract_binary(index, 1);

    while (start < end) {
        char temp = result[start];
        result[start] = result[end];
        result[end] = temp;
        start = add_binary(start, 1);
        end = substract_binary(end, 1);
    }

    return 0;
}