/**
 * Copyright 2026, Airton Ishimori
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the “Software”), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>

#include "strview.h"

#ifdef DEBUG

#define SV_LOG(...) do {                                                \
            fprintf(stdout, "[DEBUG] %s:%d: ", __FILE__, __LINE__);     \
            fprintf(stdout, __VA_ARGS__);                               \
        } while(0)

#define SV_LOGE(...) do {                                               \
            fprintf(stderr, "[DEBUG] %s:%d: ", __FILE__, __LINE__);     \
            fprintf(stderr, __VA_ARGS__);                               \
        } while(0)

#else

#define SV_LOG(stdout, ...)                 ((void)0)
#define SV_LOGE(stderr, ...)                ((void)0)

#endif

#define SV_RET_ERR_ON_NULL(x, fmt, ...) do {                         \
            if (x == NULL) {                                         \
                fprintf(stderr, "FAIL: " fmt"\n", ##__VA_ARGS__);    \
                return -1;                                           \
            }                                                        \
        } while(0)

#define SV_RET_ERR_ON_TRUE(x, fmt, ...) do {                         \
            if (x) {                                                 \
                fprintf(stderr, "FAIL: " fmt"\n", ##__VA_ARGS__);    \
                return -1;                                           \
            }                                                        \
        } while(0)

strview_t sv_from_cstr(const char *s)
{
    strview_t sv = { .data = s, .len = s ? strlen(s) : 0 };
    return sv;
}

strview_t sv_from_cstr_len(const char *s, size_t len)
{
    strview_t sv = { .data = s, .len = len };
    return sv;
}

int sv_to_cstr(strview_t sv, char *buf, size_t buflen)
{
    SV_RET_ERR_ON_NULL(buf, "buffer is null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(buflen < sv.len, "insufficient buffer room");
    memcpy(buf, sv.data, sv.len);
    buf[sv.len] = '\0';  
    return sv.len;
}

inline size_t sv_len(strview_t sv)
{
    return sv.len;
}

inline int sv_empty(strview_t sv)
{
    return sv.len > 0;
}

int sv_substr(strview_t sv, size_t pos, size_t len, strview_t *out)
{
    SV_RET_ERR_ON_NULL(out, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(pos > sv.len || len > sv.len - pos, "invalid position or string length");
    out->data = sv.data + pos;
    out->len = len;
    return 0;
}

int sv_prefix(strview_t sv, size_t end_pos, strview_t *out)
{
    SV_RET_ERR_ON_NULL(out, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(end_pos > sv.len, "invalid prefix ending position");
    out->data = sv.data;
    out->len = end_pos + 1;
    return 0;
}

int sv_suffix(strview_t sv, size_t start_pos, strview_t *out)
{
    SV_RET_ERR_ON_NULL(out, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(start_pos > sv.len, "invalid suffix starting position");
    out->data = sv.data + start_pos;
    out->len = sv.len - start_pos;
    return 0;
}

int sv_rem_prefix(strview_t *sv, size_t end_pos)
{
    SV_RET_ERR_ON_NULL(sv->data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(end_pos > sv->len, "invalid prefix ending position");
    sv->data = sv->data + end_pos + 1;
    sv->len = sv->len - end_pos;
    return 0;
}

int sv_rem_prefix_save(strview_t *sv, size_t end_pos, strview_t *save)
{
    SV_RET_ERR_ON_NULL(save, "null pointer");
    SV_RET_ERR_ON_NULL(sv->data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(end_pos > sv->len, "invalid prefix ending position");
    save->data = sv->data;
    save->len = end_pos + 1;
    sv->data = sv->data + end_pos + 1;
    sv->len = sv->len - end_pos;
    return 0;
}

int sv_rem_suffix(strview_t *sv, size_t start_pos)
{
    SV_RET_ERR_ON_NULL(sv->data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(start_pos > sv->len, "invalid suffix starting position");
    sv->len = start_pos;
    return 0;
}

int sv_rem_suffix_save(strview_t *sv, size_t start_pos, strview_t *save)
{
    SV_RET_ERR_ON_NULL(save, "null pointer");
    SV_RET_ERR_ON_NULL(sv->data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(start_pos > sv->len, "invalid suffix starting position");
    save->data = sv->data + start_pos;
    save->len = sv->len - start_pos;
    sv->len = start_pos;
    return 0;
}

int sv_cmp(strview_t a, strview_t b)
{
    return strcmp(a.data, b.data);
}

int sv_find_char(strview_t sv, char c, size_t *pos)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(pos, "argument is null pointer");

    for (size_t i = 0; i < sv.len; i++) {
        if (*(sv.data + i) == c) {
            *pos = i;
            return 0;
        }
    }

    return -1;
}

int sv_rfind_char(strview_t sv, char c, size_t *pos)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(pos, "argument is null pointer");

    for (size_t i = sv.len - 1; i > 0; i--) {
        if (*(sv.data + i) == c) {
            *pos = i;
            return 0;
        }
    }
    return -1;
}

int sv_find_cstr(strview_t sv, const char *str, size_t *pos)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(pos, "argument is null pointer");
    SV_RET_ERR_ON_NULL(str, "c string is null pointer");
    
    char *match = strstr(sv.data, str);
    if (!match) {
        SV_LOG("There is no matching substring");
        return -1;
    } 
    return sv_find_char(sv, match[0], pos);
}

int sv_starts_with(strview_t sv, strview_t prefix)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(prefix.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(prefix.len > sv.len, "prefix string length is too long");

    for (size_t i = 0; i < prefix.len; i++) 
            if (*(sv.data + i) != *(prefix.data + i))
                return -1;
    return 0;
}

int sv_ends_with(strview_t sv, strview_t suffix)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(suffix.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(suffix.len > sv.len, "suffix string length is too long");

    for (size_t i = suffix.len, j = 0; i > 0; i--, j++) 
            if (*(sv.data + sv.len - j) != *(suffix.data + i))
                return -1;
    return 0;
}

int sv_contains(strview_t sv, strview_t needle)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_NULL(needle.data, "string view *data points to null");

    char *s = strstr(sv.data, needle.data);
    if (!s)
        return -1;
    return 0;
}

int sv_trim(strview_t sv, char *buf, size_t buflen)
{
    SV_RET_ERR_ON_NULL(buf, "buffer is null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(buflen < sv.len, "insufficient buffer room");

    size_t j = 0;
    for (size_t i = 0; i < sv.len; i++) {
        if (*(sv.data + i) == ' ') 
            continue;
        buf[j++] = *(sv.data + i);
    }
    buf[j] = '\0';

    return 0;
}

int sv_to_u64(strview_t sv, uint64_t *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(sv.data[0] == '-', "invalid symbol for u64");

    size_t i = 0;
    if (sv.data[i] == '+') {
        i++;
        SV_RET_ERR_ON_TRUE(sv.len <= 1, "invalid number as string");
    }
    
    uint64_t res = 0, digit;

    for ( ; i < sv.len; i++) {
        SV_RET_ERR_ON_TRUE((sv.data[i] < '0' || sv.data[i] > '9'), "invalid range");
        /* *
         * Convert an ASCII digit character into its integer digit value.
         * '0' = 48, '9' = 57. Operate on ascii values.
         */
        digit = (uint64_t) (sv.data[i] - '0');
        SV_RET_ERR_ON_TRUE((res > (UINT64_MAX - digit)/10), "invalid range");
        res = res * 10 + digit;
    }

    *val = res;

    return 0;
}

