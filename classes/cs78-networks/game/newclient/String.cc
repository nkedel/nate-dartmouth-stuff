// String.cp

// String ADT Module

// Modification history:
//		Written by Keith Kotay on October 10, 1995 

#include <string.h>

#include "String.h"
#include <stdio.h>

SString::SString(bool b)
{
  on = b;
  pString = new char;
  *pString = '\0';
}
	
SString::SString(char * pChar, bool b)
{
  on = b;
  pString = new char[strlen(pChar) + 1];
  (void) strcpy(pString, pChar);
}
	
SString::SString(char * pChar, unsigned int Length, bool b)
{
  on = b;
  pString = new char[Length + 1];
  (void) strncpy(pString, pChar, Length);
  *(pString + Length) = '\0';
}
	
SString::SString(const SString & S, bool b)
{
  on = b;
  pString = new char[strlen(S.pString) + 1];
  (void) strcpy(pString, S.pString);
}

SString::SString(int value, bool b)
{
  on = b;
  pString = new char[15];
  sprintf(pString, "%d", value);

  /*  int i = 1;
  int num = 1;

  while (i*10 <= value)
    {i *= 10; num++;}

  pString = new char[1 + num];

  int pos = 0;

  do 
    {
      *(pString + pos) = char('0'+ (value / i));
      value %= i;
      i /= 10;
      pos++;
      num--;
    } while (num > 0);

  *(pString + pos) = '\0';*/
}
	
SString::~SString(void)
{
  if (on)
    delete [] pString;
}

	
void
SString::operator=(const SString & S)
{
  if (this != &S)
    {
      delete [] pString;
      pString = new char[strlen(S.pString) + 1];
      (void) strcpy(pString, S.pString);
    }
}


void
SString::operator+=(char C)
{
  char * pNewString;
  pNewString = new char[strlen(pString) + 2];
  (void) strcpy(pNewString, pString);
  *(pNewString + strlen(pString)) = C;
  *(pNewString + strlen(pString) + 1) = '\0';
  delete [] pString;
  pString = pNewString;
}

/*
void
SString::operator+=(int i)
{
  char * pNewerString = new char[15];
  sprintf(pNewerString, "%d", i);
  
  char * pNewString;

  pNewString = new char[strlen(pString) + strlen(pNewerString) + 1];
  (void) strcpy(pNewString, pString);
  (void) strcat(pNewString, pNewerString);
  delete [] pString;
  delete [] pNewerString;
  pString = pNewString;
}
*/
void
SString::operator+=(SString & S)
{
  char * pNewString;
  pNewString = new char[strlen(pString) + strlen(S.pString) + 1];
  (void) strcpy(pNewString, pString);
  (void) strcat(pNewString, S.pString);
  delete [] pString;
  pString = pNewString;
}


void
SString::operator+=(char * c)
{
  char * pNewString;
  pNewString = new char[strlen(pString) + strlen(c) + 1];
  (void) strcpy(pNewString, pString);
  (void) strcat(pNewString, c);
  delete [] pString;
  pString = pNewString;
}

char
SString::Char(unsigned int I)
{
  if ((I > 0) && (I <= strlen(pString)))
    return (*(pString + I - 1));
  else
    return ('\0');
}
	
SString
SString::Head(unsigned int Length)
{
  if (Length <= strlen(pString))
    {
      SString S(pString, Length);
      return (S);
    }
  else
    return (*this);
}
	
unsigned int
SString::Len(void)
{
  return (strlen(pString));
}

SString
SString::Tail(unsigned int Length)
{
  if (Length <= strlen(pString))
    {
      SString S(pString + strlen(pString) - Length, Length);
      return (S);
    }
  else
    {
      return (*this);
    }
}

bool
operator==(SString & S1, SString & S2)
{
  return (strcmp(S1.pString, S2.pString) == 0);
}

	ostream &
operator<<(ostream & O, const SString & S)
{
  return (O << S.pString);
}
