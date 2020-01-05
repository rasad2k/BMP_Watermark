# BMP_Watermark
To compile a program use gcc main.c -o wm -lm command in terminal.  
To run a program, user must give valid BMP file and color. Also, if both -date and -text are not
available, program will output the exact input image. Note that user must give the input file name
right after the script name, as shown in the example:  
./wm sign.bmp -color FFFFAF -text "Hello world"  
This example firstly asks for output filename to write and then it returns an image with written
"Hello world" in Morse code, starting from position (0, 0).  
Command line options:  
-color: User chooses a color to write on the image. It is better to choose the color that is close
to the background color, for example if the image with black background is watermarked, color
close to black should be chosen, for example, #000001. This option works only with RGB colors,
therefore user must give color with 6 characters (2 characters for each color in RGB), it can be
both in capital letters and lowercase letters. Note that this options do not have default value, hence
they are obligatory. Sample color inputs:  
-color 12EC3A  
-color AAC34B  
-text: User chooses the text to write on an image. Note that only English alphabet is available.
Input text can be both with and without quotation marks. Maximum number of characters available
for the text is 400. This option is not obligatory, therefore it can be missed. Sample text inputs:  
-text "Hello world"  
-text salam  
-pos: User chooses the position to write on an image. First value is X and second value is Y,
also they have to be separated with space. Note that X must be less than the image width and Y
must be less than the image height. This option is optional and if it is not given by the user, its
default value is (0, 0). Sample position inputs:  
-pos 25 35  
-date: Output of Linux date command is written on the image. If the text is available, date is
written after the image and they are separated with space. Note that this command is optional.  
-o: User chooses output filename. If it is not available, the program asks for an output filename
while running. Note that this is optional. Sample -o usage:  
-o mod.bmp  
