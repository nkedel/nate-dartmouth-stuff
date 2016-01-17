/*************************************************
file: chats.cc
by Dave Marmaros and Jeff Steeves
5/26/98
CS 78
Project

This file implements the game socket server class.
*************************************************/

#include "chats.h"


Sockserver::Sockserver()
{
  numlisten = MAX_LISTEN;
  people = NULL;
}

Sockserver::Sockserver(int maxlisten)
{
  numlisten = maxlisten;
  people = NULL;
}

Sockserver::~Sockserver()
{
  // HAHA!
}

// Announces a message to all connections
void
Sockserver::announce(SString message)
{
  // Store previous position on 
  Node * placer = clients.Current()->aprev;

  // Broadcasts the message to all users
  for (clients.Start(); clients.Isitem(); clients.Advance())
    write(clients.Current()->Element()->sock, message.GetC(), message.Len());
  
  // resets list to original position
  clients.curpos = placer;
}

// Disconnects a user
void 
Sockserver::drop_user()
{
  cout << "*** " << clients.Current()->Element()->nickname << " is quitting." << endl;
  
  // Compose a quiting message
  SString message("MSG *** ");
  message += clients.Current()->Element()->nickname;
  SString message1(" has left\n");
  message += message1;
  
  // Announce the message to everyone
  // announce(message);
  
  // Close the socket connecting the user
  close(clients.Current()->Element()->sock);
  
  // Delete the user from the list
  clients.Delete();
}

