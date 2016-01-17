#ifndef gamec
#define gamec

#include "gamec.h"

class Client {

private:
  Sock c;
  
  char * statusm;

  int msize, psize;
  bool minit, pinit;
  int myplayer;
  int turn;
  bool psetup;
  bool mode;        // 0 = init state 1 = gamestate

public:
  Client() {msize = 0; psize = 0; minit = false; pinit = false; turn = -2; psetup = false; mode = 0;}
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
      output += '\n';

      if (c.Writer(output.GetC(), output.Len()) > 0)
	return 0;  // Successful
      else 
	return -1;  // Unsuccessful
    }

  // Returns number of records that are being returned
  char * Get(int db, int rec, int field)
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
      grab += '\n';
      c.Writer(grab.GetC(), grab.Len());

      char * p;
      while ((p = parse()) != NULL);
 
      SString message("",false);
      SString command(p);

      int i = 1;
      for (; command.Char(i) != 'D'; i++);	
	//message += command.Char(i);

      int s = 0;
      for (; s < 3; i++)	
	if (command.Char(i) == ' ') s++;

      for (; (command.Char(i) && (command.Char(i) != '\n')) ; i++)	
	message += command.Char(i);
      
      return message.GetC();      
    }


 // Returns number of records that are being returned
  bool Put(int db, int rec, int field, char * data)
    {

      assert( (db > -1) && (rec > -1));
      SString grab("PUT ");
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
      grab += '\n';
      c.Writer(grab.GetC(), grab.Len());

      char * p = parse();
      if (*p == 'A')
	return true;
      else 
	return false;
    }

  int Ready()
    {
      SString grab("RDY\n");
      c.Writer(grab.GetC(), grab.Len());

      char * p = parse();
      if (*p == 'B')
	return true;
      else 
	return false;
    }


  void Update()
    {
      SString output("mod 0 ");
      SString time(turn);
      output += time;
      c.Writer(output.GetC(),output.Len());

      SString output2("mod 1 ");
      output2 += time;
      c.Writer(output2.GetC(),output2.Len());
    }

  /*  Player * Getplayers() {Update(); return p;}
  Monster * Getmonsters() {Update(); return m;}
  char ** Getmap() {return map;}
  */
  int Nummonsters() {return msize;}
  int Numplayer() {return psize;}

  char * Getstatus()
    {
      //if (statusm == NULL)
      //parse();
      char * sta;
      statusm = NULL;
      return sta; 
    }

  char * parse()
    {
      char * result;
      //result = new char[10000];
      //SString command;
      if ((result = c.Reader()) != NULL)
	{
	  //cout << "res: " << result << endl;
	  SString command(result);
	  //cout << "Command : " << command << endl;
	  // Cannonical list of client words
	  SString msg("MSG"); // Receive a message
	  SString inf("INF"); // Receive player nfo
	  SString bgn("BGN"); // Game Play is starting!
	  SString pok("POK"); // Player name confirmation
	  SString err("ERR"); // Error

	  SString nyt("NYT");
	  SString nrf("NRF");
	  SString fld("FLD");
	  //SString map("MAP");
	  SString aok("AOK");
	  SString iyt("IYT");

	  // Extracts the first word

	  SString word;
	  int i;
	  for (i = 1; (command.Char(i)) && isalpha(command.Char(i)); i++)
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
		}
	      statusm = command.GetC();
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
	      
	      //map = new char*[22];
	      //for (int i = 0; i < 22; i++)
	      //map[i] = new char[64];

	      //p = new Player[8];

	      SString output("mod 1 -1\n");

	      if (c.Writer(output.GetC(), output.Len()) > 0)
		cout << "Mod sent success." << endl;
	      else
		cout << "Mod unsuccessful." << endl;

	      SString output2("mod 0 -1\n");
	      c.Writer(output2.GetC(), output2.Len());
	

	      //      c.Writer(output.GetC(), output.Len()+1);
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
	      SString message("/a Move Confirmed",false);
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


	      int datab = -1;

	      SString time;
	      SString output("/u",false);

	      for (;(command.Char(i) && (command.Char(i) != '\n')); i++)
		time += command.Char(i);
	      turn = atoi(time.GetC());

	      if (!command.Char(i))
		return output.GetC();

	      i++;
	      while (command.Char(i))
		{
		  for (;(command.Char(i) != ' '); i++);
		  i++;
		  
		  SString db;
		  for (;(command.Char(i) && (command.Char(i) != ' ')); i++)	
		    db += command.Char(i);
		  int dbnum = atoi(db.GetC());
		  datab = dbnum;

		  i++;
		  SString rec;
		  for (;(command.Char(i) && (command.Char(i) != ' ')); i++)	
		    rec += command.Char(i);
		  int recnum = atoi(rec.GetC());
		  psize = recnum;

		  i++;
		  SString field;
		  for (;(command.Char(i) && (command.Char(i) != ' ')); i++)	
		    field += command.Char(i);
		  int fieldnum = atoi(field.GetC());

		  i++;
		  SString data;
		  for (;(command.Char(i) && (command.Char(i) != '\n')); i++)	
		    data += command.Char(i);
		  int datanum = atoi(data.GetC());  

		  if (dbnum == 1)
		    {
		      //strcpy(map[recnum],data.GetC());
		      //cout << recnum << " - " << map[recnum] << endl;
		      //recnum++;
		      if (recnum == 21) break;
		    }

		  if (dbnum == 0)
		    {
		      /*if (fieldnum == 0) players[recnum].setX(datanum); 
		     if (fieldnum == 1) players[recnum].setY(datanum); 
		     if (fieldnum == 2) players[recnum].setHP(datanum); 
		     if (fieldnum == 3) players[recnum].setXP(datanum); 
		     if (fieldnum == 4) players[recnum].setAC(datanum); 
		     if (fieldnum == 5) players[recnum].setDMG(datanum); 
		     if (fieldnum == 6) players[recnum].setkills(datanum); 
		     if (fieldnum == 7) players[recnum].setname(data.GetC());
		     if (fieldnum == 8) players[recnum].setweapon(data.GetC());
		     if (fieldnum == 9) players[recnum].setarmor(data.GetC());
		     if (fieldnum == 9) break;*/
		    }

		  if (command.Char(i) == '\n')
		    i++;
		}

	      if (datab == 1)
		{
		  SString outp("/m", false);
		  //	  delete [] result;
		  return outp.GetC();
		}
	      if (datab == 0)
		{
		  SString outp("/p", false);
		  //delete [] result;
		  return outp.GetC();
		}	      
	    }
	  else
	    {
	      SString message("/?",false);
	      //delete [] result;
	      return message.GetC();
	    }
	}
      //delete [] result;
      return NULL;
    }
};


#endif
