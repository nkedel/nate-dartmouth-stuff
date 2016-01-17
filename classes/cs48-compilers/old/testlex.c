#include <stdio.h>
extern int yylex();
extern char *yytext;

int main()
{
  int token;

  do
    {
      token = yylex();
      printf("%d: \"%s\"\n", token, yytext);
    }
  while (token != 0);

  return 0;
}
