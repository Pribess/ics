/*
        Copyright (C) 2022 Pribess (Heewon Cho)
        test/test.c
*/

#include <ics/string.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("====================\n");
    string a = icsfrom("Hello, ");
    string b = icsfrom("ics!");
    icsappend(&a, b);
    printf("testing icsappend...\n");
    printf("expected output : \"Hello, ics!\"\n");
    printf("output          : \"%s\"\n", a.buf);

    icsfree(a);
    icsfree(b);

    printf("====================\n");
    string c = icsfrom("hello|foo|bar|baz");
    stringlist list = icssplit(c, "|");
    printf("testing icssplit...\n");
    printf("expected output : \n\"hello\"\n\"foo\"\n\"bar\"\n\"baz\"\n");
    printf("output          : \n");
    for (int cnt = 0 ; cnt < list.cnt ; cnt++) {
        printf("\"%s\"\n", list.list[cnt].buf);
    }

    icsfree(c);

    printf("====================\n");
    printf("testing icsjoin...\n");
    printf("expected output : \"hello|foo|bar|baz\"\n");
    string joined = icsjoin(list, "|");
    printf("output          : \"%s\"\n", joined.buf);

    icsfree(joined);
    icslistfree(list);

    printf("====================\n");
}