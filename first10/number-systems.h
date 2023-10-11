#ifndef NUMBER_SYSTEMS_H
#define NUMBER_SYSTEMS_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int defineBase(const char* str);

int fromDecimal(int decimal, int base, char** res);

int handmadeStrtoi(const char* str, char** endptr, int base, int* ovrflw);

int toDecimal(const char* str, int base, int* res);

#endif