#include "bmp.c"

char * getX(char * arg)
{
	char * posX = (char *)malloc(10 * sizeof(char));
	for (int i = 0; i < strlen(arg); i++){
		if (arg[i] == ','){
			posX[i] = '\0';
			return posX;
		}
		posX[i] = arg[i];
	}
}

char * getY(char * arg)
{
	char * posY = (char *)malloc(10 * sizeof(char));
	int n = 0;	
	for (int i = 0; i < strlen(arg); ++i){
		n++;
		if (arg[i] == ','){
			break;
		}
	}
	for (int i = 0; i < strlen(arg); ++i)
	{
		if (arg[n] == '\0'){
			posY[i] = '\0';
			return posY;
			}
		posY[i] = arg[n++];
	}
}

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
	posX = "0";
	posY = "0";
	int n = 2;
	int i = n + 1;
	while (n < argc){
		if (strcmp(argv[n], "-text") == 0){
			while (*argv[i] != '-'){
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
			posX = concat(posX, getX(argv[n + 1]));
			posY = concat(posY, getY(argv[n + 1]));
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
	char buf[100];
	if (strcmp(output, "") == 0){
		printf("Enter output file name: ");
		scanf("%s", buf);
	}
	output = concat(output, buf);
	FILE * fp = fopen(filename, "rb");
	char * infoHeader = readInfoHeader(fp);
	int headerSize = getHeaderSize(infoHeader);
	rewind(fp);
	unsigned char * header = readHeader(fp, headerSize);
	int size = getImageSize(header);
	rewind(fp);
	unsigned char * image = readImageHex(fp, size);
	unsigned char * result = (unsigned char *)malloc((size + 1) * sizeof(char));
	unsigned char * str = (unsigned char *)malloc(500 * sizeof(char));
	str = "";
	str = concat(str, text);
	str = concat(str, " ");
	str = concat(str, date);
	result = watermark(image, header, color, strToInt(posX), strToInt(posY), str);
	FILE * fw = fopen(output, "wb");	
	fwrite(result, 1, size, fw);
	fclose(fw);
	free(result);
	free(header);
	fclose(fp);
	return 0;
}
