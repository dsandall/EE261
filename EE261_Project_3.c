// EE261 Progressive Programming Project
// Cal Poly, SLO
// Starter code
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILENAME "APRSIS_DATA.txt"

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


const char id[3] = ":=";
char *terminatorLocation;
int i;

void parseLine(char *line){
   if (line[0] == '#'){return;}                                // don't parse comments

   char callsign[20];                                          // declare output line
   if (strstr(line, id) != NULL){                              // check for :=
      i++;                                                     // increment count

      terminatorLocation = strchr(line, '>');                  // find end of callsign
      int callsignLen = (int) (terminatorLocation - line);     // find length of callsign, pointer arithmetic
      strncpy(callsign, line, callsignLen);                    // copy first (callsign length) characters from line to callsign
      callsign[callsignLen] = '\0';                            // terminate callsign with nullchar

      printf("%-17s%d packet(s) processed\n", callsign, i);    // format and print!
   }
}

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

   char line[1000]; 
   //readLine(fptr, line); //call readline once, to start
   
   do {
      parseLine(line);
   } while(readLine(fptr, line) != -1);

   //parseLine(line);   // parse final line (skipped due to dowhile condition returning false for final line)

   // Close the file and exit
   fclose(fptr);
   return 0;
}