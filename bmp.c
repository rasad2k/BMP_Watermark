#include "bmp.h"
#include "convert.c"


//reads info header
unsigned char * readInfoHeader(FILE * fp)
{
	unsigned char * infoHeader = (unsigned char *)malloc(15 * sizeof(char));
	fread(infoHeader, 1, 14, fp);
	infoHeader[14] = '\0';
	unsigned char * res = (unsigned char *)malloc(29 * sizeof(char));
	hexToStr(infoHeader, res, 14); //converts hex string INFOHEADER to regular string
	return res;
}

//checks if the given file is BMP file
char checkFile(unsigned char * header)
{
	char tmp[4];
	for (int i = 0; i < 4; ++i)
	{
		tmp[i] = header[i];	//reads 2 hexadecimal bytes (4 characters in this case) starting from 0th byte
	}
	return strcmp(tmp, "424D"); //if the given file is starting with '42 4D', it means it's BMP file and returns 0, otherwise some '-' or '+' values
}

//gets the total size of headers, also known as offset
int getHeaderSize(unsigned char * header)
{
	char * headerSize = (char *)malloc(9 * sizeof(char));
	for (int i = 0; i < 8; i++)
	{
		headerSize[i] = header[i + 20]; //reads 4 hexadecimal bytes (8 characters in this case) starting from 10th (20/2) byte
	}
	headerSize[8] = '\0';
	headerSize = convertEndian(headerSize);
	int size = hexToInt(headerSize);
	return size;	
}

//gets how many bytes per pixel were used
int bitsPerPixel(unsigned char * header)
{
	unsigned char * bpp = (unsigned char *)malloc(5 * sizeof(char));
	for (int i = 0; i < 4; i++)
	{
		bpp[i] = header[i + 56]; //reads 2 hexadecimal bytes (4 characters in this case) starting from 28th (56/2) byte
	}
	bpp[4] = '\0';
	bpp = convertEndian(bpp);
	int res = hexToInt(bpp);
	return res;	
}

//reads header as a string
unsigned char * readHeader(FILE * fp, int headerSize)
{
	unsigned char * header = (unsigned char *)malloc((headerSize + 1) * sizeof(char));
	fread(header, 1, headerSize, fp);
	header[headerSize] = '\0';
	unsigned char * res = (unsigned char *)malloc((headerSize * 2 + 1) * sizeof(char));
	hexToStr(header, res, headerSize);
	return res;
}

//reads image as hexadecimal string
unsigned char * readImageHex(FILE * fp, int size)
{
	unsigned char * image = (unsigned char *)malloc(size * sizeof(char));
	fread(image, 1, size, fp);
	return image;
}

//gets image width from given header
int getWidth(unsigned char * header)
{
	unsigned char * charWidth = (unsigned char *)malloc(9 * sizeof(char));
	for (int i = 0; i < 8; i++)
	{
		charWidth[i] = header[i + 36]; //reads 4 hexadecimal bytes (8 characters in this case) starting from 18 (36/2) byte
	}
	charWidth[8] = '\0';
 	charWidth = convertEndian(charWidth);
 	int width = hexToInt(charWidth);
 	return width;
}

//gets image height from given header
int getHeight(unsigned char * header)
{
	unsigned char * charHeight = (unsigned char *)malloc(9 * sizeof(char));

	for (int i = 0; i < 8; i++)
	{
		charHeight[i] = header[i + 44]; //reads 4 hexadecimal bytes (8 characters in this case) starting from 22th (44/2) byte
 	}
 	charHeight[8] = '\0';
 	charHeight = convertEndian(charHeight);
 	int height = hexToInt(charHeight);
 	return height;
}

//gets image size from given header
int getImageSize(unsigned char * header)
{
	unsigned char * imageSize = (unsigned char *)malloc(9 * sizeof(char));
	for (int i = 0; i < 8; i++)
	{
		imageSize[i] = header[i + 4]; //reads 4 hexadecimal bytes (8 characters in this case) starting from 2 (4/2) byte
	}
	imageSize[8] = '\0';
	imageSize = convertEndian(imageSize);
	int size = hexToInt(imageSize);
	return size;
}

//prints hexadecimal values of regular string image
void printImage(unsigned char * image, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("[%d]: %02X\n", i, image[i]);
	}	
}

//prints hexadecimal values of regular string header
void printHeader(unsigned char * header, int headerSize)
{
	int j = 0;
	for (int i = 0; i < headerSize; i+=2)
	{
		printf("[%d]: %c%c\n", j, header[i], header[i+1]);
		j++;
	}
}

