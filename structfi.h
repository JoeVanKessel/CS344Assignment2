#ifndef HEADER_FILE_1
#define HEADER_FILE_1
struct Room{
  char *Name;
  char *Type;
  int  index;
  int   numconnection;
  int   *outbound;
  char *connections[6];
};
#endif
