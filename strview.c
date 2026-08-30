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

int sv_rem_suffix(strview_t *sv, size_t start_pos)
{
    SV_RET_ERR_ON_NULL(sv->data, "string view *data points to null");
    SV_RET_ERR_ON_TRUE(start_pos > sv->len, "invalid suffix starting position");
    sv->data = sv->data;
    sv->len = start_pos;
    return 0;
}

inline void sv_print_safe_stdout(strview_t sv)
{
    printf("%.*s", (int) sv.len, sv.data);
}

inline void sv_println_safe_stdout(strview_t sv)
{
    printf("%.*s\n", (int) sv.len, sv.data);
}
