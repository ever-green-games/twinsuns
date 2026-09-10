
#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

typedef unsigned char uchar;

int sign(int x);
void strlower(char* str);
void strupper(char* str);
bool strmatch(char* cmd, char* input);
bool IsControlDown();

#endif
