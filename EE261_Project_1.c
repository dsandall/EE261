// EE261 Progressive Programming Project
// Cal Poly, SLO
// Starter code
#include <stdio.h>
#include <stdlib.h>
#define FILENAME "APRSIS_DATA.txt"


/*
// read a line from the file, ignoring the terminating 0x0a
// place characters read into specified array
// return number of characters read
// return -1 if EOF
size_t readLine(FILE *f, char *lineArray)
{
   char readChar;
   char count = 0;
   // read the first character
   readChar = getc(f);
   // continue until EOF
   while (readChar != EOF)
   {
      if (readChar == 0x0a)
      {
         // we have reached the end of the line
         *lineArray++ = 0; // null terminate the string
         return (count);   // return the count
      }
      *lineArray++ = readChar; // store the character
      count++;                 // increment the count
      readChar = getc(f);      // get the next character
   }
   if (readChar == EOF)
      return (-1); // return a -1 if we reached the end of the file
}

*/

int main()
{
   int c;      // character read
   FILE *fptr; // file pointer

   // open the file
   if ((fptr = fopen(FILENAME, "r")) == NULL)
   {
      printf("Error! opening file");

      // Program exits if the file pointer returns NULL.
      exit(1);
   }

   // get the first character
   c = getc(fptr);

   // keep going until the end of the file
   while (c != EOF)
   {
      putchar(c);     // print the character on the screen
      c = getc(fptr); // get the next character
   }

   // Close the file and exit
   fclose(fptr);
   return 0;
}