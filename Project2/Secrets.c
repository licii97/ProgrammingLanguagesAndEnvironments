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
    		if (ch == capitalLetters[i]) ch=capitalLetters[((i+key)%26)];
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
    		if (ch == capitalLetters[i]) ch=capitalLetters[(i-key)%26];
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

    			ch=capitalLetters[((i-key)%26)];
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

    //fill up 
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
void append_ZeroByte (FILE *input, FILE *target){
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
                    append_ZeroByte(source1, target);

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
