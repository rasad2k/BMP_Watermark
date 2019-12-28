#include "bmp.c"

int main(int argc, char const *argv[])
{	
	FILE * fp = fopen("sign.bmp", "rb");
	unsigned char * headerHex = readHeaderHex(fp);
	unsigned char * header = readHeaderString(headerHex);
	int size = getImageSize(header);
	rewind(fp);
	unsigned char * image = readImageHex(fp, size + 54);
	unsigned char * result = (unsigned char *)malloc((size +54) * sizeof(char));
	result = watermark(image, header, "000000", 10, 20, "");
	FILE * fw = fopen("modsign.bmp", "wb");
	fwrite(result, 1, size + 54, fw);

	free(result);
	free(header);
	printf("%s\n", getDate());
	return 0;
}