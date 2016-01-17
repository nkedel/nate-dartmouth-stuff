// ms -- internet message passer -- version 0.1 
//       sends a simple message to the user running msd on a port
//       or query's the users name/info on a port.

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

char * compose(char *msgText);
void makeAddr(struct sockaddr_in *anAddr, char *host, char *port);

int main (int argc, char **argv) {
  int mySocket, i;
  struct sockaddr_in myAddr;
  char mesg[128]="", *finalMsg;
  if (argc < 2) {
    puts("usage: ms <hostname or ip> <port#>\n");
    puts("reads message text (one line, or 127 characters max) from stdin\n");
    exit(1);
    }
  while (!strlen(mesg)) gets(mesg);
  finalMsg=compose(mesg);
  mySocket=socket(AF_INET, SOCK_DGRAM, 0);
  makeAddr(&myAddr, argv[1], argv[2]);
  i = sizeof(myAddr);
  if (sendto(mySocket, mesg, strlen(mesg), 0, (sockaddr *) &myAddr, i) < 0) 
    puts("Failed!\n");
  close(mySocket);
  return(0);
  }

char * compose(char *msgText) {
  const char hdr[] = "MS01!";
  const char dl1[] = "@",
             dl2[] = "#",
             dl3[] = "$";
  char *newText, myHost[MAXHOSTNAMELEN],myUser[20],msgLen[6]="000";
  if ( gethostname(myHost, MAXHOSTNAMELEN) < 0) {
    strcpy(myHost, "UNKNOWN");
    }
  strcpy(myUser, "UNKNOWN");
  
  int newlen = strlen((const char *) hdr) + strlen((const char *) myUser) + \
               strlen((const char *) dl1) + strlen((const char *) myHost) + \
               strlen((const char *) dl2) + strlen((const char *) msgLen) + \
               strlen((const char *) dl3) + 128;
  newText = new char[newlen];
  strcpy(newText, (const char *) hdr);
  strcat(newText, (const char *) myUser);
  strcat(newText, (const char *) dl1);
  strcat(newText, (const char *) myHost);
  strcat(newText, (const char *) dl2);
  strcat(newText, (const char *) msgLen);
  strcat(newText, (const char *) dl3);
  strcat(newText, (const char *) msgText);
  puts(newText);
  puts("\n");
  return newText;
  }
 
void makeAddr(struct sockaddr_in *anAddr, char *host, char *port) {
  struct hostent *SmyHost;
  struct servent *SmyServ;
  if ( SmyServ= getservbyname(port, "udp") ) anAddr->sin_port = SmyServ->s_port;
    else if  ( ( anAddr->sin_port = htons((unsigned short) atoi(port) ) ) == 0) {
      puts("Bad Service Name/Number! Aborting! \n");
      exit(1);
      }
  if ( SmyHost = gethostbyname(host) ) bcopy( SmyHost->h_addr, (char *) &anAddr->sin_addr, SmyHost->h_length);
    else if ( ( anAddr->sin_addr.s_addr = inet_addr(host) ) == INADDR_NONE) {
      puts("Bad Host Name/IP Address! Aborting! \n");
      exit(1);
      }
  }
