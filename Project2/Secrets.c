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

extern Pixel pixel_change_green(Pixel p, char ch)
{
  p.green=(int) ch;
  return p;
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

    fclose(input_filename);
    fclose(output_filename);
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
    		if (ch == capitalLetters[i]) ch=capitalLetters[((i+key)%26)];
    	}

    	fputc(ch, target);
    }

    fclose(input_filename);
    fclose(encrypted_filename);
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
    		if (ch == capitalLetters[i]) ch=capitalLetters[(i-key)%26];
    	}

    	fputc(ch, target);
    }

    fclose(encrypted_filename);
    fclose(decrypted_filename);
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
    				fclose(pi_filename);
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

    fclose(input_filename);
    fclose(encrypted_filename);
    fclose(pi_filename);
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
    				fclose(pi_filename);
    				pi = fopen(pi_filename, "r");

    				//first two digits from pi file aren't used
    				key = fgetc(pi);
    				key = fgetc(pi);
    				//third digit is first one to use
    				key = fgetc(pi);
    			}

    			ch=capitalLetters[((i-key)%26)];
    		}
    	}

    	fputc(ch, target);
    }

    fclose(encrypted_filename);
    fclose(decrypted_filename);
    fclose(pi_filename);
}

void pack_encrypt(String input_filename, String encrypted_filename)
{
    int n = 0; //counter to skip every first bit of a byte 

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

    //fill up 
    int h1 = n/8; //number of skipped bits
    int h2 = (n-h1)%8; //number of bits that leftover for the last byte
    //add "0" until the number if bits can be divided by 8 
    if (h2!=0)
    {
        while (h2 < 8){
            fputc("0", target);
            h2++;
        }
    }


    fclose(input_filename);
    fclose(encrypted_filename);
}

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

    fclose(encrypted_filename);
    fclose(decrypted_filename);
}


/* STEGANOGRAPHY */

void dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
}

void dots_reveal(String disguised_filename, String decoded_filename)
{
    char ch;
    FILE *source, *target;

    source = fopen(disguised_filename, "r");
    target = fopen(decoded_filename, "w");

    while ((ch = fgetc(source)) != EOF){
      char next = fgetc(source);
      char nextNext = fgetc(source);
      if(ch == '.' && next == ' ' && nextNext == ' ')
        fputc(1,target);
      if(ch == '.' && next == ' ')
        fputc(0,target);
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

void crude_reveal(Image img, Int2 n, String decoded_filename)
{
}


Int2 image_hide(Image img, Int2 n,
					String message_filename, Image result)
{
	return n;
}

void image_reveal(Image img, Int2 n, String decoded_filename)
{
}
