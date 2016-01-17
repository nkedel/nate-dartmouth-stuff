#include "sockserver.h"

// Parses a string and does the appropriate thing
bool
Sockserver::parseplay()
{
  SString quit("bye");
  SString kill("kill");
  SString getplayers("gpl");

  SString get("get"); // Get information from the database
  SString put("put"); // Put information into the database
  SString imt("imt"); // 'It's My Turn' -> Requests the token
  SString dun("dun"); // 'Done' -> Returns the token
  SString sta("sta"); // Client to client 'status' message
  SString brc("brc"); // 'Broadcast' a message
  SString bnd("bnd"); // Get all the name/number bindings
  SString dbs("dbs"); // Database stats, returns # records, # fields/rec
  SString mod("mod"); // Returns all modified files in a database

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
      if ((word == sta) && (clients.Current()->Element()->userlevel > 2))
	{
	  // Save old position on the list
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
	  SString news("~MSG ");


	  // Extract whisper message
	  for (; command.Char(i); i++)
	    news += command.Char(i);

	  cout << "*** " << clients.Current()->Element()->nickname << " is whispering to " << nname << endl;

	  // Send out message to appropriate persons
	  for (clients.Start(); clients.Isitem(); clients.Advance())
	    if (clients.Current()->Element()->nickname == nname)
	      write(clients.Current()->Element()->sock, news.GetC(), news.Len()+1);

	  // Empty command buffer
	  clients.curpos = placer;
	  SString empty;
	  clients.Current()->Element()->curstring = empty;
	}
      else
    // say command
    
  if ((word == brc) && (clients.Current()->Element()->userlevel > 1))
    {
      cout << "*** " << clients.Current()->Element()->nickname << " is chatting." << endl;
      
      SString nname;
      i++;

      SString messa("~MSG ");
   
      // Extracts the user message
      while (command.Char(i))
	messa += command.Char(i++);
      
      messa = messa.Head(nname.Len()-1);
      
      // Broadcast the message
      announce(messa);
    }
  
  else
    if ((word == bnd) && (clients.Current()->Element()->userlevel > 1))
      {
	SString message(gamedata.namebind());
	write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
      }
  else
    if ((word == dun) && (clients.Current()->Element()->userlevel > 1))
      {
	SString AOK("AOK\n");
	SString ERR("ERR: Wasn't your token\n");
	if (gamedata.tokendone(clients.Current()->Element()->sock) == 0)
	  {
	    write(clients.Current()->Element()->sock, AOK.GetC(), AOK.Len()+1);
	    int sockn = clients.Current()->Element()->sock;
	    
	    for (clients.Start(); ((clients.Current()->Element()->sock != sockn) 
				   && (clients.Isitem())); clients.Advance());
	    clients.Advance();
	    if (!clients.Isitem())
	      {
		clients.Start();
	      }
	    
	    SString output(gamedata.givetoken(clients.Current()->Element()->sock));
	    
	    write(clients.Current()->Element()->sock, output.GetC(), output.Len()+1);	
	  }
	else
	  write(clients.Current()->Element()->sock, ERR.GetC(), ERR.Len()+1);
      }

  else
    if ((word == imt) && (clients.Current()->Element()->userlevel > 1))
      {
	SString message(gamedata.gettoken(clients.Current()->Element()->sock));
	write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
      }

  else
    if ((word == get) && (clients.Current()->Element()->userlevel > 1))
      {
      i++;
      SString db;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	db += command.Char(i);

      int numdb = atoi(db.GetC());

      i++;
      SString rec;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	rec += command.Char(i);

      int numrec = 0;
      if (rec.Char(0) == '*')
	numrec = -1;
      else
	numrec = atoi(rec.GetC());

      i++;
      SString field;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	field += command.Char(i);

      int numfield = 0;
      if (field.Char(0) == '*')
	numfield = -1;
      else
	numfield = atoi(field.GetC());
      char * p = gamedata.getf(numdb, numrec, numfield);
      if (p == NULL)
	{
	  SString message("ERR Empty Record\n");
	  write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
	  return false;
	}
      SString message(gamedata.getf(numdb, numrec, numfield));
      write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
      }

    else

    if ((word == mod) && (clients.Current()->Element()->userlevel > 1))
      {
      i++;
      SString db;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	db += command.Char(i);
      int numdb = atoi(db.GetC());

      i++;
      SString time;
      for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	time += command.Char(i);
      int numtime = atoi(time.GetC());

      SString message(gamedata.getmod(numdb, numtime));
      write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
      }

    else

      if ((word == put) && (clients.Current()->Element()->userlevel > 1))
	{
	  i++;
	  SString token;
	  for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	    token += command.Char(i);

	  int tokennum = atoi(token.GetC());
	  // <token time> <transaction#> <db#> <rec#> <field#> <data>

	  i++;
	  SString trans;
	  for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	    trans += command.Char(i);
	  int transnum = atoi(trans.GetC());

	  i++;
	  SString db;
	  for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	    db += command.Char(i);

	  int numdb = atoi(db.GetC());

	  i++;
	  SString rec;
	  for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	    rec += command.Char(i);

	  int numrec = atoi(rec.GetC());

	  i++;
	  SString field;
	  for (; (command.Char(i) != ' ') && (command.Char(i) != char(13)) && command.Char(i); i++)
	    field += command.Char(i);

	  int numfield = atoi(field.GetC());

	  int dbtypenum = gamedata.getfieldtype(numdb, numfield);
	  
	  i++;
	  SString sdata("",false);
	  for (;(command.Char(i) != char(10)) && (command.Char(i) != char(13)) && command.Char(i); i++)
	    sdata += command.Char(i);
	  
	  SString message;
	  
	  if (dbtypenum == 0) 
	    message += gamedata.put(clients.Current()->Element()->sock, 
				    tokennum, transnum, numdb, numrec, 
				    numfield, sdata.GetC());
	  if (dbtypenum == 1)
	    {
	      int * t1 = new int;
	      *t1 = atoi(sdata.GetC());
	      message += gamedata.put(clients.Current()->Element()->sock,
				      tokennum, transnum, numdb, numrec, 
				      numfield, t1);
	    }

	  if ((dbtypenum != 0) && (dbtypenum != 1))
	    {
	      SString message("ERR Invalid data type\n");
	      write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
	    }
	  else
	    write(clients.Current()->Element()->sock, message.GetC(), message.Len()+1);
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
      SString errmes("ERR *** ERROR: Invalid Command: ");
      errmes += word;
      errmes += '\n';
      write(clients.Current()->Element()->sock, errmes.GetC(), errmes.Len()+1);
    }
  return false;
}
