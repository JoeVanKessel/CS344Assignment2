#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "structfi.h"
#include <dirent.h>
#include <sys/types.h>
#include <time.h>

char *roomsArr[10] = {"Home",
  "Breannah",
  "Prarie",
  "Store",
  "Mine",
  "Butcher",
  "Gunsmith",
  "Farm",
  "Mountian",
  "Spaceship"};

char *connectionList[6];


char *findNewestDir(){
  int newestDirTime = -1; // Modified timestamp of newest subdir examined
   char targetDirPrefix[32] = "vankessj.rooms."; // Prefix we're looking for
   char *newestDirName; // Holds the name of the newest dir that contains prefix
   newestDirName = malloc(sizeof(char)*32);
   memset(newestDirName, '\0', sizeof(newestDirName));

   DIR* dirToCheck; // Holds the directory we're starting in
   struct dirent *fileInDir; // Holds the current subdir of the starting dir
   struct stat dirAttributes; // Holds information we've gained about subdir

   dirToCheck = opendir("."); // Open up the directory this program was run in

   if (dirToCheck > 0) // Make sure the current directory could be opened
   {
     while ((fileInDir = readdir(dirToCheck)) != NULL) // Check each entry in dir
     {
       if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) // If entry has prefix
       {
      //   printf("Found the prefex: %s\n", fileInDir->d_name);
         stat(fileInDir->d_name, &dirAttributes); // Get attributes of the entry

         if ((int)dirAttributes.st_mtime > newestDirTime) // If this time is bigger
         {
           newestDirTime = (int)dirAttributes.st_mtime;
           memset(newestDirName, '\0', sizeof(newestDirName));
           strcpy(newestDirName, fileInDir->d_name);
        //   printf("Newer subdir: %s, new time: %d\n",
            //      fileInDir->d_name, newestDirTime);
         }
       }
     }
   }

   closedir(dirToCheck); // Close the directory we opened

  // printf("Newest entry found is: %s\n", newestDirName);

  return newestDirName;
}

void PrintRooms(struct Room **rp){
    int i;
    for (i = 0; i < 7; i++){
      printf("Name: %s\n", rp[i]->Name);
      printf("Type: %s\n", rp[i]->Type);
      printf("Number Connected: %d\n", rp[i]->numconnection);
  }
}

void PlayGame(struct Room **rp){
  char destination[8];
  int gameState = 0;
  int i;
  for (i = 0; i < 7; i++){
    if (strcmp(rp[i]->Type, "START_ROOM") == 0){
      printf("CURRENT LOCATION: %s\n", rp[i]->Name);
    }
  }
  while(gameState == 0){
      printf("WHERE TO? >");
      fgets(destination, sizeof(destination), stdin);
      gameState = 1;
    }

}
// struct Room * setConnection(char** connectionList, int connectionCount, struct Room * rp){
//   //printf("%d\n", connectionCount);
//
//   for (int i = 0; i < connectionCount; i++){
//     rp->connection[i] = malloc(sizeof(char)*8);
//     strcpy(rp->connection[i], connectionList[i]);
//   //  printf("%s\n", rp->connection[i]);
//   }
// }


