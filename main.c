#include <stdio.h>
#include <stdlib.h>

#include "strview.h"

int main() 
{
    strview_t sv = sv_from_cstr("test string");

    printf("string: %s\n", sv.str);
    printf("len: %zu\n", sv_len(sv));
    printf("is empty: %d\n", sv_empty(sv));

    return 0;
}
