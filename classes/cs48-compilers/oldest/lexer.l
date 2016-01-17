%{
/* lexer.l / lexer.yy.c
** Nate Edel, 13 Apr 1999
**
** flex definition for the C48 lexical analyzer 
*/

#include "parse.tab.h"
#include "lexer.h"
int lines;

%}

%option stack
%x comment

DIGIT   [0-9]
LETTER  [A-Za-z]
T_ID      (_|{LETTER})(_|{LETTER}|{DIGIT})*
SPACE   (" "|\t)
NEWLINE "\n"
I_NUM	{DIGIT}+
F_NUM	{DIGIT}+"."{DIGIT}+
ESCAPES (a|b|f|n|r|t|v|\\|\'|\"|0)
C_NUM (\'.\')|(\'\\{ESCAPES}\')

%%
{T_ID}    {
	/* first check if we have a reserved word */
	int i;
	for (i=0; i < N_RES_WORDS; i++) 
		if (strcmp(yytext,res_words[i]) == 0)
			return(t_res_words[i]); /* return the token */
	
	/* if not a reserved word, then we have an id - return it */
	return(ID);	
	}

{F_NUM} { return(FNUM); }
{I_NUM} { return(NUM);  }
{C_NUM} { return(NUM);  }

"<=" { return(LTE);    }
">=" { return(GTE);    }
"==" { return(CEQ);    }
"!=" { return(NEQ);    }
"++" { return(INCR);   }
"--" { return(DECR);   }
"&&" { return(L_AND);  }
"||" { return(L_OR);   }

"+"  { return(PLUS);   }
"-"  { return(MINUS);  }
"*"  { return(TIMES);  }
"/"  { return(SLASH);  }
"<"  { return(LTHAN);  }
">"  { return(GTHAN);  }
"["  { return(LBRACK); }
"]"  { return(RBRACK); }
"="  { return(EQUAL);  }
";"  { return(SEMI);   }
","  { return(COMMA);  }
"("  { return(LPAREN); }
")"  { return(RPAREN); }
"{"  { return(LSQB);   }
"}"  { return(RSQB);   }
\'   { return(SQUOTE); }
\"   { return(DQUOTE); }
"!"  { return(BANG);   }

"/*"    yy_push_state(comment);

<comment>\* {
        int c;
        do 
		if ((c = input()) == '/') { yy_pop_state(); break; }
        while (c=='*');
	}
                        
<comment>[^\*]  ;
<INITIAL,comment>{NEWLINE} lines++;

{SPACE} ;

.	{ return (yytext[0]); }

<INITIAL,comment><<EOF>> { return 0; }
