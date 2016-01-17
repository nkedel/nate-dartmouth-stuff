#include "sockcom.h"
#include "iostream.h"
#include "player.h"
#include "monster.h"
#include "globals.h"
#include "assert.h"

#ifndef gamec
#define gamec

class Client {

private:
  Sock c;
  
  char * statusm;
  int turn;
  bool psetup;
  bool mode;        // 0 = init state 1 = gamestate

public:
  Client() {turn = -2; mode = 0;}
  ~Client() {}

  int Connect(char * host, int port)
    {
      return c.Connect(host, port);
    }

  int Say(char * message)
    {      
      SString output;
      if (mode) 
	output += "SAY ";
      else
	output += "BRC ";

      output += message;
      output += '\n';

      return c.Writer(output.GetC(), output.Len()+1);      
    }

  int Sendstats(char * name, char * charfile, char * password)
    {
      SString output("PLR ");
      output += name;
      output += ' ';
      output += charfile;
      output += ' ';
      output += password;
      output += '\r';
      output += '\n';

      if (c.Writer(output.GetC(), output.Len()+1) > 0)
	return 0;  // Successful
      else 
	return -1;  // Unsuccessful
    }

  // Returns number of records that are being returned
  char * get(int db, int rec, int field)
    {
      assert( (db > -1) && (rec > -1));
      SString grab("GET ");
      SString d(db);
      SString r(rec);
      SString f(field);
      grab += d;
      grab += ' ';
      grab += r;
      grab += ' ';
      grab += f;
      grab += '\r';
      grab += '\n';
      cout << "Writing: " << grab;// << "~\n";
      c.Writer(grab.GetC(), grab.Len()+1);

      char * p;
      while (  *(p = c.Reader()) == '~');

      cout << "P : " << p << endl;
      SString message("",false);
      SString command(p);

      int i = 1;
      for (; command.Char(i) != 'D'; i++);	

      int s = 0;
      for (; s < 4; i++)	
	if (command.Char(i) == ' ') s++;

      for (; (command.Char(i) && (command.Char(i) != '\n')) ; i++)	
	message += command.Char(i);
      
      return message.GetC();      
    }


 // Returns number of records that are being returned
  bool put(int db, int rec, int field, char * data)
    {

      assert( (db > -1) && (rec > -1));
      SString grab("PUT 0 0 ");
      SString d(db);
      SString r(rec);
      SString f(field);
      SString dat(data);
      grab += d;
      grab += ' ';
      grab += r;
      grab += ' ';
      grab += f;
      grab += ' ';
      grab += data;
      grab += '\r';
      grab += '\n';
      c.Writer(grab.GetC(), grab.Len()+1);
      char * p;
      while (  *(p = c.Reader()) == '~');
      //char * p = parse();
      if (*p == 'A')
	return true;
      else 
	return false;
    }

  int Ready()
    {
      SString grab("RDY\n");
      c.Writer(grab.GetC(), grab.Len());

      char * p;
      while ((p=parse()) == NULL);
      if (*p == 'B')
	return true;
      else 
	return false;
    }

  int tokendone()
    {
      SString grab("DUN\n");
      c.Writer(grab.GetC(), grab.Len());

      char * p;
      while ((p=parse()) == NULL);
      if (*p == 'B')
	return true;
      else 
	return false;
    }

  char * Getstatus()
    {
      char * sta;
      statusm = NULL;
      return sta; 
    }
  
  void IYTwait()
    {
      SString iyt("IYT");
      SString word("");
      
      while (!(word == iyt))
	{
	  char * result;
	  if ((result = c.Reader()) != NULL)
	    {
	      SString command(result);
	      SString iyt("IYT");
	      SString word;
	      int i;
	      for (i = 1; (command.Char(i)) && (command.Char(i) != ' '); i++)
		word += toupper(command.Char(i));
	      cout << "word: " << word << endl;
	      SString empty;
	      word = empty;
	    }
	}
    }

  char * parse()
    {
      cout << "Starting parse" << endl;
      char * result;
      if ((result = c.Reader()) != NULL)
	{
	  SString command(result);
	  cout << "Command : " << command << endl;
	  // Cannonical list of client words
	  SString msg("MSG"); // Receive a message
	  SString inf("INF"); // Receive player nfo
	  SString bgn("BGN"); // Game Play is starting!
	  SString pok("POK"); // Player name confirmation
	  SString err("ERR"); // Error

	  SString nyt("NYT");
	  SString nrf("NRF");
	  SString fld("FLD");
	  SString aok("AOK");
	  SString iyt("IYT");

	  // Extracts the first word

	  SString word;
	  int i;
	  for (i = 1; (command.Char(i)) && (command.Char(i) != ' '); i++)
	    word += toupper(command.Char(i));
	  cout << "word: " << word << endl;

	  if (word.Char(1) == '~')
	    {
	      SString message("",false);
	      i++;
	      for (;command.Char(i); i++)	
		message += command.Char(i);
	      if (statusm != NULL)
		{
		  cout << "WARNING - Overwriting status information\n";
		  delete [] statusm;
		  cout << "hello2\n";
		}
	      statusm = message.GetC();
	      return parse();
	    }
	  else
	  if ((word == err) || (word == msg) || (word == inf)) // Player information being sent
	    {
	      SString message("",false);

	      i++;
	      for (;command.Char(i); i++)	
		message += command.Char(i);
	      
	      return message.GetC();
	    }
	  else
	    if (word == pok) // Player confirmation
	    {
	      SString message("Player Submission Confirmed",false);
	      psetup = true;
	      return message.GetC();
	    }
	  else
	    if (word == bgn) // Player information being sent
	    {
	      mode = 1;
	      SString message("Game is about to begin",false);
	      return message.GetC();
	    }
	  else
	    if (word == nyt) // Player confirmation
	    {
	      SString message("It's Not Your Turn",false);
	      return message.GetC();
	    }
	  else
	    if (word == aok) // Player confirmation
	    {
	      SString message("AOK -> Move Confirmed",false);
	      return message.GetC();
	    }
	  else
	  if (word == nrf)
	    {
	      cout << "Receiving data" << endl;
	      
	      SString message("/",false);

	      i++;
	      for (;command.Char(i); i++)	
		message += command.Char(i);
	      
	      return message.GetC();
	    }
	  else
	    {
	      SString message("/?",false);
	      return message.GetC();
	    }
	}
      return NULL;
    }
};


#endif
