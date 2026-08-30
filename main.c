#include <stdio.h>
#include <stdlib.h>

#include "strview.h"

int main() 
{
    strview_t sv = sv_from_cstr("test-string");

    printf("string: %s\n", sv.data);
    printf("len: %zu\n", sv_len(sv));
    printf("is empty (0 if empty): %d\n", sv_empty(sv));

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
    sv_prefix(sv, 3, &prefix);
    sv_suffix(sv, 5, &suffix);
    printf("string prefix: ");
    sv_println_safe_stdout(prefix);
    printf("string suffix: ");
    sv_println_safe_stdout(suffix);

    printf("\n");

    strview_t sv2 = sv_from_cstr("abcdefghij");
    printf("original string: ");
    sv_println_safe_stdout(sv2);
    printf("string after removing prefix: ");
    sv_rem_prefix(&sv2, 4);
    sv_println_safe_stdout(sv2);

    printf("\n");

    strview_t sv3 = sv_from_cstr("abcdefghij");
    printf("original string: ");
    sv_println_safe_stdout(sv3);
    printf("string after removing suffix: ");
    sv_rem_suffix(&sv3, 7);
    sv_println_safe_stdout(sv3);

    printf("\n");

    strview_t save_rem_prefix;
    strview_t sv4 = sv_from_cstr("abcdefghij");
    printf("original string: ");
    sv_println_safe_stdout(sv4);
    printf("string after removing prefix: ");
    sv_rem_prefix_save(&sv4, 4, &save_rem_prefix);
    sv_println_safe_stdout(sv4);
    printf("string that was removed: ");
    sv_println_safe_stdout(save_rem_prefix);

    printf("\n");

    strview_t save_rem_suffix;
    strview_t sv5 = sv_from_cstr("abcdefghij");
    printf("original string: ");
    sv_println_safe_stdout(sv5);
    printf("string after removing suffix: ");
    sv_rem_suffix_save(&sv5, 7, &save_rem_suffix);
    sv_println_safe_stdout(sv5);
    printf("string that was removed: ");
    sv_println_safe_stdout(save_rem_suffix);

    return 0;
}
