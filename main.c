#include "bmp.c"

int main(int argc, char const *argv[])
{	
	FILE * fp = fopen("ss1.bmp", "rb");
	char * infoHeader = readInfoHeader(fp);
	int headerSize = getHeaderSize(infoHeader);
	rewind(fp);
	unsigned char * header = readHeader(fp, headerSize);
	int size = getImageSize(header);
	rewind(fp);
	unsigned char * image = readImageHex(fp, size);
	unsigned char * result = (unsigned char *)malloc((size + 1) * sizeof(char));
	result = watermark(image, header, "FF00FF", 10, 20, getDate());
	FILE * fw = fopen("mod.bmp", "wb");	
	fwrite(result, 1, size, fw);
	fclose(fw);
	free(result);
	free(header);
	fclose(fp);
	return 0;
}
