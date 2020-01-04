#include "convert.h"
#include "morse.c"

//converts string to int
int strToInt (char * str)
{
	int result = 0;
	for (int i = 0; i < strlen(str); i++){
		result *= 10;
		result += str[i] - '0';
	}
	return result;
}

// converts regular text to morse code
unsigned char * morseWord(unsigned char * text)
{
	int size = strlen(text); //size of original text
	unsigned char * res = (char *)malloc((10 * size) * sizeof(char));
	int index;
	for (; *text != '\0'; text++)
	{
        if (*text == ' '){ //for texts with more than one word
            res = concat(res, "\n"); //if there is a new word, puts 'new line' to result
            continue;
        }

        if (*text >= '0' && *text <= '9'){ //if it's a number
            index = *text - '0';
            res = concat(res, num[index]); //concatanates result with morse code of number
            if (*(text + 1) != ' ' && *(text + 1) != '\0')
	            res = concat(res, " "); //puts 'space' after each number's morse code
            continue;
        }
		if (*text == ':'){ //if it's a symbol
			index = 0;
			res = concat(res, symbol[index]); //concatanates result with morse code of symbol
			if (*(text + 1) != ' ' && *(text + 1) != '\0')
	        	res = concat(res, " "); //puts 'space' after each symbol's morse code
			continue;
		} else if (*text == '+'){
			index = 1;
			res = concat(res, symbol[index]); //concatanates result with morse code of symbol
			if (*(text + 1) != ' ' && *(text + 1) != '\0')
	        	res = concat(res, " "); //puts 'space' after each symbol's morse code
			continue;
		} else if (*text == '-'){
			index = 2;
			res = concat(res, symbol[index]); //concatanates result with morse code of symbol
			if (*(text + 1) != ' ' && *(text + 1) != '\0')
	        	res = concat(res, " "); //puts 'space' after each symbol's morse code
			continue;
		}

		if (*text >= 65 && *text <= 90){
			index = *text - 'A'; //to get alphabetic index, we minus 'a' or 'A'
			res = concat(res, alpha[index]); //concatanates result with morse code of letter
			if (*(text + 1) != ' ' && *(text + 1) != '\0')
	        	res = concat(res, " "); //puts 'space' after each letter's morse code
			continue;
		}
		else if (*text >= 97 && *text <= 122){
			index = *text - 'a';
			res = concat(res, alpha[index]); //concatanates result with morse code of letter
			if (*(text + 1) != ' ' && *(text + 1) != '\0')
	        	res = concat(res, " "); //puts 'space' after each letter's morse code
			continue;
		}
		return 0;
	}
	res[strlen(res)] = '\0';
	return res;
}

//converts hexadecimal string to regular string
void hexToStr(unsigned char* input,unsigned char* output, int size)
{
    int k = 0; 
      
    for (int i = 0; i < size; i++)
    {
        sprintf((output + k), "%02X", input[i]);
        k += 2;
    }
    //insert NULL at the end of the output string
    output[k++] = '\0';
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
            val = hex[i] - 48; //to get int value, we minus ASCII code of '0'
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10; //we minus ASCII code of 'a'
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10; //we minus ASCII code of 'A'
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
	unsigned char * str = (char *)malloc(size * sizeof(char)); //allocates space with the size of sum of 2 input strings
	for (int i = 0; i < strlen(str1); i++)
		*(str+i) = str1[i];
	for (int i = 0; i < strlen(str2); i++)
		*(str+strlen(str1)+i) = str2[i];
	str[size - 1] = '\0';
	return str;
}

//converts Big-endian  to Little-endian or vice-versa
unsigned char * convertEndian(unsigned char * string)
{
	int size = strlen(string);
	unsigned char * result = (unsigned char *)malloc((size + 1) * sizeof(char));
	int i = 0;
	for (; *string != '\0'; string += 2)
	{
		result[size - 2] = *string;
		result[size - 1] = *(string + 1);
		size -= 2;
		i += 2;
	}
	result[i] = '\0';
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
	result[k] = '\0';
	return result;
}