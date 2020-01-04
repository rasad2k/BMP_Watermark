#include "bmp.c"

int main(int argc, char *argv[])
{
	if (argc < 4){
		printf("Please read the manual and run the program correctly!\n");
		return 1;
	}
	char * filename = (char *)malloc(100 * sizeof(char));
	strcpy(filename, argv[1]);
	char * text = (char *)malloc(400 * sizeof(char));
	text = "";
	char * date = (char *)malloc(100 * sizeof(char));
	date = "";
	char * color = (char *)malloc(10 * sizeof(char));
	color = "";
	char * posX = (char *)malloc(10 * sizeof(char));
	char * posY = (char *)malloc(10 * sizeof(char));
	char * output = (char *)malloc(100 * sizeof(char));
	output = "";
	posX = "0"; //default value of posX 
	posY = "0"; //default value of posY
	int n = 2, i;
	while (n < argc){
		if (strcmp(argv[n], "-text") == 0){
			i = n + 1;
			while (i < argc && *argv[i] != '-'){
				text = concat(text, argv[i]);
				text = concat(text, " ");
				i++;
			}
			if (text[strlen(text) - 1] == ' '){
				text[strlen(text) - 1] = '\0';
			}
			n++;
			continue;
		}

		if (strcmp(argv[n], "-date") == 0){
			date = concat(date, getDate());
			n++;
			continue;
		}
		if (strcmp(argv[n], "-color") == 0){
			color = concat(color, argv[n + 1]);
			n++;
			continue;
		}
		if (strcmp(argv[n], "-pos") == 0){
			posX = "";
			posY = "";
			posX = concat(posX, argv[n + 1]);
			posY = concat(posY, argv[n + 2]);
			n++;
			continue;
		}
		if (strcmp(argv[n], "-o") == 0){
			output = concat(output, argv[n + 1]);
			n++;
			continue;
		}
		n++;
	}
	FILE * fp = fopen(filename, "rb");
	if(fp == NULL){
		printf("Couldn't open the given file, file might be invalid or you might not have access to open it.\n");
		return 1;
	}
	char * infoHeader = readInfoHeader(fp);
	if(checkFile(infoHeader)){
		printf("Entered file is either not BMP file or corrupted BMP file!\n");
		return 1;
	}

	int headerSize = getHeaderSize(infoHeader);
	rewind(fp);
	unsigned char * header = readHeader(fp, headerSize);


	if (strToInt(posX) > getWidth(header) || strToInt(posY) > getHeight(header)){
		printf("Entered positions isn't in the boundaries of image, please reduce input positions!\n");
		return 1;
	}


	int size = getImageSize(header);
	rewind(fp);
	unsigned char * image = readImageHex(fp, size);
	unsigned char * result = (unsigned char *)malloc((size + 1) * sizeof(char));
	unsigned char * str = (unsigned char *)malloc(500 * sizeof(char));
	str = "";
	str = concat(str, text);
	if (date != ""){
		str = concat(str, " ");
		str = concat(str, date);
	}
	result = watermark(image, header, color, strToInt(posX), strToInt(posY), str);
	if (strcmp(result, "sizeOverflow") == 0){
		printf("Not enough space to write in this positions, try reducing X or increasing Y!\n");
		return 1;
	}
	if (strcmp(result, "morseErr") == 0)
	{
		printf("Entered text or date cannot be translated into the Morse code, consider that only English alphabet is available for texts!\n");
		return 2;
	}

	char buf[100];
	if (strcmp(output, "") == 0){
		printf("Enter output file name: ");
		scanf("%s", buf);
	}
	output = concat(output, buf);

	FILE * fw = fopen(output, "wb");	
	fwrite(result, 1, size, fw);
	fclose(fw);
	free(result);
	free(header);
	fclose(fp);
	return 0;
}
