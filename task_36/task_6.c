#include <stdio.h>
#include <string.h>

int check_brackets(const char *str) {
    char stack[1000];
    int top = 0;
    
    while (*str) {
        if (*str == '(') {
            stack[top] = ')';
            top++;
        } else if (*str == '[') {
            stack[top] = ']';
            top++;
        } else if (*str == '{') {
            stack[top] = '}';
            top++;
        } else if (*str == '<') {
            stack[top] = '>';
            top++;
        }
        else if (*str == ')' || *str == ']' || *str == '}' || *str == '>') {
            if (top == 0) {
                return 0;
            }

            top--;
            if (stack[top] != *str) {
                return 0;  
            }
        }
        
        str++;
    }
    
    return top == 0;  
}

int main(void) {
    char buffer[1000];

    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        fprintf(stderr, "Cannot get the string.\n");  
        return -1;
    }
    
    if (check_brackets(buffer)) {
        printf("That's fine.\n");
    } else {
        printf("Nah.\n");
    }
    
    return 0;
}