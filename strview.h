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

#ifndef _STRVIEW_H_
#define _STRVIEW_H_

#include <string.h>

/**
 * Main data structure representing string of chars.
 * String view does not allocate memory to build string,
 * but it is a pointer to existing char* in memory.
 */
typedef struct {
    const char *data;
    size_t len;
} strview_t;

strview_t sv_from_cstr(const char *s);
strview_t sv_from_cstr_len(const char *s, size_t len);

int sv_to_cstr(strview_t sv, char *buf, size_t buflen);

size_t sv_len(strview_t sv);
int sv_empty(strview_t sv);

int sv_substr(strview_t sv, size_t pos, size_t len, strview_t *out);
int sv_prefix(strview_t sv, size_t end_pos, strview_t *out);
int sv_suffix(strview_t sv, size_t start_pos, strview_t *out);

int sv_rem_prefix(strview_t *sv, size_t end_pos);
int sv_rem_suffix(strview_t *sv, size_t start_pos);
int sv_rem_prefix_save(strview_t *sv, size_t end_pos, strview_t *save);
int sv_rem_suffix_save(strview_t *sv, size_t start_pos, strview_t *save);

int sv_cmp(strview_t a, strview_t b);

int sv_find_char(strview_t sv, char c, size_t *pos);
int sv_rfind_char(strview_t sv, char c, size_t *pos);
int sv_find_cstr(strview_t sv, const char *str, size_t *pos);

void sv_print_safe_stdout(strview_t sv);
void sv_println_safe_stdout(strview_t sv);

#endif
