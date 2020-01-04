#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int strToInt (char * str);
unsigned char * morseWord(unsigned char * text);
void hexToStr(unsigned char* input,unsigned char* output, int size);
int hexToInt(unsigned char * hex);
unsigned char *concat(unsigned char * str1, unsigned char * str2);
unsigned char * convertEndian(unsigned char * string);
unsigned char * stringToHex(unsigned char * string);

#endif