#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned char * morseWord(unsigned char * text);
unsigned char *concat(unsigned char * str1, unsigned char * str2);
void string2hexString(unsigned char* input,unsigned char* output, int size);
unsigned char * convertEndian(unsigned char * string);
int hexToInt(unsigned char * hex);
unsigned char * stringToHex(unsigned char * string);

#endif