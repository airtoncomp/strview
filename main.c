#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

    printf("\n");

    strview_t sv6 = sv_from_cstr("abcd");
    strview_t sv7 = sv_from_cstr("abcd");
    printf("compare string (0 = equal, positive = left > right, negative = otherwise): ");
    printf("%d\n", sv_cmp(sv6, sv7));
    sv_rem_prefix(&sv7, 2);
    printf("compare string (0 = equal, positive = left > right, negative = otherwise): ");
    printf("%d\n", sv_cmp(sv6, sv7));

    printf("\n");

    strview_t sv8 = sv_from_cstr("abcdec");
    char c = 'c';
    size_t pos;
    sv_find_char(sv8, 'c', &pos);
    printf("first occurrence of char in string: ");
    sv_println_safe_stdout(sv8);
    printf("found %c at %zu\n", c, pos);
    sv_rfind_char(sv8, 'c', &pos);
    printf("found %c at %zu (reverse find)\n", c, pos);

    printf("\n");

    strview_t sv9 = sv_from_cstr("test-case");
    const char *str = "case";
    sv_find_cstr(sv9, str, &pos);
    printf("found word %s at %zu\n", str, pos);

    printf("\n");

    strview_t sv10 = sv_from_cstr("hello-world");
    strview_t sv11 = sv_from_cstr("ld");
    printf("it starts with prefix (0 = yes, -1 = no): %d\n", sv_starts_with(sv10, sv11));
    printf("it ends with suffix (0 = yes, -1 = no): %d\n", sv_ends_with(sv10, sv11));

    printf("\n");

    strview_t sv12 = sv_from_cstr("this is my library");
    strview_t sv13 = sv_from_cstr("is");
    printf("it contains (0 = yes, -1 = no): %d\n", sv_contains(sv12, sv13));

    printf("\n");

    strview_t sv14 = sv_from_cstr("string to be trimmed");
    size_t buflen = 256;
    char buf[buflen];
    sv_trim(sv14, buf, buflen);
    printf("trimmed: %s\n", buf);

    printf("\n");

    strview_t sv15 = sv_from_cstr("+123313409090");
    strview_t sv16 = sv_from_cstr("-9892321");
    uint64_t n64_u;
    int64_t n64;
    sv_to_u64(sv15, &n64_u);
    printf("string to u64: %ld\n", n64_u);
    sv_to_i64(sv16, &n64);
    printf("string to i64: %ld\n", n64);

    printf("\n");

    strview_t sv17 = sv_from_cstr("+778");
    strview_t sv18 = sv_from_cstr("-5436");
    uint32_t n32_u;
    int32_t n32;
    sv_to_u32(sv17, &n32_u);
    printf("string to u32: %d\n", n32_u);
    sv_to_i32(sv18, &n32);
    printf("string to i32: %d\n", n32);

    printf("\n");
    strview_t sv19 = sv_from_cstr("+23.9876");
    strview_t sv20 = sv_from_cstr("-989.23");
    strview_t sv21 = sv_from_cstr("32e-3");
    strview_t sv22 = sv_from_cstr("-89.1e+4");
    double nd, nde;
    float nf, nfe;
    sv_to_double(sv19, &nd);
    sv_to_float(sv20, &nf);
    sv_to_double(sv21, &nde);
    sv_to_float(sv22, &nfe);
    printf("string to double: %lf\n", nd);
    printf("string to float: %f\n", nf);
    printf("string to double (e): %e\n", nde);
    printf("string to float (e): %e\n", nfe);

    return 0;
}
