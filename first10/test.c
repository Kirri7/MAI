#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "number-systems.c"

int validateCh(char ch) {
    if ('0' <= ch && ch <= '0' + 9) {
        return 1;
    }
    if ('a' <= ch && ch <= 'a' + 25) {
        return 1;
    }
    if ('A' <= ch && ch <= 'A' + 25) {
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    char ch = ' ';
    printf("%d\n", validateCh(ch));

    return 0;
}