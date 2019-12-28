#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int getImageSize(unsigned char * header);
unsigned char * readHeaderHex(FILE * fp);
unsigned char * readHeaderString(unsigned char * header);
void printImage(unsigned char * image, int size);
void printHeader(unsigned char * header, int size);
unsigned char * readImageHex(FILE * fp, int size);
unsigned char * readImageString(unsigned char * image, int size);
unsigned char * watermark(unsigned char * image, unsigned char * header, unsigned char * color, int posX, int posY, char * text);

#endif