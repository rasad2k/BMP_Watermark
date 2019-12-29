#ifndef __BMP_H__
#define __BMP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

unsigned char * readInfoHeader(FILE * fp);
int getHeaderSize(unsigned char * header);
int bytesPerPixel(unsigned char * header);
int getImageSize(unsigned char * header);
unsigned char * readHeader(FILE * fp, int headerSize);
void printImage(unsigned char * image, int size);
void printHeader(unsigned char * header, int headerSize);
unsigned char * readImageHex(FILE * fp, int size);
unsigned char * readImageString(unsigned char * image, int size);
unsigned char * watermark(unsigned char * image, unsigned char * header, unsigned char * color, int posX, int posY, char * text);

#endif