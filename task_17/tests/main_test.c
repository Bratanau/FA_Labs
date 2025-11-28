#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "number_utils.h"
#include <assert.h>

int main(void) {
    assert(find_minimal_base("0") == 2);        
    assert(find_minimal_base("1") == 2);      
    assert(find_minimal_base("A") == 11);
    assert(find_minimal_base("Z") == 36);       
    assert(find_minimal_base("GHI") == 19);
    assert(find_minimal_base("") == -1);
    assert(find_minimal_base("123") == 4);    

    assert(convert_to_decimal("0", 2) == 0);
    assert(convert_to_decimal("1", 2) == 1);
    assert(convert_to_decimal("10", 2) == 2);
    assert(convert_to_decimal("A", 16) == 10);
    assert(convert_to_decimal("Z", 36) == 35);
    assert(convert_to_decimal("10", 10) == 10);
    assert(convert_to_decimal("123", 5) == 38); 
    assert(convert_to_decimal("", 10) == LLONG_MIN);       
    assert(convert_to_decimal("123", 2) == LLONG_MIN);
    assert(convert_to_decimal("GHI", 18) == LLONG_MIN);     

    printf("All tests passed.\n");
    return 0;
}
