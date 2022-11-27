/*
        Copyright (C) 2022 Pribess (Heewon Cho)
        include/string.h
*/

#ifndef _STRING_
#define _STRING_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* default size of string */
#define BLOCK_SIZE 4
/* default character type */
typedef char primitive;

struct ics {
    size_t cap;
    size_t len;
    primitive *buf;
};

typedef struct ics string;
typedef struct ics ics_t;


size_t plen(const primitive *src);
void *pcpy(primitive *dst, const primitive *src);

size_t bitceil(size_t value);


size_t plen(const primitive *src) {
    int len = 0;
    while (src[len++]);
    return len - 1;
}

void *pcpy(primitive *dst, const primitive *src) {
    return memcpy(dst, src, plen(src) + 1);
}

int pcmp(const primitive *a, const primitive *b) {
    for (int cnt = 0 ; plen(a) + 1 >= cnt ; cnt++) {
        if (a[cnt] != b[cnt]) {
            return 1;
        }
    }
    return 0;
}

/* get smallest power of 2 greater than give value */
size_t bitceil(size_t value) {
    value--;
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value++;
    return value;
}


ics_t icsnew();
ics_t icsfrom(const primitive *src);
ics_t icsdup(ics_t src);

void icsappend(ics_t *dst, ics_t src);

size_t icslen(ics_t s);
int icscmp(ics_t a, ics_t b);

void icsfree(ics_t s);


ics_t icsnew() {
    size_t size = BLOCK_SIZE;

    ics_t new;
    new.cap = size;
    new.len = 0;
    new.buf = malloc(size);
    new.buf[0] = 0;
    return new;
}

ics_t icsfrom(const primitive *src) {
    size_t size = BLOCK_SIZE > plen(src) + 1 ? BLOCK_SIZE : bitceil(plen(src) + 1);

    ics_t new;
    new.cap = size;
    new.len = plen(src);
    new.buf = pcpy(malloc(size), src);
    return new;
}

ics_t icsdup(ics_t src) {
    return icsfrom(src.buf);
}

void icsappend(ics_t *dst, ics_t src) {
    size_t newlen = dst->len + src.len;
    size_t newcap = newlen + 1;
    if (newcap > dst->cap) {
        void *buf = realloc(dst->buf, bitceil(newcap));
        if (!buf) {
            printf("reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        dst->buf = buf;
    }
    pcpy(dst->buf + dst->len, src.buf);
    dst->cap = bitceil(newcap);
    dst->len = newlen;
}

size_t icslen(ics_t s) {
    return s.len;
}

int icscmp(ics_t a, ics_t b) {
    if (a.len != b.len) {
        return 1;
    }
    return pcmp(a.buf, b.buf);
}

void icsfree(ics_t s) {
    free(s.buf);
}

#endif