void getRooms(struct Room **rp){
  char *dirname;
  dirname = malloc(sizeof(char)*32);
  dirname = findNewestDir();
  //printf("%s\n", dirname);
  DIR * newestDir;
  struct dirent *roomsInNewDir;
  newestDir = opendir(dirname);
  char delim[8] = " ";
  char delim2[8] = "\n";
  int index = -1;
  int i;

  if (newestDir > 0){

    while ((roomsInNewDir = readdir(newestDir)) != NULL){
      // char *connectionList[6];
      // for (int i = 0; i < 6; i++){
      //   connectionList[i] = malloc(sizeof(char)*8);
      // }
      //

      if(strcmp(".", roomsInNewDir->d_name) != 0 && strcmp("..", roomsInNewDir->d_name) != 0){
        index++;
      //  rp[index]->connectionList = malloc(6*sizeof(char *));
         if(index > 7){
           break;
         }
        char filename[50];
        char filedest[64];
        FILE *fp;
        strcpy(filename, roomsInNewDir->d_name);
        sprintf(filedest, "%s/%s", dirname, filename);
        fp = fopen(filedest, "r");
        if (fp == NULL){
          exit(1);
          }

          char lineBuffer[256];
          char *lineDelim;
          int connectionCount = 0;

          while (fgets (lineBuffer, sizeof(lineBuffer), fp) != NULL){

            lineDelim = strtok(lineBuffer, delim);


          //  printf("%d\n", index);
            while( lineDelim != NULL ){
            //  printf("%s\n", lineDelim);

              if( strcmp(lineDelim, "NAME:") == 0){
                //printf("here");
                lineDelim = strtok(NULL, delim);
                int length = strlen(lineDelim);
                lineDelim[length-1] = 0;
              //  printf("%d\n", index);
                 rp[index]->Name = malloc(sizeof(char)*8);
                 strcpy(rp[index]->Name, lineDelim);
              //  printf("%s\n", lineDelim);
              }

              if( strcmp(lineDelim, "TYPE:") == 0){
                lineDelim = strtok(NULL, delim);
                int length = strlen(lineDelim);
                lineDelim[length-1] = 0; // trim newline character
                rp[index]->Type = malloc(sizeof(char)*8);
                strcpy(rp[index]->Type, lineDelim);

              }

              if( strcmp(lineDelim, "CONNECTION") == 0){
                lineDelim = strtok(NULL, delim);
                 lineDelim = strtok(NULL, delim);
                 int length = strlen(lineDelim);
                 lineDelim[length-1] = 0;
                 connectionList[connectionCount] = malloc(sizeof(char)*8);
                 strcpy(connectionList[connectionCount], lineDelim);

                 connectionCount++;
              // //  printf("%s\n", lineDelim);
              }
              // //rp[index]->numconnection = malloc(sizeof(int*));
                lineDelim = strtok(NULL, delim);
                int game;

            }

          }

        fclose(fp);
        rp[index]->numconnection = connectionCount;
      }


    }

  }



//  PlayGame(rp);
  closedir(newestDir);
}

struct Room **MakeRoomsInit(){

// Make 7 instances of room structs
  struct Room R1;
  struct Room R2;
  struct Room R3;
  struct Room R4;
  struct Room R5;
  struct Room R6;
  struct Room R7;

  // Make pointers of type struct room
  struct Room *ptr_room1;
  struct Room *ptr_room2;
  struct Room *ptr_room3;
  struct Room *ptr_room4;
  struct Room *ptr_room5;
  struct Room *ptr_room6;
  struct Room *ptr_room7;

// initialize an array that will contain 7
  struct Room *ptr_array[7];


  ptr_room1 = &R1;
  ptr_room2 = &R2;
  ptr_room3 = &R3;
  ptr_room4 = &R4;
  ptr_room5 = &R5;
  ptr_room6 = &R6;
  ptr_room7 = &R7;

  ptr_array[0]= ptr_room1;
  ptr_array[1]= ptr_room2;
  ptr_array[2]= ptr_room3;
  ptr_array[3]= ptr_room4;
  ptr_array[4]= ptr_room5;
  ptr_array[5]= ptr_room6;
  ptr_array[6]= ptr_room7;


  struct Room **ptr;
  ptr = malloc(sizeof(struct Room**));
  ptr = ptr_array;
  int i;
  for (  i=0; i < 7; i++){
    ptr[i] = malloc(sizeof(struct Room*));
  }
  int outCon[6];
  getRooms(ptr);
  PlayGame(ptr);

//  PrintRooms(ptr);
return ptr;
}




void main(){
  struct Room ** roomsList;
  roomsList = MakeRoomsInit();

//  PrintRooms(roomsList);
}
