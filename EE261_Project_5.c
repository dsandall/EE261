// EE261 Progressive Programming Project
// Cal Poly, SLO
// Starter code
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define FILENAME "fortnite.txt"
#define CALLSIGN "N4VAL"

const char id[3] = ":=";
int numCallsignsDetected = 0;


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
Location previousLocation;

double dms2Rad(Coord *dms){
   double sum = (abs(dms->degrees) + ((double)dms->minutes)/60.0 + ((double)dms->seconds)/3600.0)/57.29577951;
   return dms->degrees >0 ? sum : -sum;
}

double getDistance(Location *loc1, Location *loc2){
   double lat1 = dms2Rad(&(loc1->Latitude));
   double lat2 = dms2Rad(&(loc2->Latitude));
   double long1 = dms2Rad(&(loc1->Longitude));
   double long2 = dms2Rad(&(loc2->Longitude));
   return 3963.0 * acos((sin(lat1) * sin(lat2)) + cos(lat1) *cos(lat2) * cos(long2 - long1));
}

char* parseCallsign(char *line, char *callsign){
   char *terminatorLocation = strchr(line, '>');              // find end of callsign
   int callsignLen = (int) (terminatorLocation - line);       // find length of callsign, pointer arithmetic
   strncpy(callsign, line, callsignLen);                      // copy first (callsign length) characters from line to callsign
   callsign[callsignLen] = '\0';                              // terminate callsign with nullchar
   return callsign;
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

   double dist = 0.0;
   if (strcmp(callsign, CALLSIGN)!=0){
      return; //ignore, wrong callsign
   }
   else{
      Location location = parseCoords(line);

      if (numCallsignsDetected != 0){
         //only runs if not initial contact, otherwise dist is 0
         dist = getDistance(&location, &previousLocation);
      }
      printf("Contact %d\n", numCallsignsDetected);
      printf("%-9smoved %.3lf miles\n", callsign, dist);
      printf("Past: ");
      printCoords(&previousLocation);
      printf("Curr: ");
      printCoords(&location);
      printf("--------------------------------\n");
      memcpy(&previousLocation, &location, sizeof(location)); //copy location to previous location
      numCallsignsDetected++;
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
   
   while(readLine(fptr, line) != -1){
      parseLine(line);
   }

   // Close the file and exit
   fclose(fptr);
   return 0;
}