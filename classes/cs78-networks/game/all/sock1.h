/*************************************************
file: driver.cc
by Dave Marmaros and Jeff Steeves
4/13/98
CS 78
Assignment 1

This file implements the internet chat client program.
*************************************************/


#include "sockaddr.h"
#include <iostream.h>
#include "String.h"
#include "msleep.h"

#define MAXHOSTNAMELEN 	128		
#define PORTNUM 3210

class Sock
{
public:
  Sock();
  Sock(char *, int); 
  ~Sock();

  int socknum;
  bool valid;
  struct sockaddr_in sa;
  SString input;

  int Connect(char *, int);
  int Disconnect();

  int Reader(char *);
  int Writer(char *, int);

  bool Open() 
    {return valid;}
};


Sock();
{
  valid = false;
}

Sock(char * host, int port)
{
  valid = true;
  socknum = Connect(host, port);
  if (socknum = -1)
    valid = false;
}

~Sock()
{
  if (valid) 
    close(socknum);
}

int
Sock::Connect(char * host, int port);
{
  // Trying to open a new socket when one is already open
  // Solution: Close the other one first

  if (valid)
    return -1;

  int rc = getAddressForHost(&sa, host, port);

  // Print an error and exit if the host was not found.
  if (rc != 0) {
    printf("Can't find %s\n", host);
    return -2;
  }

  // Get a socket
  int socknum = socket(AF_INET,SOCK_STREAM,0);
 
  // Establish a connection to the server.  
  if (connect(socknum,(struct sockaddr*) &sa,sizeof(sa)) == -1)
    {
      // Check if the connection was successful. If not, return an error.
      cout << "ERROR - Connection Fails." << endl;
      return -3;
    }
  
  // Set the socket to nonblocking. 
  int flags = fcntl(socknum, F_GETFL, 0);
  fcntl(sock1, F_SETFL, flags | O_NONBLOCK);

  // We're done!
  valid = true;
  return 0;
}


int
Sock::Disconnect()
{
  if (valid)
    {
      shutdown(socknum, 0);
      close(socknum);
      valid = false;
      return 0;
    }
  return -1;
}

int 
Sock::Reader(char * output);
{
  bool done = false;
  char tchar = 0xff;

  // Read any characters that are being sent and write them to the console.
  while (1 == (read_val = read(sock1, &tchar, 1)))
      {
	if (tchar == 0xff)
	  {
	    done = true;
	    break;
	  }
	input += tchar;
      }

  // If read returned 0, then the server has closed the connection.
  if (read_val == 0)
    {
      cout << "Server connection lost. Exiting." << endl;
      valid = false;
    }
  
  // Do we have a full string? (a message string must end with 0xff)
  if (done)
    {
      output = input.GetC();
      return 0;
    }
  else 
    {
      output = NULL;
      return -1;
    }
}


int 
Sock::Writer(char * output, int len);
{
  char eom = 0xff;
  write(socknum, output, len);
  write(socknum, eom, 1);
}





int main(int argc, char *argv[])
{
  struct sockaddr_in sa;
  int rc;
  int pnum;
  
  if (argc < 2)
    {
      fprintf(stderr, "Usage: %s hostname [port]\n", argv[0]);
      exit(1);
    }
  
  // Fill in the sockaddr structure with the desired host and port
  if (argc == 3)
    {
      pnum = atoi(argv[2]);
      rc = getAddressForHost(&sa, argv[1], pnum);
    } else {
      rc = getAddressForHost(&sa, argv[1], PORTNUM);
    }  

  // Print an error and exit if the host was not found.
  if (rc != 0) {
    printf("Can't find %s\n", argv[1]);
    exit(1);
  }

  // Create a socket
  int sock1 = socket(AF_INET,SOCK_STREAM,0);
 
  // Establish a connection to the server.  
  int con = connect(sock1,(struct sockaddr*) &sa,sizeof(sa));

  // Check if the connection was successful. If not, quit.
  if (con == -1)
    {
      cout << "Connection Fails. Exiting Program." << endl;
      exit(1);
    }

  // Get the file descriptor for standard input.
  int istr = fileno(stdin);

  // Set the standard input to nonblocking.
  int flags2 = fcntl(istr, F_GETFL, 0);
  fcntl(istr, F_SETFL, flags2 | O_NONBLOCK);
 
  // Set the socket to nonblocking. 
  int flags = fcntl(sock1, F_GETFL, 0);
  fcntl(sock1, F_SETFL, flags | O_NONBLOCK);
  
  SString st;
  SString quit("/quit\n");
  char input;
  int read_val;
  bool done = FALSE;

  do
    {
      // Read any characters that are being sent and write them to the console.
      while (0 <= (read_val = read(sock1, &input, 1)))
	{
	  // If read returned 0, then the server has closed the connection.
	  if (read_val == 0)
	    {
	      cout << "Server connection lost. Exiting." << endl;
	      exit(1);
	    }
      	  cout << input;
    	}

      // Since our read call is non-blocking, test an unused character
      // to determine if new input exists.
      char ch = char(1);
	  
      read(istr, &ch, 1);
      
      // If there is a new character, append it to our input string.
      if (int(ch) != 1) 
	st += ch;
      
      // If the character is a newline, send the entire string to the server.
      if (ch == '\n')
	{
	  //cout << "Sending Command: " << st << endl;
	  write(sock1, st.GetC(), st.Len());
	  
	  if (st == quit)
	    {
	      done = TRUE;
	    }

	  // Create a new empty string to clear the contents of the input.
	  SString a;
	  st = a;
	}
      msleep(10);
      // Loop until the user sends a quit server command.
    } while (!done);
  
  // close down the socket and exit the client program.
  shutdown(sock1, 0);
  close(sock1);
  exit(0);
}




