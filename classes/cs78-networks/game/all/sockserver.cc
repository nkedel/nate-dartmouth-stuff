/*************************************************
file: sockserver.cc
by Dave Marmaros and Jeff Steeves
5/26/98
CS 78
Project

This file implements the game socket server class.
*************************************************/

#include "sockserver.h"
#include "assert.h"

Sockserver::Sockserver()
{
  mode = PLAYING;
  numlisten = MAX_LISTEN;
  people = NULL;
  gameinit = false;
}

Sockserver::Sockserver(int maxlisten)
{
  mode = WAITING;
  numlisten = maxlisten;
  people = NULL;
  gameinit = false;
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
    write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
  
  // resets list to original position
  clients.curpos = placer;
}

// Disconnects a user
void 
Sockserver::drop_user()
{
  cout << "*** " << clients.Current()->Element()->nickname << " is quitting." << endl;
  
  // Compose a quiting message
  SString message("~MSG *** ");
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
  SString bnd("bnd");

  // Extract the command
  SString word;
  SString command(clients.Current()->Element()->curstring);
  
  // Empty the command line
  SString empty;
  clients.Current()->Element()->curstring = empty;

  // Extract the first word and convert it to lower case
  int i;
  for (i = 1; (command.Char(i)) && isalpha(command.Char(i)); i++)
    word += tolower(command.Char(i));
  
  cout << word << endl;
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
      
      SString INF("INF ");
      SString output(INF);
      SString numit(clients.numitems);
      output += numit;

      // Parse list printing out who is logged on
      for (clients.Start(); clients.Isitem(); clients.Advance())
	{
	  SString nickn(clients.Current()->Element()->nickname);
	  SString servname(clients.Current()->Element()->servername);
	  /* 
	     SString lev(clients.Current()->Element()->userlevel);
	     output += lev;
	     output += '\n';
	     */
	  
	  output += ' ';
	  output += nickn;
	  output += ' ';
	  output += servname;
      
	}
      
      output += '\n';
  
      write(cursock, output.GetC(), output.Len()+1);
  
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
	  	  
	  write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
	  clients.Current()->Element()->playerok = true;
	}
      else 
	{
	  SString message("ERR *** ERROR: Invalid info\n");
	  write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
	}
      
      clients.curpos = placer;
    }
  else
    // say command
    
  if ((word == say) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** " << clients.Current()->Element()->nickname << " is chatting." << endl;
      
      SString nname;
      i++;

      SString nick(clients.Current()->Element()->nickname);
      SString messa("~MSG ");
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
	  SString output("~BGN ");


	  // Initialize the player's database
	  gamedata.initdb(3);

	  SString dbname("PLAYERS");	  
	  char ** namef;
	  namef = new char*[11];

	  /*	  int x, y;
	  int hp, xp, ac, dmg, kills;
	  char * name;
	  char weapon[32];
	  char armor[32];
	  */

          SString * s;
	  s = new SString[11];
	  
	  s[0] += "x";
	  s[1] += "y";
	  s[2] += "hp";
	  s[3] += "xp";
	  s[4] += "ac";
	  s[5] += "dmg";
	  s[6] += "kills";
	  s[7] += "name";
	  s[8] += "weapon";
	  s[9] += "armor";
	  s[10] += "live";

	  
	  
	  for(int i = 0; i < 11; i++)
	    {
	      s[i].set(false);
	      namef[i] = s[i].GetC();
	    }

	  int ftype[11] = {1,1,1,1,1,1,1,0,0,0,1};
	  gamedata.setupdb(0, 8, 11, dbname.GetC(), namef, ftype);

	  
	  // Monster's database

	  /* int x, y;
	     int hp, ac, dmg;
	     char * name;
	     int tile;
	     bool live; */

	  
	  SString dbname2("MONSTER");	  
	  char ** namef2;
	  namef2 = new char*[10];

          SString * t;
	  t = new SString[10];
	  
	  t[0] += "x";
	  t[1] += "y";
	  t[2] += "hp";
	  t[3] += "ac";
	  t[4] += "dmg";
	  t[5] += "name";
	  t[6] += "tile";
	  t[7] += "live";
	 	  
	  for(int i = 0; i < 8; i++)
	    {
	      t[i].set(false);
	      namef2[i] = t[i].GetC();
	    }

	  int ftype2[8] = {1,1,1,1,1,0,1,1};
	  gamedata.setupdb(2, 100, 8, dbname2.GetC(), namef2, ftype2);

	  


	  // Map
	 
	  SString mapname("MAP",false);
	  char ** namemf;
	  namemf = new char*[1];
	  SString maprow1("MAPROW1",false);
	  //SString maprow2("MAPROW2",false);
	  //SString maprow3("MAPROW3",false);
	  namemf[0] = maprow1.GetC();
	  //namemf[1] = maprow2.GetC();
	  //namemf[2] = maprow3.GetC();
	  int mtype[1] = {0};

	  gamedata.setupdb(1, 25, 1, mapname.GetC(), namemf, mtype);


	  /*
	  // Read the map
	  int fd = open("map.3", O_RDONLY);
	  if (fd == -1)
	    {
	      cout << "ERROR: Map does not exist (call Nate)!" << endl;
	      shutdownServer();
	      exit(1);
	    }
	 
	  char tc;
	  SString temp;
	  while (0 < read(fd, &tc, 1))
	    {
	      temp += tc; 
	      if (tc == '\n')
		{
		  void ** fields;
		  fields = new void*[1];
		  SString map(temp.GetC(),false);
		  fields[0] = map.GetC();
		  
		  gamedata.addrec(1,fields,1);
		  
		  SString empty;
		  temp = empty;
		}
	    }
	  
	  close(fd);
	  */

	   int count = 0;

	   for (clients.Start(); clients.Isitem(); clients.Advance())
	     if (clients.Current()->Element()->playerok) 
	       count++;
	   
	   if (count > 0)
	     for (clients.Start(); clients.Isitem(); clients.Advance())
	       if (clients.Current()->Element()->playerok)
		 {
		   void ** fields;
		   
		   fields = new void*[11];
		   
		   int * t0 = new int;
		   *t0 = (count+1);
		   fields[0] = t0;

		   int * t1 = new int;
		   fields[1] = &(*t1 = (count+1));

		   int * t2 = new int;
		   fields[2] = &(*t2 = 100);

		   int * t3 = new int;
		   fields[3] = &(*t3 = 0);

		   int * t4 = new int;
		   fields[4] = &(*t4 = 22);

		   int * t5 = new int;
		   fields[5] = &(*t5 = 10);

		   int * t6 = new int;
		   fields[6] = &(*t6 = 0);
		   
		   SString t7(clients.Current()->Element()->nickname,false);
		   fields[7] = t7.GetC();
		  
		   SString weapon("Sword of Death",false);
		   fields[8] = weapon.GetC();
		   SString armor("Armor of Doom",false);
		   fields[9] = armor.GetC();

		   int * t10 = new int;
		   fields[10] = &(*t10 = 1);

		   gamedata.addrec(0,fields, 11);
		   count++;
	       }
	   
	   announce(output);
	   mode = PLAYING;
	   return false;
	   //return true;
	}

      
      clients.curpos = placer;
      SString message("ERR *** ERROR: You Do Not Have Access To RDY\n");
      write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);


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
      write(clients.Current()->Element()->sock, sysopmes.GetC(), sysopmes.Len()+1);
    }

  // A client will log off the server
  
  else

    if ((word == quit) && (clients.Current()->Element()->userlevel > -1) || (command.Len() == 0))
      {
      cout << "*** " << clients.Current()->Element()->nickname << " is quitting." << endl;
      
      // Compose a quiting message
      SString message("~MSG *** ");
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
      write(clients.Current()->Element()->sock, errmes.GetC(), errmes.Len()+1);
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

  {
	  gamedata.initdb(3);
	  SString dbname("PLAYERS");	  
	  char ** namef;
	  namef = new char*[11];
          SString * s;
	  s = new SString[11];
	  
	  s[0] += "x";
	  s[1] += "y";
	  s[2] += "hp";
	  s[3] += "xp";
	  s[4] += "ac";
	  s[5] += "dmg";
	  s[6] += "kills";
	  s[7] += "name";
	  s[8] += "weapon";
	  s[9] += "armor";
	  s[10] += "live";

	  
	  
	  for(int i = 0; i < 11; i++)
	    {
	      s[i].set(false);
	      namef[i] = s[i].GetC();
	    }

	  int ftype[11] = {1,1,1,1,1,1,1,0,0,0,1};
	  gamedata.setupdb(0, 8, 11, dbname.GetC(), namef, ftype);

	  
	  // Monster's database

	  /* int x, y;
	     int hp, ac, dmg;
	     char * name;
	     int tile;
	     bool live; */

	  
	  SString dbname2("MONSTER");	  
	  char ** namef2;
	  namef2 = new char*[10];

          SString * t;
	  t = new SString[10];
	  
	  t[0] += "x";
	  t[1] += "y";
	  t[2] += "hp";
	  t[3] += "ac";
	  t[4] += "dmg";
	  t[5] += "name";
	  t[6] += "tile";
	  t[7] += "live";
	 	  
	  for(int i = 0; i < 8; i++)
	    {
	      t[i].set(false);
	      namef2[i] = t[i].GetC();
	    }

	  int ftype2[8] = {1,1,1,1,1,0,1,1};
	  gamedata.setupdb(2, 100, 8, dbname2.GetC(), namef2, ftype2);

	  


	  // Map

	  SString mapname("MAP",false);
	  char ** namemf;
	  namemf = new char*[1];
	  SString maprow1("MAPROW1",false);
	  namemf[0] = maprow1.GetC();
	  int mtype[1] = {0};

	  gamedata.setupdb(1, 25, 1, mapname.GetC(), namemf, mtype);


	  /*
	  // Read the map
	  int fd = open("map.3", O_RDONLY);
	  if (fd == -1)
	    {
	      cout << "ERROR: Map does not exist (call Nate)!" << endl;
	      shutdownServer();
	      exit(1);
	    }
	 
	  char tc;
	  SString temp;
	  while (0 < read(fd, &tc, 1))
	    {
	      temp += tc; 
	      if (tc == '\n')
		{
		  void ** fields;
		  fields = new void*[1];
		  SString map(temp.GetC(),false);
		  fields[0] = map.GetC();
		  
		  gamedata.addrec(1,fields,1);
		  
		  SString empty;
		  temp = empty;
		}
	    }
	  
	  close(fd);
	  
	  */
	   int count = 0;

	   for (clients.Start(); clients.Isitem(); clients.Advance())
	     if (clients.Current()->Element()->playerok) 
	       count++;
	   
	   if (count > 0)
	     for (clients.Start(); clients.Isitem(); clients.Advance())
	       if (clients.Current()->Element()->playerok)
		 {
		   void ** fields;
		   
		   fields = new void*[11];
		   
		   int * t0 = new int;
		   *t0 = (count+1);
		   fields[0] = t0;

		   int * t1 = new int;
		   fields[1] = &(*t1 = (count+1));

		   int * t2 = new int;
		   fields[2] = &(*t2 = 100);

		   int * t3 = new int;
		   fields[3] = &(*t3 = 0);

		   int * t4 = new int;
		   fields[4] = &(*t4 = 22);

		   int * t5 = new int;
		   fields[5] = &(*t5 = 10);

		   int * t6 = new int;
		   fields[6] = &(*t6 = 0);
		   
		   SString t7(clients.Current()->Element()->nickname,false);
		   fields[7] = t7.GetC();
		  
		   SString weapon("Sword of Death",false);
		   fields[8] = weapon.GetC();
		   SString armor("Armor of Doom",false);
		   fields[9] = armor.GetC();

		   int * t10 = new int;
		   fields[10] = &(*t10 = 1);

		   gamedata.addrec(0,fields, 11);
		   count++;
		 }
	   
	   //announce(output);
	   mode = PLAYING;

  }

   
  while (1)
    {
      clients.Start();
      /*if ((mode == PLAYING) && (gameinit == false) && (clients.Isitem()))
	{
	  //clients.Start();
	  //SString output(gamedata.givetoken(clients.Current()->Element()->sock));
	  //write(clients.Current()->Element()->sock, output.GetC(), output.Len()+1);	
	  gameinit = true;
	  //close(listenport);
	}*/

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
	  
	  SString welcome("~MSG *** Welcome to the Thunderdome server ***\n");
	  //write(clientitem->sock, welcome.GetC(), welcome.Len()+1);

	  // Announce that user has arrived.
	  SString message("~MSG *** ");
	  SString message1(name);
	  SString message2(" has arrived\n");
	  message += message1;
	  message += message2;
	  //announce(message);
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
		  //cout<<(int)input<<endl;
		  // If something has been received process it
		  if (0 < result)
		    {		         
		      //char * p;
		      //while ((p = parse()) == NULL);
		      clients.Current()->Element()->curstring += input;
		      
		      if (input == '\n')
			{
			  //cout << clients.Current()->Element()->curstring << endl;
			  if (mode == WAITING) 
			    {
			      if(parse()) 
				return;
			    }
			  else
			    if (mode == PLAYING)
			      if (parseplay())
				return;
			}

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


