#include <stdio.h>
#include <stdlib.h>

#include "strview.h"

int main() 
{
    strview_t sv = sv_from_cstr("test string");

    printf("string: %s\n", sv.data);
    printf("len: %zu\n", sv_len(sv));
    printf("is empty: %d\n", sv_empty(sv));

    char buf1[256];
    sv_to_cstr(sv, buf1, 256);
    printf("to cstr: %s\n", buf1);

    strview_t out;
    if (sv_substr(sv, 5, 3, &out) < 0) {
        fprintf(stderr, "sv_substr() failed\n");
        return -1;
    }
    printf("substring: %.*s, len: %zu\n", (int) out.len, out.data, out.len);
    sv_print_safe_stdout(out);

    char buf2[256];
    sv_to_cstr(out, buf2, 256);
    printf("to cstr: %s\n", buf2);

    strview_t suffix, prefix;
    sv_prefix(sv, 4, &prefix);
    sv_suffix(sv, 5, &suffix);
    sv_println_safe_stdout(prefix);
    sv_println_safe_stdout(suffix);

    return 0;
}
