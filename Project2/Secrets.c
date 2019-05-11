/*
max width = 80 colunas
tab = 4 spaces
01234567890123456789012345678901234567890123456789012345678901234567890123456789

LAP - AMD 2019
secrets.c

------
Student 1: 56554 Pascal Maximilian Engel
Student 2: 56601 Felicitas Schmelz

Comment:

?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????

*/

#include "Secrets.h"

/* FUNCTIONS String */


/* FUNCTIONS Byte */
typedef unsigned char Byte;

// shift left and insert a new least significant bit
#define shiftl(byte, newbit)    ( (byte) * 2 + (newbit) )

// most significant bit
#define msb(byte)               ( ((signed char)(byte)) < 0 ? 1 : 0 )


/* FUNCTIONS Int2 */


/* FUNCTIONS Pixel */

//additional function fpr crude_hide
extern Pixel pixel_change_green(Pixel p, char ch)
{
  p.green=(int) ch;
  return p;
}

//additional functions for image_hide
int hide_bit_in_colorvalue(int value, char bit)
{
    if ((bit == '0') && ((value % 2) == 0 )) return value;
    else if ((bit == '0') && ((value % 2) == 1 )) return (value - 1);
    else if ((bit == '1') && ((value % 2) == 0 )) return (value +1) ;
    else return value;
}

Pixel hide_3Zeros_in_Pixel(Pixel p)
{
    p.red = hide_bit_in_colorvalue(p.red, 0);
    p.green = hide_bit_in_colorvalue(p.green, 0);
    p.blue = hide_bit_in_colorvalue(p.blue, 0);
    return p;
}

//additional function for get image_reveal

char get_bit_from_colorvalue(int value){
    if ((value%2) == 1) return 1;
    else return 0;
}


/* FUNCTIONS Image */


/* ENCRYPTION */

void copy_file(String input_filename, String output_filename)
{
    char ch;
    FILE *source, *target;

    source = fopen(input_filename, "r");

    target = fopen(output_filename, "w");

    while ((ch = fgetc(source)) != EOF)
      fputc(ch, target);

    fclose(source);
    fclose(target);
}

void cesar_encrypt(String input_filename, int key, String encrypted_filename)
{
	char capitalLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char ch;
    FILE *source, *target;

    source = fopen(input_filename, "r");

    target = fopen(encrypted_filename, "w");

    while ((ch = fgetc(source)) != EOF){
    	for (int i = 0; i<26; i++){
    		if (ch == capitalLetters[i]) {
                ch=capitalLetters[((i+key)%26)];
                break;
            }
    	}

    	fputc(ch, target);
    }

    fclose(source);
    fclose(target);
}

void cesar_decrypt(String encrypted_filename, int key,
										String decrypted_filename)
{
	char capitalLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	char ch;
    FILE *source, *target;

    source = fopen(encrypted_filename, "r");

    target = fopen(decrypted_filename, "w");

    while ((ch = fgetc(source)) != EOF){
    	for (int i = 0; i<26; i++){
    		if (ch == capitalLetters[i]){
            if (key > i) ch=capitalLetters[(i-key)+26];
            else ch=capitalLetters[(i-key)];
            break;
            }
    	}

    	fputc(ch, target);
    }

    fclose(source);
    fclose(target);
}


void pi_encrypt(String input_filename, String pi_filename,
										String encrypted_filename)
{
	char capitalLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int key;

	char ch;
    FILE *source, *target, *pi;

    source = fopen(input_filename, "r");

    target = fopen(encrypted_filename, "w");

    pi = fopen(pi_filename, "r");

    //first two digits from pi file aren't used
    key = fgetc(pi);
    key = fgetc(pi);

    while ((ch = fgetc(source)) != EOF){
    	for (int i = 0; i<26; i++){
    		if (ch == capitalLetters[i]){
    			if ((key = fgetc(pi)) == EOF){
    				fclose(pi);
    				pi = fopen(pi_filename, "r");

    				//first two digits from pi file aren't used
    				key = fgetc(pi);
    				key = fgetc(pi);
    				//third digit is first one to use
    				key = fgetc(pi);
    			}
    			ch=capitalLetters[((i+key)%26)];
    		}
    	}

    	fputc(ch, target);
    }

    fclose(source);
    fclose(target);
    fclose(pi);
}