int sv_to_i64(strview_t sv, int64_t *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");

    size_t i = 0;
    if (sv.data[i] == '+' || sv.data[i] == '-') {
        i++;
        SV_RET_ERR_ON_TRUE(sv.len <= 1, "invalid number as string");
    }

    int is_negative = (sv.data[0] == '-');

    /* *
     * Negative range of int64_t has one more representable magnitude
     * than the positive range.
     * INT64_MAX =  9223372036854775807
     * INT64_MIN = -9223372036854775808
     */
    uint64_t limit = is_negative ? (uint64_t) INT64_MAX + 1 : (uint64_t) INT64_MAX;
    
    uint64_t res = 0, digit;

    for ( ; i < sv.len; i++) {
        SV_RET_ERR_ON_TRUE((sv.data[i] < '0' || sv.data[i] > '9'), "invalid range");
        /* *
         * Convert an ASCII digit character into its integer digit value.
         * '0' = 48, '9' = 57. Operate on ascii values.
         */
        digit = (uint64_t) (sv.data[i] - '0');
        SV_RET_ERR_ON_TRUE((res > (limit - digit)/10), "invalid range");
        res = res * 10 + digit;
    }
    
    /* Add - sign later */
    if (is_negative) {
        *val = (res == (uint64_t) INT64_MAX + 1) ? INT64_MIN : -(int64_t)res;
    } else {
        *val = (int64_t) res;
    }

    return 0;
}


int sv_to_u32(strview_t sv, uint32_t *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(sv.data[0] == '-', "invalid symbol for u32");

    size_t i = 0;
    if (sv.data[i] == '+') {
        i++;
        SV_RET_ERR_ON_TRUE(sv.len <= 1, "invalid number as string");
    }
    
    uint64_t res = 0, digit;

    for ( ; i < sv.len; i++) {
        SV_RET_ERR_ON_TRUE((sv.data[i] < '0' || sv.data[i] > '9'), "invalid range");
        /* *
         * Convert an ASCII digit character into its integer digit value.
         * '0' = 48, '9' = 57. Operate on ascii values.
         */
        digit = (uint32_t) (sv.data[i] - '0');
        SV_RET_ERR_ON_TRUE((res > (UINT32_MAX - digit)/10), "invalid range");
        res = res * 10 + digit;
    }

    *val = res;

    return 0;
}

