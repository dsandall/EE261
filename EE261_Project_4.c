// EE261 Progressive Programming Project
// Cal Poly, SLO
// Starter code
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define FILENAME "APRSIS_DATA.txt"

const char id[3] = ":=";

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

typedef struct dms
    {
    int degrees;
    int minutes;
    int seconds;
    } Coord;

typedef struct point
    {
    Coord Latitude;
    Coord Longitude;
    } Location;

void parseCallsign(char *line, char *callsign){
   char *terminatorLocation = strchr(line, '>');              // find end of callsign
   int callsignLen = (int) (terminatorLocation - line);       // find length of callsign, pointer arithmetic
   strncpy(callsign, line, callsignLen);                      // copy first (callsign length) characters from line to callsign
   callsign[callsignLen] = '\0';                              // terminate callsign with nullchar
}

Location parseCoords(char *line){
   Location location;
   char strong[10] = "\0";                         //placeholder string
   char *coordsLocation = strstr(line, id) +2;     // find beginning of coordinates, increment by 2 to skip id

   strncpy(strong, coordsLocation, 2);
   location.Latitude.degrees = (int) atoi(strong);
   coordsLocation += 2;

   strncpy(strong, coordsLocation, 2);
   location.Latitude.minutes = (int) atoi(strong);
   coordsLocation += 3;

   strncpy(strong, coordsLocation, 2);
   location.Latitude.seconds = (int) atoi(strong)*.6;
   coordsLocation += 2;

   //handle NS
   strncpy(strong, coordsLocation, 1);
   if (strong[0] == 'S'){location.Latitude.degrees = -location.Latitude.degrees;}
   coordsLocation += 2;


   //begin longitude
   strncpy(strong, coordsLocation, 3);
   location.Longitude.degrees = (int) atoi(strong);
   coordsLocation += 3;

   strncpy(strong, coordsLocation, 3);
   location.Longitude.minutes = (int) strtod(strong, NULL);
   coordsLocation += 3;

   strncpy(strong, coordsLocation, 2);
   location.Longitude.seconds = (int) atoi(strong)*.6;
   coordsLocation += 2;

   //handle WE
   strncpy(strong, coordsLocation, 1);
   if (strong[0] == 'W'){location.Longitude.degrees = -location.Longitude.degrees;}
   coordsLocation += 2;


   return location;
}

void printCoords(Location *l){
   printf("%-2d %-2d %-2d / ", l->Latitude.degrees, l->Latitude.minutes, l->Latitude.seconds);
   printf("%-3d %-2d %-2d\n", l->Longitude.degrees, l->Longitude.minutes, l->Longitude.seconds);
}

int lineCount; 
void parseLine(char *line){
   if (line[0] == '#'){return;}                                // don't parse comments
   if (strstr(line, id) == NULL){return;}                      // or lines without id

   char callsign[20];
   lineCount++;                                                
   parseCallsign(line, callsign);
   Location location = parseCoords(line);

   printf("%-10s ", callsign);
   printCoords(&location);
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
   
   while(readLine(fptr, line) != -1){
      parseLine(line);
   }

   // Close the file and exit
   fclose(fptr);
   return 0;
}