// Parses a string and does the appropriate thing
bool
Sockserver::parse()
{
  SString say("say");
  SString quit("bye");
  SString kill("kill");
  SString getplayers("gpl");
  SString player("plr");
  SString yoda("yoda");
  SString ready("rdy");

  SString get("get"); // Get information from the database
  SString put("put"); // Put information into the database
  SString imt("imt"); // 'It's My Turn' -> Requests the token
  SString dun("dun"); // 'Done' -> Returns the token
  SString sta("sta"); // Client to client 'status' message
  SString brc("brc"); // 'Broadcast' a message
  SString bnd("bnd"); // Get all the name/number bindings
  //SString dbn("dbn"); // Get database associated with name 
  //SString dbr("dbr"); // Get database associated with number
  //SString fln("fln"); // Get field associated with name
  //SString flr("flr"); // Get field associated with number
  SString dbs("dbs"); // Database stats, returns # records, # fields/rec

  // Extract the command
  SString word;
  SString command(clients.Current()->Element()->curstring);
  
  // Empty the command line
  SString empty;
  clients.Current()->Element()->curstring = empty;
  
  int i;
  for (i = 1; (command.Char(i)) && isalpha(command.Char(i)); i++)
    word += command.Char(i);
  
  SString word1;
  for (i = 1; (word.Char(i)) && isalpha(word.Char(i)); i++)
    word1 += tolower(word.Char(i));
  
  word = word1;
  
  // Run the kill command
    
  if ((word == kill) && (clients.Current()->Element()->userlevel > 5))
    shutdownServer();
  else
    
    // Prints out a list of who is logged on
    
    if ((word == getplayers) && (clients.Current()->Element()->userlevel > 0))
    {
      // Store previous place on the list
      Node * placer = clients.Current()->aprev;
      int cursock = clients.Current()->Element()->sock;
      cout << "*** " << clients.Current()->Element()->nickname << " is calling 'getplayers'" << endl;
      
      // Parse list printing out who is logged on
      for (clients.Start(); clients.Isitem(); clients.Advance())
	{
	  SString nickn(clients.Current()->Element()->nickname);
	  SString servname(clients.Current()->Element()->servername);
	  /* SString space("    ");
	  SString output(space);
	  output += nickn;
	  SString from(" from ");
	  output += from;
	  output += servname;
	  SString level(" level: ");
	  output += level;
	  SString lev(clients.Current()->Element()->userlevel);
	  output += lev;
	  output += '\n';
	  */

	  SString INF("INF ");
	  SString output(INF);
	  output += nickn;
	  SString from(" from ");
	  output += from;
	  output += servname;
	  output += '\n';
	  
	  write(cursock, output.GetC(), output.Len());
	}
      
      // Return to the previous place on the list
      clients.curpos = placer;
    }
  
  else
    
  if ((word == player) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** " << clients.Current()->Element()->nickname << " is inserting player information" << endl;
      Node * placer = clients.Current()->aprev;
      i++;
      SString nname;

      if (command.Char(i) == '"')
	{
	  i++;
	  for (; (command.Char(i) != '"') && (command.Char(i) != char(13)) && command.Char(i); i++)	
	    nname += command.Char(i);
	  i++;
	}
      else
	for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)	
	  nname += command.Char(i);
      
      i++;
      SString charfile;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	charfile += command.Char(i);
	
      clients.Current()->Element()->charfile = charfile;
      i++;
      SString pass;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && (command.Char(i) != char(10)) && command.Char(i); i++)
	pass += command.Char(i);
      
      clients.Current()->Element()->pass = pass;
      // move to old spot on the list
      
      // Is the name 0 bytes in length? (> 1 because of the \n char present)
      if ((nname.Len() > 0) && (pass.Len() > 0) && (charfile.Len() > 0)) 
	{
	  clients.Current()->Element()->nickname = nname;
	  /* SString message("*** Info: ");
	  message += nname;
	  message += ' ';
	  message += charfile;
	  message += ' ';      
	  message += pass;
	  message += '\n';
	  */

	  SString message("POK");
	  message += '\n';
	  	  
	  write(clients.Current()->Element()->sock, message.GetC(), message.Len());
	  clients.Current()->Element()->playerok = true;
	}
      else 
	{
	  SString message("ERR *** ERROR: Invalid info\n");
	  write(clients.Current()->Element()->sock, message.GetC(), message.Len());
	}
      
      clients.curpos = placer;
    }
  else
    // say command
    
  if ((word == brc) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** " << clients.Current()->Element()->nickname << " is chatting." << endl;
      
      SString nname;
      i++;

      SString nick(clients.Current()->Element()->nickname);
      SString messa("BRC ");
      messa += nick;
      messa += '>';
      messa += ' ';

      // Extracts the user action
      while (command.Char(i))
	messa += command.Char(i++);
      
      messa = messa.Head(nname.Len()-1);
      
      // Broadcast the message
      announce(messa);
    }
  

  else
    // ready command
    
    if ((word == ready) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** " << clients.Current()->Element()->nickname << " is attempting to start the game." << endl;
      Node * placer = clients.Current()->aprev;
      
      clients.Start();
      clients.Prev();
      for (clients.Prev(); (clients.Isitem() && (!clients.Current()->Element()->playerok))
	     ; clients.Prev());
	
      if (clients.curpos == placer)
	{
	  cout << "Ready ACTIVATED" << endl;
	  return true;
	}

      
      clients.curpos = placer;
      SString message("ERR *** ERROR: You Do Not Have Access To RDY\n");
      write(clients.Current()->Element()->sock, message.GetC(), message.Len());


      // Broadcast the message
      //announce(messa);
    }


  // Grants superuser access
  
  else
    
  if ((word == yoda) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** Granting SYSOP access" << endl;
      clients.Current()->Element()->userlevel = 6;
      SString sysopmes("*** SYSOP access granted\n");
      write(clients.Current()->Element()->sock, sysopmes.GetC(), sysopmes.Len());
    }

  // A client will log off the server
  
  else

    if ((word == quit) && (clients.Current()->Element()->userlevel > -1) || (command.Len() == 0))
      {
      cout << "*** " << clients.Current()->Element()->nickname << " is quitting." << endl;
      
      // Compose a quiting message
      SString message("MSG *** ");
      message += clients.Current()->Element()->nickname;
      SString message1(" has left\n");
      message += message1;
      
      // Announce the message to everyone
      announce(message);
      
      // Close the socket connecting the user
      close(clients.Current()->Element()->sock);
      
      // Delete the user from the list
      clients.Delete();
    }
  else
    {
      // Unknown command  
      SString errmes("ERR *** ERROR: Invalid Command\n");
      write(clients.Current()->Element()->sock, errmes.GetC(), errmes.Len());
    }
  return false;
}

// Runs initialization routines...
void 
Sockserver::initializeServer(int port)
{
  struct sockaddr_in sa;

  listenport = port;
  
  // Create a socket for the server.
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      cout << "Socket fails. Exiting Program." << endl;
      exit(1);
    }
  
  // Set up the sockaddr data structure
  getAddressForAnyHost(&sa, port); 
  
  // Bind the socket to the address
  if ((bind(sock, (struct sockaddr*) &sa, sizeof(sa))) != 0)
    {
      cout << "Bind Fails. The port may be in use. Exiting Program." << endl;
      exit(1);
    }
 
  // Listen to the socket to prepare to accept connections
  if (listen(sock, numlisten) != 0)
    {
      cout << "Listen Fails. Exiting Program." << endl;
      exit(1);
    }
  cout << "Port opened!" << endl;
}


