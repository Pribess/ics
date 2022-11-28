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

struct icslist {
    size_t cap;
    size_t cnt;
    ics_t *list;
};

typedef struct icslist stringlist;
typedef struct icslist icslist_t;


size_t plen(const primitive *src);
primitive *pcpy(primitive *dst, const primitive *src);
primitive *pncpy(primitive *dst, const primitive *src, size_t size);
void *icsrealloc(void *dst, size_t size);

size_t bitceil(size_t value);


size_t plen(const primitive *src) {
    int len = 0;
    while (src[len++]);
    return len - 1;
}

primitive *pcpy(primitive *dst, const primitive *src) {
    return (primitive *)memcpy(dst, src, (plen(src) + 1) * sizeof(primitive));
}

primitive *pncpy(primitive *dst, const primitive *src, size_t size) {
    primitive *buf = memcpy(dst, src, size * sizeof(primitive));
    buf[size] = 0;
    return buf;
}

void *icsrealloc(void *dst, size_t size) {
    void *buf = realloc(dst, size);
    if (!buf) {
        printf("reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    return buf;
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

icslist_t icssplit(ics_t src, const primitive *delimiter);
ics_t icsjoin(icslist_t src, const primitive *delimiter);

icslist_t icsnewlist();
void icslistappend(icslist_t *dst, icslist_t src);
void icslistpush(icslist_t *dst, ics_t src);

size_t icslen(ics_t s);
int icscmp(ics_t a, ics_t b);

void icsfree(ics_t s);
void icslistfree(icslist_t s);


ics_t icsnew() {
    size_t size = BLOCK_SIZE;

    ics_t new;
    new.cap = size;
    new.len = 0;
    new.buf = malloc(size * sizeof(primitive));
    new.buf[0] = 0;
    return new;
}

ics_t icsfrom(const primitive *src) {
    size_t size = BLOCK_SIZE > plen(src) + 1 ? BLOCK_SIZE : bitceil(plen(src) + 1);

    ics_t new;
    new.cap = size;
    new.len = plen(src);
    new.buf = pcpy(malloc(size * sizeof(primitive)), src);
    return new;
}

ics_t icsdup(ics_t src) {
    return icsfrom(src.buf);
}

void icsappend(ics_t *dst, ics_t src) {
    size_t newlen = dst->len + src.len;
    size_t newcap = newlen + 1;
    if (newcap > dst->cap) {
        dst->buf = icsrealloc(dst->buf, bitceil(newcap) * sizeof(primitive));
    }
    pcpy(dst->buf + dst->len, src.buf);
    dst->cap = bitceil(newcap);
    dst->len = newlen;
}

icslist_t icssplit(ics_t src, const primitive *delimiter) {
    icslist_t list = icsnewlist();
    primitive *start = src.buf;
    for (int cnt = 0 ; cnt < plen(src.buf) ; cnt++) {
        if (src.buf[cnt] == delimiter[0]) {
            for (int idx = 0 ; plen(src.buf) - cnt > idx && idx <= plen(delimiter) ; idx++) {
                /* continue if delimiter does not match*/
                if (src.buf[cnt + idx] != delimiter[idx]) {
                    cnt += idx;
                    break;
                }
                /* create element if last character of delimiter matches */
                if (idx == plen(delimiter) - 1 && src.buf[cnt + idx] == delimiter[idx]) {
                    /* skip empty element */
                    if (cnt != 0 && src.buf - start + cnt != 0) {
                        ics_t element = icsnew();
                        /* the present value of start and cnt correspond to start of element and current index of the whole string */
                        element.len = src.buf - start + cnt;
                        element.cap = bitceil(element.len + 1);
                        element.buf = icsrealloc(element.buf, element.cap * sizeof(primitive));
                        pncpy(element.buf, start, element.len);
                        icslistpush(&list, element);
                    }
                    start = src.buf + cnt + idx + 1;
                    cnt += idx;
                    break;
                }
            }
        }
    }
    /* last element */
    if (start != src.buf + src.len) {
        ics_t element = icsnew();
        element.len = src.buf + src.len - start;
        element.cap = bitceil(element.len + 1);
        element.buf = icsrealloc(element.buf, element.cap * sizeof(primitive));
        pncpy(element.buf, start, element.len);
        icslistpush(&list, element);
    }

    return list;
}

ics_t icsjoin(icslist_t src, const primitive *delimiter) {
    ics_t new = icsnew();
    ics_t ds = icsfrom(delimiter);
    for (int cnt = 0 ; cnt < src.cnt ; cnt++) {
        icsappend(&new, src.list[cnt]);
        if (cnt != src.cnt - 1) {
            icsappend(&new, ds);
        }
    }
    icsfree(ds);
    return new;
}

icslist_t icsnewlist() {
    icslist_t list;
    list.cap = BLOCK_SIZE;
    list.cnt = 0;
    list.list = malloc(list.cap * sizeof(ics_t));
    return list;
}

void icslistappend(icslist_t *dst, icslist_t src){
    size_t newcnt = dst->cnt + src.cnt;
    size_t newcap = newcnt;
    if (newcap > dst->cap) {
        dst->list = icsrealloc(dst->list, bitceil(newcap) * sizeof(ics_t));
    }
    for (int cnt = 0 ; cnt < src.cnt ; cnt++) {
        dst->list[dst->cnt + cnt] = icsdup(src.list[cnt]);
    }
    dst->cap = bitceil(newcap);
    dst->cnt = newcnt;
}

void icslistpush(icslist_t *dst, ics_t src){
    size_t newcnt = dst->cnt + 1;
    size_t newcap = newcnt;
    if (newcap > dst->cap) {
        dst->list = icsrealloc(dst->list, bitceil(newcap) * sizeof(ics_t));
    }
    dst->list[dst->cnt] = icsdup(src);
    dst->cap = bitceil(newcap);
    dst->cnt = newcnt;
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

void icslistfree(icslist_t s) {
    for (int cnt = 0 ; cnt < s.cnt ; cnt++) {
        free(s.list[cnt].buf);
    }
    free(s.list);
}

#endif