void pi_decrypt(String encrypted_filename, String pi_filename,
											String decrypted_filename)
{
	char capitalLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int key;

	char ch;
    FILE *source, *target, *pi;

    source = fopen(encrypted_filename, "r");

    target = fopen(decrypted_filename, "w");

    pi = fopen(pi_filename, "r");

    while ((ch = fgetc(source)) != EOF){
    	for (int i = 0; i<26; i++){
    		if (ch == capitalLetters[i]){
    			if ((key = fgetc(pi)) == EOF){
    				fclose(pi);
    				pi = fopen(pi_filename, "r");

    				//first two digits from pi file aren't used
    				key = fgetc(pi);
    				key = fgetc(pi);
    				//third digit is first one to use
    				key = fgetc(pi);
    			}

          if (key > i) ch=capitalLetters[(i-key)+26];
          else ch=capitalLetters[(i-key)];
          break;
    		}
    	}

    	fputc(ch, target);
    }

    fclose(source);
    fclose(target);
    fclose(pi);
}

void pack_encrypt(String input_filename, String encrypted_filename)
{
    int n = 0; //counter to skip every first bit of a byte

    char zero = 0;
    char ch;
    FILE *source, *target;

    source = fopen(input_filename, "r");

    target = fopen(encrypted_filename, "w");

    while ((ch = fgetc(source)) != EOF)
    {
        //every first bit of a byte is skipped
        if ((n%8)!=0) fputc(ch, target);
        n++;
    }

    /*shiftl(ch,0); -> hab keine erklärung der funktion im internet gefunden,
    aber ohne geht auch, halt umständlicher und ein bisschen hässlicher :D
    */

    //fill up wih 0 till last byte is completed
    int h1 = n/8; //number of skipped bits
    int h2 = (n-h1)%8; //number of bits that leftover for the last byte
    //add "0" until the number if bits can be divided by 8
    if (h2!=0)
    {
        while (h2 < 8){
            fputc(zero, target);
            h2++;
        }
    }


    fclose(source);
    fclose(target);
}


//TODO
void pack_decrypt(String encrypted_filename, String decrypted_filename)
{
    int n = 0; //counter to skip every first bit of a byte

    char ch;
    char str[8];
    FILE *source, *target;

    source = fopen(encrypted_filename, "r");

    target = fopen(decrypted_filename, "w");

    while ((ch = fgetc(source)) != EOF)
    {
        /*
        //add a zero and let 7 bits follow
        if (n==0)
        {
            str[n] = "0";
            n++;
        }

        if (n!=0)
        {
            str[n] = ch;
            n++;
        }

        if (n==8){
            fputc(str, target);
            n=0;
        }
        */

    }

    fclose(source);
    fclose(target);
}


/* STEGANOGRAPHY */
 //additional function to dots_hide
void append_ZeroByte_dots_hide (FILE *input, FILE *target){
    char ch;
    char next;

    //counter to get 8 Bits, when entering the function the first 0 bot is already set
    int n = 1;
    //so you need to skip all the " " to encode current full stop as a 0
    while ((next=fgetc(input))== ' '){}

    while ((ch=fgetc(input)) != EOF){
        fputc(ch, target);

        // you have to make sure, that the first 8 full stops are encoded as a 0
        if ((n < 8) && (ch == '.')){
            next = fgetc(input);
            fputc(next, target);

            if (next == ' ' ){
                // following " " are cut out from the input_file
                while ((next=fgetc(input))== ' '){}
                n++;
	    }
        }
    }

    //if input is ended, but there is not the full 0-Byte at the end it is an error
    if (n<8) {
        fclose(input);
        fclose(target);
        error("0-Byte does not fit in container", "blablawas auch immer hier rein soll");
    }

}

void dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
    char ch1;
    char ch2;
    char next;
    FILE *source1, *source2, *target;

    source1 = fopen(input_filename, "r");
    source2 = fopen(message_filename, "r");
    target = fopen(disguised_filename, "w");

    while ((ch1=fgetc(source1)) != EOF){
        fputc(ch1, target);

        if (ch1 == '.'){
            next = fgetc(source1);
            fputc(next, target);

            // if after "." there is " " then it is a full stop
            if (next == ' ' ){
                ch2 = fgetc(source2);

                //if message is ended, you need to append a 0-Byte
                if (ch2 == EOF){
                    fclose(source2); //you do not need message anymore

                    //call helping function to apennd 0-Byte
                    append_ZeroByte_dots_hide(source1, target);

                    //when appended the 0-Byte succcessfully, you just need to close the streams
                    fclose(source1);
                    fclose(target);
                    return;
                }

                //read next char in message and encode either as a 0 or a 1
                else if (ch2 == 0) {
                    // following " " are cut out from the input_file
                    while ((next=fgetc(source1))== ' '){}
                }
                else {
                    fputc(' ', target);
                    // following " " are cut out from the input_file
                    while ((next=fgetc(source1))== ' '){}
                }
            }
        }
    }

    fclose(source1);
    fclose(source2);
    fclose(target);

    if (ch2 != EOF) error("message does not fit in container", "blabla, was auch immer hier rein soll");
}

//TODO muss man das 0-Byte "00000000" am ende von der nachricht entfernen?
void dots_reveal(String disguised_filename, String decoded_filename)
{
    char ch;
    int n = 0; //counter to find end of the message
    FILE *source, *target;

    source = fopen(disguised_filename, "r");
    target = fopen(decoded_filename, "w");

    while ((ch = fgetc(source)) != EOF){
        if (ch=='.'){ //next characters are just read, if the one before is a dot
            char next = fgetc(source);
            char nextNext = fgetc(source);
            if(ch == '.' && next == ' ' && nextNext == ' '){
                fputc(1,target);
                n=0;
            }
            if(ch == '.' && next == ' '){
                fputc(0,target);
                n++;
                if (n==8) { //if it is the last bit of the end 0-byte, then end function
                    fclose(source);
                    fclose(target);
                    return ;
                }
            }
        }

    }

    fclose(source);
    fclose(target);
}

Int2 crude_hide(Image img, Int2 n,
					String message_filename, Image result)
{
  char ch;
  FILE *source;

  source = fopen(message_filename, "r");

  Int2 i;
  for(i.y = 0; i.y < n.y; i.y++)
  for(i.x = 0; i.x < n.x; i.x++) {
        result[i.x][i.y] = pixel_change_green(img[i.x][i.y],ch);
  }
  fclose(source);
  return n;
}

//TODO
void crude_reveal(Image img, Int2 n, String decoded_filename)
{
}



