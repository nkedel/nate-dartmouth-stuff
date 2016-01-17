// msd -- internet message reveiver daemon -- version 0.1 
//        receives message from ms, and prints it to the console.

// usage: ms <hostname or ip> <port#> 
// reads message text (one line, or 127 characters max) from stdin
// packet format: "MS01!from@host#msglen$text"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

const char myProto[] = "udp";
const char rcvMesg[] = "*RECEIVED*";

int main (int argc, char **argv) {
  int mySocket;
  struct sockaddr_in inAddr, outAddr;
  struct servent *myServ;
  char inBuf[255];
  int i;
  if (argc!=2) { puts("Usage: msd <port #>\n"); exit(1); }
  if (fork()) { puts("Forked; Daemon Started.\n"); exit(0); }
  mySocket = socket(AF_INET, SOCK_DGRAM, 0);  
  bzero( (char *) &inAddr, sizeof(inAddr));
  inAddr.sin_family = AF_INET;
  inAddr.sin_addr.s_addr = INADDR_ANY;
  if ( (myServ=getservbyname(argv[1], myProto)) ) inAddr.sin_port = myServ->s_port;
    else if  ( ( inAddr.sin_port = htons((unsigned short) atoi(argv[1]) ) ) == 0) {
      puts("Bad Service Name/Number! Aborting! \n");
      exit(1);
      }
  if (bind(mySocket, (struct sockaddr *) &inAddr, sizeof(inAddr)) < 0) {
     puts("Can't bind socket. Dying.\n"); exit(0); 
     }
  while (1) {
    i = sizeof(outAddr);
    if (recvfrom(mySocket, inBuf, sizeof(inBuf), 0, (struct sockaddr *) &outAddr, &i) < 0) {
      puts("Recvfrom indicated error. Dying.\n"); exit(1);
      }
    puts(inBuf);
    puts("\n");
    // parse & print (inbuf) later
    //  (void) sento(mySocket, rcvMesg, sizeof(rcvMesg), 0, (struct sockaddr *)&outAddr, sizeof(outAddr));
    } 
  }
