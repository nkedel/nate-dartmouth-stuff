// String.h

// String ADT Header
// Creates a string object similar to a C-style character array, but with many
// convenient access functions.

// Modification history:
//   Written by Keith Kotay on October 10, 1995 
//   Extra commands added by David Marmaros in Winter 1998.
#ifndef STRING_H
#define STRING_H

#include <iostream>

class SString
	{
	private:
	  bool on;
	  char * pString;
	public:
	  SString(bool b = true);										
	    // constructor
	  SString(char * pChar, bool b = true);								
	    // constructor
	  SString(char * pChar, unsigned int Length, bool b = true);	
	    // constructor - first Length characters of pChar
	  SString(const SString & S, bool b = true); 								
	    // constructor
	  SString(int value, bool b = true); 
	  ~SString(void);										
	  // destructor
	       void set(bool b) {on = b;}
	       void         operator=(const SString & S); 							
	       // assignment operator
	       //void operator+=(int i);
		    void         operator+=(char C); 								// concatenates a character to a String
																										     void         operator+=(SString & S); 							// concatenates a String to a String
		       void         operator+=(char * C);
		       char	    Char(unsigned int I); 							// returns the ith character in a String 
		       SString		  Head(unsigned int Length);						// returns the first N characters in a String
		       unsigned int Len(void);											// returns the length of a String
		       SString       Tail(unsigned int Length);						// returns the last N characters in a String
		       char *       GetC()
			 {return pString;}
		friend bool      operator==(SString & S1, SString & S2);		// tests Strings for equality
		friend ostream & operator<<(ostream & O, const SString & S);			// displays a String
		friend istream & operator>>(istream & I, SString & S); 
	};
		
#endif // STRING_H
