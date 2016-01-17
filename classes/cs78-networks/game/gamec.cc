#define gamec
#include "sockcom.h"
#include "gamec.h"
#include "assert.h"

// game network client; based on chat client by David Marmaros, modified for
// network game by David Marmaros and myself (Nate Edel), using David's
// socket class

Client::Client() {turn = -2; mode = 0;}
Client::~Client() {}

int Client::Connect(char * host, int port)
    {
      return c.Connect(host, port);
    }

int Client::Say(char * message)
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

void Client::flush(void) {
  char *x;
  while (x = c.Reader()) ;
  }

int Client::Sendstats(char * name, char * charfile, char * password)
    {
      SString output("PLR ");
      output += name;
      output += ' ';
      output += charfile;
      output += ' ';
      output += password;
//      output += '\r';
      output += '\n';

      if (c.Writer(output.GetC(), output.Len()+1) > 0)
	return 0;  // Successful
      else 
	return -1;  // Unsuccessful
    }

  // Returns number of records that are being returned
char * Client::get(int db, int rec, int field)
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
//      grab += '\r';
      grab += '\n';
      c.Writer(grab.GetC(), grab.Len()+1);
      
      char * p = c.Reader();
      if (rec == 0) print_local(p);
      
      //	// // cout << "P : " << p << endl;
      SString message("",false);
      SString command(p);

      int i = 1;
      for (; command.Char(i) && command.Char(i) != 'D'; i++);	
  
      if (!command.Char(i)) return NULL;

      int s = 0;
      for (; s < 4; i++)	
      	if (command.Char(i) == ' ') s++;

      for (; (command.Char(i) && (command.Char(i) != '\n')) ; i++)	
	message += command.Char(i);
      
      return message.GetC();      
    }


 // Returns number of records that are being returned
bool Client::put(int db, int rec, int field, char * data)
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
  //    grab += '\r';
      grab += '\n';
      c.Writer(grab.GetC(), grab.Len()+1);
      
      char * p;
      while (  *(p = c.Reader()) != 'A');
    	return true;
    }

int Client::Ready()
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

int Client::tokendone()
    {
      SString grab("DUN\r\n");
      c.Writer(grab.GetC(), grab.Len()+1);

      char * p;
      while ((p=parse()) == NULL);
      if (*p == 'A')
	      return true;
      else 
	      return false;
      }

void Client::requesttoken() {
      SString grab("IMT\r\n");
      c.Writer(grab.GetC(), grab.Len()+1);
      }


char * Client::Getstatus()
    {
      char * sta;
      statusm = NULL;
      return sta; 
    }
  
bool Client::IYTwait() {
      SString iyt("IYT");
      SString word("");
      
      while (word.Len() < 4) {
	      char * result;
	      if ((result = c.Reader()) != NULL) {
  	      SString command(result) ;
          SString empty;
          word = empty;	  
	        for (int i = 1; (command.Char(i)) && (command.Char(i) != ' '); i++)
		        word += toupper(command.Char(i));
// // cout << "word: " << word << endl;
          if (word == iyt) return true;
	        }
	      }     
      return false;
      }

char * Client::parse()
    {
      //	// // cout << "Starting parse" << endl;
      char * result;
      if ((result = c.Reader()) != NULL)
	{
	  SString command(result);
	  //	// // cout << "Command : " << command << endl;
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
	  //	// // cout << "word: " << word << endl;

	  if (word.Char(1) == '~')
	    {
	      SString message("",false);
	      i++;
	      for (;command.Char(i); i++)	
		message += command.Char(i);
	      if (statusm != NULL)
		{
		  //	// // cout << "WARNING - Overwriting status information\n";
		  delete [] statusm;
		  //	// // cout << "hello2\n";
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
	      //	// // cout << "Receiving data" << endl;
	      
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

Client net;