int sv_to_i32(strview_t sv, int32_t *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");

    size_t i = 0;
    if (sv.data[i] == '+' || sv.data[i] == '-') {
        i++;
        SV_RET_ERR_ON_TRUE(sv.len <= 1, "invalid number as string");
    }

    int is_negative = (sv.data[0] == '-');

    /* *
     * Negative range of int32_t has one more representable magnitude
     * than the positive range.
     * INT32_MAX =  2147483647
     * INT32_MIN = -2147483648
     */
    uint32_t limit = is_negative ? (uint32_t) INT32_MAX + 1 : (uint32_t) INT32_MAX;
    
    uint32_t res = 0, digit;

    for ( ; i < sv.len; i++) {
        SV_RET_ERR_ON_TRUE((sv.data[i] < '0' || sv.data[i] > '9'), "invalid range");
        /* *
         * Convert an ASCII digit character into its integer digit value.
         * '0' = 48, '9' = 57. Operate on ascii values.
         */
        digit = (uint32_t) (sv.data[i] - '0');
        SV_RET_ERR_ON_TRUE((res > (limit - digit)/10), "invalid range");
        res = res * 10 + digit;
    }
    
    /* Add - sign later */
    if (is_negative) {
        *val = (res == (uint32_t) INT32_MAX + 1) ? INT32_MIN : -(int32_t)res;
    } else {
        *val = (int32_t) res;
    }

    return 0;
}

static int is_float_point(strview_t sv)
{
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");

    size_t i = 0;
    if (sv.data[i] == '+' || sv.data[i] == '-') {
        i++;
        SV_RET_ERR_ON_TRUE(sv.len <= 1, "invalid float point number as string");
    }

    for ( ; i < sv.len; i++) {
        char c = sv.data[i];
        if (c >= '0' && c <= '9')
            continue;
        if (c == '.')
            continue;
        if (c == 'e'|| c == 'E') {
            if (i + 1 < sv.len) {
                if (sv.data[i + 1] == '+' || sv.data[i + 1] == '-') {
                    i++;
                }
            }
            continue;
        }
        fprintf(stderr, "FAIL: invalid character for float point number\n");
        return 0;
    }

    return 1;
}

int sv_to_double(strview_t sv, double *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(!is_float_point(sv), "invalid number as string");

    char *buf = malloc(sv.len + 1);
    SV_RET_ERR_ON_NULL(buf, "could not allocate temporary memory space");

    memcpy(buf, sv.data, sv.len);
    buf[sv.len] = '\0';

    char *end = NULL;
    double res = strtod(buf, &end);
    if (errno == ERANGE || end != buf + sv.len) {
        free(buf);
        fprintf(stderr, "strtod() failed\n");
        return -1;
    }
    free(buf);
    *val = res;

    return 0;
}

int sv_to_float(strview_t sv, float *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(!is_float_point(sv), "invalid number as string");

    char *buf = malloc(sv.len + 1);
    SV_RET_ERR_ON_NULL(buf, "could not allocate temporary memory space");

    memcpy(buf, sv.data, sv.len);
    buf[sv.len] = '\0';

    char *end = NULL;
    float res = strtof(buf, &end);
    if (errno == ERANGE || end != buf + sv.len) {
        free(buf);
        fprintf(stderr, "strtof() failed\n");
        return -1;
    }
    free(buf);
    *val = res;

    return 0;
}

int sv_to_int(strview_t sv, int *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");

    /* int is commonly 4B, but not guaranteed */
    int64_t tmp;
    SV_RET_ERR_ON_TRUE(sv_to_i64(sv, &tmp) < 0, "invalid int as string");

    SV_RET_ERR_ON_TRUE((tmp < INT_MIN || tmp > INT_MAX), "out of range");
    *val = (int) tmp;

    return 0;
}

int sv_to_long(strview_t sv, long *val)
{
    SV_RET_ERR_ON_NULL(val, "null pointer");
    SV_RET_ERR_ON_NULL(sv.data, "string view *data points to null");

    /* long can be 4B or 8B on some systems, but at most 8B */
    int64_t tmp;
    SV_RET_ERR_ON_TRUE(sv_to_i64(sv, &tmp) < 0, "invalid int as string");

    SV_RET_ERR_ON_TRUE((tmp < LONG_MIN || tmp > LONG_MAX), "out of range");
    *val = (long) tmp;

    return 0;
}

inline void sv_print_safe_stdout(strview_t sv)
{
    if (!sv.data) {
        SV_LOGE("String view *data is null pointer");
        return;
    }
    printf("%.*s", (int) sv.len, sv.data);
}

inline void sv_println_safe_stdout(strview_t sv)
{
    if (!sv.data) {
        SV_LOGE("String view *data is null pointer");
        return;
    }
    printf("%.*s\n", (int) sv.len, sv.data);
}