Int2 append_ZeroByte_image_hide(Image img, Int2 n, Image result, Int2 j, char current_color){
    //Int2 j is position in image, where message ended
    Int2 i = {0 , 0};
    int c = 0; //counter for 8 bits of the 0-Byte at the end

    //different cases at which color value of the pixel the message ended
    switch (current_color){
        case 'r':
            result[j.x][j.y] = hide_3Zeros_in_Pixel(img[i.x][i.y]);
            c=c+3;
            break;
        case 'g':
            result[j.x][j.y].green = hide_bit_in_colorvalue(img[i.x][i.y].green, 0);
            result[j.x][j.y].blue = hide_bit_in_colorvalue(img[i.x][i.y].blue, 0);
            c=c+2;
            break;
        case 'b':
            result[j.x][j.y].blue = hide_bit_in_colorvalue(img[i.x][i.y].blue, 0);
            c=c+1;
            break;
        }


    //next 8-c Bits have to be 0
    for(i.y = j.y; i.y < n.y; i.y++){
        for(i.x = j.x; i.x < n.x; i.x++) {
            //different cases of how many bits of the 0-byte are left
            switch ((8-c)){
                case 0:
                    break;
                case 1:
                    result[i.x][i.y].red = hide_bit_in_colorvalue(img[i.x][i.y].red, 0);
                    result[i.x][i.y].green = img[i.x][i.y].green;
                    result[i.x][i.y].blue = img[i.x][i.y].blue;
                    c=c+1;
                    break;
                case 2:
                    result[i.x][i.y].red = hide_bit_in_colorvalue(img[i.x][i.y].red, 0);
                    result[i.x][i.y].green = hide_bit_in_colorvalue(img[i.x][i.y].green, 0);
                    result[i.x][i.y].blue = img[i.x][i.y].blue;
                    c=c+2;
                    break;
                default:
                    result[i.x][i.y] = hide_3Zeros_in_Pixel(img[i.x][i.y]);
                    c=c+3;
                    break;
                }
                if (c==8) break;
            }
            if (c==8) break;
        }


    //if 0-Byte is successfully appended, rest of the img is just copied to result
    for(i.y = i.y; i.y < n.y; i.y++)
    for(i.x = i.x; i.x < n.x; i.x++) {
        result[i.x][i.y] = img[i.x][i.y];
    }

    //if img end before all 8 0-Bits are appended, there is an error
    if (c < 8) error("Message does not fit in image", "blablabla, whatever belongs here");
    return n;

}

Int2 image_hide(Image img, Int2 n,
					String message_filename, Image result)
{
    char ch;
    Int2 i;
    int c = 0; //counter for 8 bits of the 0-Byte at the end

    FILE *source;
    source = fopen(message_filename, "r");

    for(i.y = 0; i.y < n.y; i.y++)
    for(i.x = 0; i.x < n.x; i.x++) {
        //red part of pixel
        ch = fgetc(source);
        if (ch != EOF)
            result[i.x][i.y].red = hide_bit_in_colorvalue(img[i.x][i.y].red, ch);
        else {
            fclose(source);
            append_ZeroByte_image_hide(img, n, result, i, 'r');
        }

        //green part of pixel
        ch = fgetc(source);
        if (ch != EOF)
            result[i.x][i.y].green = hide_bit_in_colorvalue(img[i.x][i.y].green, ch);
        else {
            fclose(source);
            append_ZeroByte_image_hide(img, n, result, i, 'g');
        }

        //blue part of pixel
        ch = fgetc(source);
        if (ch != EOF)
            result[i.x][i.y].blue = hide_bit_in_colorvalue(img[i.x][i.y].blue, ch);
        else {
            fclose(source);
            append_ZeroByte_image_hide(img, n, result, i, 'b');
        }
    }

    fclose(source);

    //function gets here, when img is finished, but message or 0-Byte nor yet
    error("Message does not fit in image", "blablabla, whatever belongs here");
    return n;
}

//TODO do you have to delete 0-Byte at the end of the decoded file?
void image_reveal(Image img, Int2 n, String decoded_filename)
{
    char ch;
    Int2 i;
    int c = 0; //counter for 8 bits of the 0-Byte at the end

    FILE *target;
    target = fopen(decoded_filename, "w");

    for(i.y = 0; i.y < n.y; i.y++) {
        for(i.x = 0; i.x < n.x; i.x++) {

            //red part of pixel
            ch = get_bit_from_colorvalue(img[i.x][i.y].red);
            fputc (ch, target);
            if (ch == '0') c++;
            if (c == 8) break;

            //green part of pixel
            ch = get_bit_from_colorvalue(img[i.x][i.y].green);
            fputc (ch, target);
            if (ch == '0') c++;
            if (c == 8) break;

            //blue part of pixel
            ch = get_bit_from_colorvalue(img[i.x][i.y].blue);
            fputc (ch, target);
            if (ch == '0') c++;
            if (c == 8) break;
        }

        if (c == 8) break;
    }

    fclose(target);
}
