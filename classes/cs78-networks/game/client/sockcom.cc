#include "sockcom.h"

Sock::Sock()
{
  wait = 0;
  valid = false;
}

Sock::Sock(char * host, int port)
{
  wait = 0;
  valid = true;
  socknum = Connect(host, port);
  if (socknum == -1)
    valid = false;
}

Sock::~Sock()
{
  if (valid) 
    close(socknum);
}

int
Sock::Connect(char * host, int port)
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
  if ((socknum = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
      // Socket failed!
      return -4;
    }
 
  // Establish a connection to the server.  
  if (connect(socknum,(struct sockaddr*) &sa,sizeof(sa)) == -1)
    {
      // Check if the connection was successful. If not, return an error.
      // cout << "ERROR - Connection Fails." << endl;
      return -3;
    }
  
  // Set the socket to nonblocking. 
  //int flags = fcntl(socknum, F_GETFL, 0);
  //fcntl(socknum, F_SETFL, flags | O_NONBLOCK);

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

char * 
Sock::Reader()
{
  bool done = false;
  char tchar = 0x01;
  int read_val = 0;

  // Read any characters that are being sent and write them to the console.
  while ((!done) && (1 == (read_val = read(socknum, &tchar, 1))))
      {
	if ((tchar == 0x00) /*|| (tchar == '\n')*/)
	  {
	    done = true;
	    break;
	  }
	input += tchar;
      }

  // If read returned 0, then the server has closed the connection.
  if (read_val == 0)
    {
      // cout << "Server connection lost. Exiting." << endl;
      valid = false;
      return NULL;
    }
  
  // Do we have a full string? (a message string must end with 0x00)
  if (done)
    {
      SString out(input, false);
      // cout << "Reader returns: " << out.GetC() << "\r\n";
      SString empty;
      input = empty;
      return out.GetC();
    }
  else 
    {
      //outp = NULL;
      return NULL;
    }
}


int 
Sock::Writer(char * output, int len)
{
  //char eom = 0x00;
  // cout << "Writer is writing: " << output << " len = " << len << "\r\n";
  return write(socknum, output, len-1);
  //write(socknum, &eom, 1);
  //return 0;
}

int 
Sock::Preconnect(int port, int maxl)
{
  if (valid)
    return -1;

  // Create a socket for the server.
  if ((socknum = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
     	// cout << "Socket fails. Exiting Program." << endl;
      return -4;
    }
  
  // Set up the sockaddr data structure
  getAddressForAnyHost(&sa, port); 
  
  // Bind the socket to the address
  if ((bind(socknum, (struct sockaddr*) &sa, sizeof(sa))) != 0)
    {
      // cout << "Bind Fails. The port may be in use. Exiting Program." << endl;
      return -3;
    }
 
  // Listen to the socket to prepare to accept connections
  if (listen(socknum, maxl) != 0)
    {
    	// cout << "Listen Fails. Exiting Program." << endl;
      return -2;
    }
  
  wait = 1;
  return 1;
}

int 
Sock::Check()
{
  if (wait != 1)
    return -1;

  fd_set fds[1];
  FD_ZERO(fds);
  FD_SET(socknum, fds);
      
  // check to see if any connections are waiting to be accepted.
  if (FD_ISSET(socknum, fds))
	{
	  // If yes, clear it and create a new entry
	  FD_CLR(socknum, fds);
	  struct sockaddr_in clientaddr;
	  int sa_size = sizeof(clientaddr);
	  socknum = accept(socknum, (struct sockaddr *) &clientaddr, &sa_size);
	  
	  // Turn off blocking for the new socket
	  int thingy = fcntl(socknum, F_GETFL, 0);
	  fcntl(socknum, F_SETFL, thingy | O_NONBLOCK);

	  // Did accept work?
	  if (socknum < 0)
	    {
	    	// cout << "Accept failed. Exiting Program." << endl;
	      wait = 0;
	      return -2;
	    }
	  
	  wait = 0;
	  valid = true;
	}
  return 0;
}
