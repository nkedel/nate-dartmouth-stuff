%{
#include <stdlib.h>

int deref_c = 0;
%}

%x comment
%s dereference
DECDIGIT [0-9]
OCTDIGIT [0-7]
HEXDIGIT [0-9A-Fa-f]
HEXNUM 0x{HEXDIGIT}+
OCTNUM 0{OCTDIGIT}+
DECNUM ([1-9]{DECDIGIT}*)|(0{DECDIGIT}*(8|9){DECDIGIT}*)
IDENT  [A-Za-z_][A-Za-z0-9_]*

%%
"/*"	BEGIN(comment);

<comment>\* {
	int c;
	if ((c = input()) == '/') BEGIN(INITIAL);
	}

<comment>[^\*]	;

{DECNUM}	{
	long ournumber;
	ournumber=strtol(yytext,NULL,10);
	fprintf(yyout,"0x%x",ournumber);
	}

{OCTNUM}	{
	long ournumber;
	ournumber=strtol(yytext,NULL,8);
	fprintf(yyout,"0x%x",ournumber);
	}

{HEXNUM}	{
	long ournumber;
	ournumber=strtol(yytext,NULL,16);
	fprintf(yyout,"0x%x",ournumber);
	}

{IDENT}"[" 	{
	int c;
	while ((c = input()) == ' ' || c == '\t' || c == '\n' || c=='/') { 
/* badly obfuscated code: checks for and ignores comments within []s */
		register int d;
		if (c=='/') {
			if ((d=input()) == '*') {
				while (1) if ((c=input()) == '*' || c==EOF)
					if ((d=input())=='/' || d==EOF) break;
				}
				else {
					unput(d);
					unput(c);
					break;
					}
			}
/* end badly obfuscated code */

		};	
	if (c == ']') fprintf(yyout, "%s]", yytext); 
	   else { 
		unput(c);
		deref_c++; 
		yytext[--yyleng]=0; 
		fprintf(yyout, "*(%s+(",yytext);
		}
	}

"]"	{ 
	if (deref_c == 0) { fprintf(stderr,"Unmatched square bracket!"); ECHO; }
	   else { fprintf(yyout, "))"); deref_c--; }
	}

{IDENT} ECHO;
.	ECHO;

%%
int main(int argc, char **argv) {
	if (argc==2) yyin=fopen(argv[1],"r");
		else yyin = stdin;
	yylex();
	}
	 