void
Sockserver::runServer()
{
  int newsockfd;
  fd_set fds[1];
  cout << "*** Server is now running." << endl;
   
  while (1)
    {
      
      FD_ZERO(fds);
      FD_SET(sock, fds);

      // for each client in the list set the fd.
      int fdmax = 0;
      for (clients.Start(); clients.Isitem(); clients.Advance())
	{
	  FD_SET(clients.Current()->Element()->sock, fds);
	  
	  if (clients.Current()->Element()->sock > fdmax)
	    fdmax = clients.Current()->Element()->sock;
	}
      
      // select( max of number of fd or sock + 1, fds, NULL, NULL, NULL);
      if ((select(((fdmax > sock) ? fdmax : sock) + 1, fds, NULL, NULL, NULL)) < 1)
	{
	  cout << "Select failed. Exiting Program." << endl;
	  exit(1);
	}

      // Finished defining select
      
      // check to see if any connections are waiting to be accepted.
      if (FD_ISSET(sock, fds))
	{
	  // If yes, clear it and create a new entry
	  FD_CLR(sock, fds);
	  struct sockaddr_in clientaddr;
	  int sa_size = sizeof(clientaddr);
	  newsockfd = accept(sock, (struct sockaddr *) &clientaddr, &sa_size);
	  
	  // Turn off blocking for the new socket
	  int thingy = fcntl(newsockfd, F_GETFL, 0);
	  fcntl(newsockfd, F_SETFL, thingy | O_NONBLOCK);

     
	  // Did accept work?
	  if (newsockfd < 0)
	    {
	      cout << "Accept failed. Exiting Program." << endl;
	      exit(1);
	    }
	  
	  // Create a new item for our linked list
	  Item * clientitem;
	  clientitem = new Item;

	  // Set user attributes
	  clientitem->sa = clientaddr;
	  clientitem->sock = newsockfd;
	  
	  SString temp(getHostnameFromAddress( (struct sockaddr_in *) &clientaddr));
	  clientitem->servername = temp;

	  cout << "*** " << temp << " connected" << endl;

	  SString name;
	  
	  // Extract the nickname of the user (ie - the first word in the host name)
	  for (int i = 1; temp.Char(i) && (temp.Char(i) != '.'); i++)
	    name += temp.Char(i);
	  
	  clientitem->nickname = name;
	  
	  cout << "*** Name: " << name << endl;

	  // Insert data onto list
	  clients.InsertFront(clientitem);
	  
	  SString welcome("MSG *** Welcome to the Thunderdome server ***\n");
	  write(clientitem->sock, welcome.GetC(), welcome.Len());

	  // Announce that user has arrived.
	  SString message("MSG *** ");
	  SString message1(name);
	  SString message2(" has arrived\n");
	  message += message1;
	  message += message2;
	  announce(message);
	}
      
      char input;

      // check which connection has new input.
      for (clients.Start(); clients.Isitem(); clients.Advance())
	{
	  if(FD_ISSET(clients.Current()->Element()->sock, fds))
	    {
	      FD_CLR(clients.Current()->Element()->sock, fds);
	      
	      int nsock = clients.Current()->Element()->sock;
	      
	      int result;
	      while (0 <= (result = read(nsock, &input, 1)))
		{
		  // If something has been received process it
		  if (0 < result)
		    {
		      clients.Current()->Element()->curstring += input;
		      
		      if (input == '\n')
			if (parse()) 
			  return;

		      input = char(1);
		    }
		  // If socket has been closed then result == 0 so drop_user
		  if (result == 0)
		    {
		      drop_user();
		      break;
		    }
		}
	    }
	}
    } 
  
}


// Shuts down the server safely
int
Sockserver::shutdownServer()
{
  cout << "*** Shutting down the server..." << endl;
  /* SString message("*** Server is shutting down. Thank you for visiting!\n"); */
  SString message("END\n");
  announce(message);
  cout << "*** Server is being killed" << endl;

  int count = 0;

  for (clients.Start(); clients.Isitem(); clients.Advance())
    if (clients.Current()->Element()->playerok) 
      count++;

  if (count > 0)
    {
      people = new Players[count];
      int number = 0;
      for (clients.Start(); clients.Isitem(); clients.Advance())
	if (clients.Current()->Element()->playerok)
	  {
	    
	    people[number].name = new char[clients.Current()->Element()->nickname.Len()+1];
	    people[number].charfile = new char[clients.Current()->Element()->charfile.Len()+1];
	    people[number].password = new char[clients.Current()->Element()->pass.Len()+1];
	    
	    people[number].socknum = clients.Current()->Element()->sock;
	    
	    strcpy(people[number].name, clients.Current()->Element()->nickname.GetC());
	    strcpy(people[number].charfile, clients.Current()->Element()->charfile.GetC());
	    strcpy(people[number].password, clients.Current()->Element()->pass.GetC());
	  }
    }



  // Close all clients
  //  for (clients.Start(); clients.Isitem(); clients.Advance())
  //  close(clients.Current()->Element()->sock);
  
  // Close the connection port
  shutdown(listenport, 0);
  close(listenport);
  
  // Exit the program
  return count;
  //exit(1);
}

int
Sockserver::Start(int port, Players * p)
{
  initializeServer(port);
  runServer();
  p = people;
  return shutdownServer();
}


