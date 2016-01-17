/*
 * vsclient.cc
 *
 * A very minimal implementation of the client send portion of the assignment
 *
 * Nate Edel & Peter Horowitz
 *
 */

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "sockaddr.h"

void helpmessage(void); // explains the command line options

void parserr(int err);  // error handler; parses errno into a text message

void reader(int readSock);  // function that will be the process that reads from
                            //  the server

void writer(int writeSock); // function that will be the process that will output
                            // information from the server

// inline void alive(const char* msg) { printf("%s\n",msg); }

int main(int argc, char **argv)
{
  int clientSocket, i;
	// sets the default server name to zealand.cs.dartmouth.edu at port 3210
	char serverName[128] = "zealand.cs.dartmouth.edu";
  char username[16];
  char outBuffer[255];
  int  serverPort = 3210,
       childId = 0; // for use with the fork call
	struct sockaddr_in serverSockaddr;
	 
  switch (argc) {
    case 2: helpmessage(); // copies the user supplied servername
            exit(0);
    case 3: strcpy(serverName, argv[1]); // "
            serverPort = atoi(argv[2]);  // receive new port if 
                                         // specified by user
            break;
    default: break;
    }
  
  // test to make sure that the serverName is at least 3 characters if it isn't
  // complain and goto helpmessage()
    
  if (strlen(serverName) < 3 || serverPort == 0) helpmessage();
	
	// Try to find an IP address for serverName; if we can't, complain and die.
	i = getAddressForHost(&serverSockaddr, serverName, serverPort);
	if (i!=0) {
		printf("Either can't find %s or there has been an error.\n", serverName);
    helpmessage();
	}
	
	printf("Enter your name:");
	gets(username);
	if (strlen(username) < 2) strcpy(username,"Ned Flanders"); 
  // open a socket, abort on error
	i = socket(PF_INET, SOCK_STREAM, 0);
	if (i == -1) parserr(errno);
	clientSocket = i;
	
  // connect to the server; abort on error
	i = connect(clientSocket, (sockaddr *) &serverSockaddr, sizeof(serverSockaddr));
  if (i== -1) parserr(errno);
  
  // build a "/name username" command then send it to the server.
  sprintf(outBuffer, "/name %s\n", username);
  i=write(clientSocket, outBuffer, strlen(outBuffer));
  
  if (i == 0 || i == -1) parserr(errno);
  
  // split (fork) into 2 processes -- original for reader, new one for writer
  childId=fork();
  if (childId < 0) exit(-1);   // die on error
  if (childId > 0) reader(clientSocket); // original process: reader
     else writer(clientSocket);          // child process: writer
  if (childId > 0) kill(childId, 9);     // if parent, make sure child is dead.
  exit(0);
  }
  
void writer(int writeSock) {
  char outBuffer[255]; 
  int i=0;
  while(1) {  
    gets(outBuffer);
    i = strlen(outBuffer);
    if (i == 0) continue; // don't bother to send blank message
    outBuffer[i]='\n';
    i=write(writeSock, outBuffer, i);
    if (i == -1) parserr(errno);
    if (i == 0 || strcmp(outBuffer, "/quit") == 0) break; // break while loop
                                                          // when /quit typed
    }
  close(writeSock); // terminates connection 
  }

void reader(int readSock) {
  char inBuffer[255];
  int i;
  while (1) {
    // read in up to 256 characters and 
    // write them to file descriptor 1 (STDOUT)
    i=read(readSock, inBuffer, 255);
    if (i>0) write(1,inBuffer,i);
    if (i == -1) parserr(errno);
    if (i == 0) break;
    }
  close(readSock); // terminates socket
  }


void parserr(int err) {
    // prints out error messages
    printf("ERROR: %s (code %d)\n", sys_errlist[err], err);
    }
    
void helpmessage(void) {
    printf("chatclient [servername serverport]\n");
    printf("  servername / serverport pair is optional, and\n");
    printf("  defaults to 'zealand.cs.dartmouth.edu 3210'\n");
    exit(0);
    }