//gets the output of linux command 'date'
char * getDate()
{
	char * res = (char *)malloc(100 * sizeof(char));
	FILE * fp = popen("date", "r");
	fgets(res, 100, fp);
	res[strlen(res) - 1] = '\0';
	pclose(fp);
	return res;
}

int getReservedSize(unsigned char * morse)
{
	int n = 0;
	for (int i = 0; morse[i] != '\0' ; i++)
	{
		if(morse[i] == '.'){
			if(morse[i + 1] == '-' || morse[i + 1] == '.'){
				n += 2;
				continue;
			}
			if(morse[i + 1] == ' '){
				n += 4;
				continue;
			}
			if(morse[i + 1] == '\n'){
				n += 6;
				continue;
			}
			if(morse[i + 1] == '\0'){
				n++;
				continue;
			}
		}
		else if(morse[i] == '-'){
			if(morse[i + 1] == '-' || morse[i + 1] == '.'){
				n += 4;
				continue;
			}
			if(morse[i + 1] == ' '){
				n += 6;
				continue;
			}
			if(morse[i + 1] == '\n'){
				n += 8;
				continue;
			}
			if(morse[i + 1] == '\0'){
				n += 3;
				continue;
			}
		}
	}
	return n;
}

//watermarks the given image with the morse code of the given text,
//with the given color on the given position x, y 
unsigned char * watermark(unsigned char * image, unsigned char * header, unsigned char * color, int posX, int posY, char * text)
{
	int bpp = bitsPerPixel(header); //gets the value of bits per pixel
	int gap;
	int byteNumber = bpp / 8;
	if( byteNumber == 3) //checks how many bytes per pixel image has
		gap = 0; //if 3, then it doesn't take transparency into the account, therefore gap is 0
	else 
		gap = 1; //else, if it has 4, then it takes transparency into the account, therefore gap is 0
	
	int width;	
	int headerSize = getHeaderSize(header);
	if(((getWidth(header) * byteNumber) % 4) != 0)
		width = getWidth(header) * byteNumber + (4 - (getWidth(header) * byteNumber) % 4);
	else 
		width = getWidth(header) * byteNumber;
	unsigned char * littleColor = convertEndian(color); //convert Big-endian color to Little-endian color
	int index = (getHeight(header) - posY - 1) * width + posX * byteNumber + headerSize; // gets index of the the pixel on the position posX, posY
	unsigned char * morse = (unsigned char *)malloc(20 * strlen(text) * sizeof(char));

	morse = morseWord(text); //gets morse code of the text
	if (morse == 0) //if text couldn't be translated to Morse code, returns "morseErr"
	{
		return "morseErr";
	}
	int size =  (getReservedSize(morse) + 2) * 3;
	if (index + size > getImageSize(header)){ //if text has to be written at the end of the image, return "sizeOverflow"
		return "sizeOverflow";
	}
	size *= 2;
	unsigned char * tmp = (unsigned char *)malloc(size * sizeof(char)); //creates tmp string to store values of image from index up to index+size
	for (int i = 0; i < size; ++i){
		tmp[i] = image[index + i]; //fills tmp with the values of the image
	}
	unsigned char * tmp1 = (unsigned char *)malloc(2 * size * sizeof(char)); 
	hexToStr(tmp, tmp1, size); //converts hexadecimal string tmp to regular string tmp1
	int tmpIndex = 0;
	for (; *morse != '\0'; morse++)
	{
		switch (*morse)
		{
			case '.':
			tmpIndex += gap * 2;
				for (int i = 0; i < 6; ++i)
				{
					tmp1[tmpIndex + i]= littleColor[i];
				}
				tmpIndex += (6 + gap) * 2; //to update index and to skip 1 backgroung pixel
				break;
			case '-':
				for (int i = 0; i < 3; ++i)
				{
					tmpIndex += gap * 2;
					for (int j = 0; j < 6; ++j)
					{
						tmp1[tmpIndex + j] = littleColor[j];
					}
					tmpIndex += 6; //to update index
				}
				tmpIndex += 6 + (gap * 2); //to skip 1 background pixel
				break;
			case ' ':
				tmpIndex += (6 + (gap * 2)) * 2; //to skip 3 backgroud pixel, 12 instead of 18 cause '.' or '-' already skips 1
				break;
			case '\n':
				tmpIndex += (6 + (gap * 2)) * 4; //to skip 5 background pixel, 24 instead of 24 cause '.' or '-' already skips 1
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