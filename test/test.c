/*
        Copyright (C) 2022 Pribess (Heewon Cho)
        test/test.c
*/

#include <ics/string.h>

#include <stdio.h>

int main(int argc, char *argv[]) {
    string a = icsfrom("Hello, ");
    string b = icsfrom("ics!");
    icsappend(&a, b);
    printf("testing icsappend...\n");
    printf("expected output : \"Hello, ics!\"\n");
    printf("output          : \"%s\"\n", a.buf);
}