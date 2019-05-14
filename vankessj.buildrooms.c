#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "structfi.h"
#include <time.h>


void InitNumConnections(struct Room** np){
  for (i = 0; i < 7; i++){
    np[i]->numconnection = 0;
  }
}

void PrintSingleRoom(struct Room* aroom){
      printf("Name: %s\n", aroom->Name);
      printf("Type: %s\n", aroom->Type);
}

void setindex(struct Room** allrooms){
  for(int i = 0; i < 7; i++){
    allrooms[i]->index = i;
  }
}

//############################################################################
//FUNCTION: SetNames
//TYPE: void
//PARAMETERS: POINTER STRUCT ROOM** pointer to array of pointers to structs
//############################################################################

void SetNames(struct Room **xy){

  char *names[7]; //array of names
  char *name_list[10] = {
    "Home",
    "Breannah",
    "Prarie",
    "Store",
    "Mine",
    "Butcher",
    "Gunsmith",
    "Farm",
    "Mountian",
    "Spaceship"
    };

//scramble the name_list
  for (int i = 0; i < 10; i++){
    char *tmp = name_list[i];
    int k = rand()%10;
    name_list[i] = name_list[k];
    name_list[k] = tmp;

  }

//take 7 names
  for (int i = 0; i < 7; i++){
    names[i] = malloc(sizeof(char)*8);
    names[i] = name_list[i];
  }

//add the names in names array to the name member in each struct
  for (int i = 0; i < 7; i++){
    xy[i]->Name = malloc(sizeof(char)*8);
    strcpy(xy[i]->Name,names[i]);
  }



}

//############################################################################
//FUNCTION: SetTypes
//TYPE: void
//PARAMETERS: POINTER STRUCT ROOM** pointer to array of pointers to structs
//FUNCTION:Iitialize the room types for each room struct
//############################################################################


void SetTypes(struct Room **yx){

  //all rooms recive type mid_room
  for (int i = 0; i < 7; i++){
    yx[i]->Type = malloc(sizeof(char)*8);
    strcpy(yx[i]->Type, "MID_ROOM");
  }

  //pick random room save index
  //make this room type start_room
  int arand = rand() % 7;
//  printf("%d\n", arand);
  strcpy(yx[arand]->Type, "START_ROOM");

  int repflag = 1;

  //pick random room that isnt the first room
  //make this room type end_room
  while (repflag == 1){
    int anorand = rand() % 7;
  //  printf("%d\n",anorand);
    if (anorand != arand ){
      strcpy(yx[anorand]->Type, "END_ROOM");
      repflag = 0;
    }
  }

}

int IsGraphFull(struct Room** therooms){
  int fullyconnectedcounter =0;
  for (int i = 0; i < 7; i++){
    if (therooms[i]->numconnection >= 3){
      fullyconnectedcounter++;
    }
  }

  if (fullyconnectedcounter == 7){
    return 1;
  }

  else{
    return 0;
  }

}



int ConnectionAlreadyExists(int numcon, int *outA, int indxofB){
  for (int j = 0; j < numcon; j++){
    if(outA[j] == indxofB){
      return 1;
    }
  }
  return 0;
}


void AddRandomConnection(struct Room **allrooms){

  int indexA;
  int indexB;


  int i =1;
  int j = 1;

  while(i == 1){
    indexA = rand()%7;
    if (allrooms[indexA]->numconnection < 6){
      break;
    }
  }
do{
    indexB = rand()%7;
}while( allrooms[indexB]->numconnection >= 6 ||indexA == indexB || ConnectionAlreadyExists(allrooms[indexA]->numconnection, allrooms[indexA]->outbound, allrooms[indexB]->index) == 1);


  int Aconnect = allrooms[indexA]->numconnection;
  allrooms[indexA]->outbound[Aconnect] = allrooms[indexB]->index;
  allrooms[indexA]->numconnection = Aconnect + 1;
  int Bconnect = allrooms[indexB]->numconnection;
  allrooms[indexB]->outbound[Bconnect] = allrooms[indexA]->index;
  allrooms[indexB]->numconnection = Bconnect + 1;
}

//############################################################################
//FUNCTION: CreatOutboundConnections
//TYPE: void
//PARAMETERS: POINTER STRUCT ROOM** pointer to array of pointers to structs
//FUNCTION: initialises outbound connections
//############################################################################
void SetOutboundConnections(struct Room** alltherooms){

  for(int i = 0; i < 7; i++){
    alltherooms[i]->outbound = malloc(sizeof(int)*7);
  }
  while(IsGraphFull(alltherooms) == 0){
    AddRandomConnection(alltherooms);
  }
}

//############################################################################
//FUNCTION: PrintRooms
//TYPE: void
//PARAMETERS: NA
//FUNCTION: Prints Rooms
//############################################################################
void PrintRooms(struct Room **rp){
    for (int i = 0; i < 7; i++){
      printf("Name: %s\n", rp[i]->Name);
      printf("Type: %s\n", rp[i]->Type);
      printf("Number Connected: %d\n", rp[i]->numconnection);
      int numcon = rp[i]->numconnection;
      for (int j = 0; j < numcon; j++){
        int connection1 = rp[i]->outbound[j];
        printf("connection: %s\n", rp[connection1]->Name);
      }

    }
  }


void MakeDir(struct Room **rp){
    int processID;
    processID = getpid();
    char dirname[20] = "vankessj.rooms.";
    sprintf(dirname, "%s%d",dirname, processID);
    mkdir(dirname, 0755);
    FILE *files[7];
    for (int i = 0; i < 7; i++){
      char filename[50];
      sprintf(filename, "%s/room%d.txt", dirname, i+1);
      files[i] = fopen(filename, "w");
      fprintf(files[i], "ROOM NAME: %s\n", rp[i]->Name);

      int numcon = rp[i]->numconnection;
      for (int j = 0; j < numcon; j++){
        int connection = rp[i]->outbound[j];
        fprintf(files[i], "CONNECTION %d: %s\n", j+1, rp[connection]->Name);
      }
      fprintf(files[i], "ROOM TYPE: %s\n", rp[i]->Type);
      fclose(files[i]);
    }

  }
//############################################################################
//FUNCTION: MakeRoomsInit
//TYPE: void
//PARAMETERS: POINTER STRUCT ROOM** pointer to array of pointers to structs
//############################################################################
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

  for (int i=0; i < 7; i++){
    ptr[i] = malloc(sizeof(struct Room*));
  }
  SetNames(ptr);
  SetTypes(ptr);
  InitNumConnections(ptr);
  setindex(ptr);
  SetOutboundConnections(ptr);
  MakeDir(ptr);

return ptr; //Return the pointer to an array of pointers
}

struct Room** MakeGameBoard(){

  struct Room **rooms;
  rooms = MakeRoomsInit();

  return rooms;

}

int main(){

  srand(time(NULL));
  struct Room **boardptr;
  boardptr = MakeGameBoard();
  int pid = getpid();
  printf("%d\n", pid);

//  PrintRooms(boardptr);

  return 0;
}
