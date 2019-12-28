#include "convert.h"
#include "morse.c"

// converts regular text to morse code
unsigned char * morseWord(unsigned char * text)
{
	int size = strlen(text); //size of original text;
	unsigned char * res = (char *)malloc((10 * size) * sizeof(char));
	for (; *text != '\0'; text++)
	{
		int index;
        if (*text == ' '){ //for texts with more than one word
            res = concat(res, "\n"); //if there is a new word, puts 'new line' to result
            continue;
        }

        if (*text <= '9'){ //if it's a number
            index = *text - '0';
            res = concat(res, num[index]);
            res = concat(res, " ");
            continue;
        }
		if (*text == ':'){
			index = 0;
			res = concat(res, symbol[index]);
			res = concat(res, " ");
			continue;
		} else if (*text == '+'){
			index = 1;
			res = concat(res, symbol[index]);
			res = concat(res, " ");
			continue;
		}

		if (*text < 97)
			index = *text - 'A'; //to get alphabetic index, we minus 'a' or 'A'
		else 
			index = *text - 'a';

		res = concat(res, alpha[index]); //concatanates result with morse code of letter
		res = concat(res, " "); //puts 'space' after each letter's morse code
	}
	return res;
}

//converts hexadecimal string to regular string
void string2hexString(unsigned char* input,unsigned char* output, int size)
{
    int i = 0; 
      
    for (int loop = 0; loop < size; loop++)
    {
        sprintf((output + i), "%02X", input[loop]);
        i += 2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}

//converts hexadecimal string to integer
int hexToInt(unsigned char * hex)
{
    int i = 0, val, len ,decimal;

    decimal = 0;

    len = strlen(hex);
    len--;
    for(i=0; hex[i]!='\0'; i++)
    {
 
        if(hex[i]>='0' && hex[i]<='9')
        {
            val = hex[i] - 48;
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10;
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }
    return decimal;
}

//concatanates 2 strings
unsigned char *concat(unsigned char * str1, unsigned char * str2)
{
	int size = strlen(str1) + strlen(str2) + 1;
	unsigned char * str = (char *)malloc(size * sizeof(char));
	for (int i = 0; i < strlen(str1); i++)
		*(str+i) = str1[i];
	for (int i = 0; i < strlen(str2); i++)
		*(str+strlen(str1)+i) = str2[i];
	return str;
}

//converts Big-endian  to Little-endian or vice-versa
unsigned char * convertEndian(unsigned char * string)
{
	int size = strlen(string);
	unsigned char * result = (unsigned char *)malloc((size + 1) * sizeof(char));
	for (; *string != '\0'; string += 2)
	{
		result[size - 2] = *string;
		result[size - 1] = *(string + 1);
		size -= 2;
	}
	return result;
}

//opposite of string2hexString function,
//converts regular string to hexadecimal string
unsigned char * stringToHex(unsigned char * string)
{
	unsigned char * result = (unsigned char *)malloc((strlen(string) / 2 + 1) * sizeof(char));
	unsigned char * tmp = (unsigned char *)malloc(10 * sizeof(char));
	int n, k = 0;
	for (int i = 0; i < strlen(string); i += 2)
	{
		tmp[0] = string[i];
		tmp[1] = string[i + 1];
		n = hexToInt(tmp);
		result[k] = (unsigned char)n;
		k++;
	}
	return result;
}