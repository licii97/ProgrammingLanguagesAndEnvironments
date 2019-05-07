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


/* FUNCTIONS Int2 */


/* FUNCTIONS Pixel */


/* FUNCTIONS Image */


/* ENCRYPTION */

void copy_file(String input_filename, String output_filename)
{
    char ch;
    FILE *source, *target;

    source = fopen(input_filename, "r");

    target = fopen(output_filename, "r");

    while ((ch = fgetc(source)) != EOF)
      fputc(ch, target);

    fclose(input_filename);
    fclose(output_filename);
}

void cesar_encrypt(String input_filename, int key, String encrypted_filename)
{
}

void cesar_decrypt(String encrypted_filename, int key,
										String decrypted_filename)
{
}

void pi_encrypt(String input_filename, String pi_filename,
										String encrypted_filename)
{
}

void pi_decrypt(String encrypted_filename, String pi_filename,
											String decrypted_filename)
{
}

void pack_encrypt(String input_filename, String encrypted_filename)
{
}

void pack_decrypt(String encrypted_filename, String decrypted_filename)
{
}


/* STEGANOGRAPHY */

void dots_hide(String input_filename,
				String message_filename, String disguised_filename)
{
}

void dots_reveal(String disguised_filename, String decoded_filename)
{
}

Int2 crude_hide(Image img, Int2 n,
					String message_filename, Image result)
{
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
