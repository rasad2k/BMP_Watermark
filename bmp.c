#include "bmp.h"
#include "convert.c"

//reads file header from given file as a hexadecimal string
unsigned char * readHeaderHex(FILE * fp)
{
	unsigned char * header = (unsigned char *)malloc(54 * sizeof(char));
	fread(header, 1, 54, fp);
	return header;	
}

//converts hexadecimal string header to regular string header
unsigned char * readHeaderString(unsigned char * header)
{

	unsigned char * headerString = (unsigned char *)malloc(109 * sizeof(char));
	string2hexString(header, headerString, 54);
	return headerString;
}

//reads image as hexadecimal string
unsigned char * readImageHex(FILE * fp, int size)
{
	unsigned char * image = (unsigned char *)malloc(size * sizeof(char));
	fread(image, 1, size, fp);
	return image;
}

//converts hexadecimal string image to regular string image
unsigned char * readImageString(unsigned char * image, int size)
{
	unsigned char * imageString = (unsigned char *)malloc((size * 2 + 1) * sizeof(char));
	string2hexString(image, imageString, size);
	return imageString;
}

//gets image width from given header
int getWidth(unsigned char * header)
{
	int size = 0;
	unsigned char * charWidth = (unsigned char *)malloc(9 * sizeof(char));
	for (int i = 0; i < 8; i += 2)
	{
		charWidth[i] = header[i + 36];
		charWidth[i + 1] = header[i + 37];
	}
 	charWidth = convertEndian(charWidth);
 	int width = hexToInt(charWidth);
 	return width;
}

//gets image height from given header
int getHeight(unsigned char * header)
{
	int size = 0;
	unsigned char * charHeight = (unsigned char *)malloc(9 * sizeof(char));

	for (int i = 0; i < 8; i += 2)
	{
		charHeight[i] = header[i + 44];
		charHeight[i + 1] = header[i + 45];
 	}
 	charHeight = convertEndian(charHeight);
 	int height = hexToInt(charHeight);
 	return height;
}

//gets image size from given header
int getImageSize(unsigned char * header)
{
	int height = getHeight(header);
	int width = getWidth(header);
 	return width * height * 3;
}

//prints hexadecimal values of regular string image
void printImage(unsigned char * image, int size)
{
	int k = 0;
	for (int i = 0; i < 2 * size; i+=2)
	{
		printf("[%d]: %c%c\n", k, image[i], image[i+1]);
		k++;
	}	
}

//prints hexadecimal values of regular string header
void printHeader(unsigned char * header, int size)
{
	int j = 0;
	for (int i = 0; i < 108; i+=2)
	{
		printf("[%d]: %c%c\n", j, header[i], header[i+1]);
		j++;
	}
}

char * getDate()
{
	char * res = (char *)malloc(100 * sizeof(char));
	FILE * fp = popen("date", "r");
	fgets(res, 100, fp);
	res[strlen(res) - 1] = '\0';
	pclose(fp);
	return res;
}

//watermarks the given image with the morse code of the given text,
//with the given color on the given position x, y 
unsigned char * watermark(unsigned char * image, unsigned char * header, unsigned char * color, int posX, int posY, char * text)
{
	unsigned char * littleColor = convertEndian(color); //convert Big-endian color to Little-endian color
	int index = ((getHeight(header) - posY - 1) * getWidth(header) + posX) * 3 + 54; // gets index of the the pixel on the position posX, posY
	unsigned char * morse = morseWord(text); //gets morse code of the text
	int size = 10 * strlen(morse); 
	unsigned char * tmp = (unsigned char *)malloc(size * sizeof(char)); //creates tmp string to store values of image from index up to index+size
	for (int i = 0; i < size; ++i){
		tmp[i] = image[index + i]; //fills tmp with the values of the image
	}

	unsigned char * tmp1 = (unsigned char *)malloc(2 * size * sizeof(char)); 
	string2hexString(tmp, tmp1, size); //converts hexadecimal string tmp to regular string tmp1
	int tmpIndex = 0;
	for (; *morse != '\0'; morse++)
	{
		switch (*morse)
		{
			case '.':
				for (int i = 0; i < 6; ++i)
				{
					tmp1[tmpIndex + i]= littleColor[i];
				}
				tmpIndex += 12; //to update index and to skip 1 backgroung pixel
				break;
			case '-':
				for (int i = 0; i < 3; ++i)
				{
					for (int j = 0; j < 6; ++j)
					{
						tmp1[tmpIndex + j] = littleColor[j];
					}
					tmpIndex += 6; //to update index
				}
				tmpIndex += 6; //to skip 1 background pixel
				break;
			case ' ':
				tmpIndex += 12; //to skip 3 backgroud pixel, 12 instead of 18 cause '.' or '-' already skips 1
				break;
			case '\n':
				tmpIndex += 24; //to skip 5 background pixel, 24 instead of 24 cause '.' or '-' already skips 1
				break;
			default: break;
		}
	}

	tmp = stringToHex(tmp1); //reconvert regular string tmp1 to hexadecimal string tmp


	for (int i = 0; i < size; ++i)
	{
		image[index + i] = tmp[i]; //changes the pixels of the image with the modified pixels
	}
	return image;
}