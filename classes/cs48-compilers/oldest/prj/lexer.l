%{
/* lexer.l / lexer.yy.c
** Nate Edel, 13 Apr 1999
**
** flex definition for the C48 lexical analyzer 
*/

#include "tokens.h"
#include "lexer.h"
%}

%option stack
%x comment

DIGIT   [0-9]
LETTER  [A-Za-z]
ID      (_|{LETTER})(_|{LETTER}|{DIGIT})*
SPACE   (" "|\t|\n)
I_NUM	{DIGIT}+
F_NUM	{DIGIT}+"."{DIGIT}+
ESCAPES (a|b|f|n|r|t|v|\\|\'|\"|0)
C_NUM (\'.\')|(\'\\{ESCAPES}\')

%%
{ID}    {
	/* first check if we have a reserved word */
	int i;
	for (i=0; i < N_RES_WORDS; i++) 
		if (yytext[0] == res_words[i][0])  /* check first character
						    * faster than strcmp()?
						    */
			if (strcmp(yytext,res_words[i]) == 0)
				return(t_res_words[i]); /* return the token */
	
	/* if not a reserved word, then we have an id - return it */
	return(IDENT);	
	}

{F_NUM} { return(FNUM); }
{I_NUM} { return(INUM); }
{C_NUM} { return(INUM); }

"<=" { return(LTOE);   }
">=" { return(GTOE);   }
"==" { return(CEQUAL); }
"!=" { return(NEQUAL); }
"++" { return(INCR);   }
"--" { return(DECR);   }
"&&" { return(LAND);   }
"||" { return(LOR);    }

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
        if ((c = input()) == '/') yy_pop_state();
        }
                        
<comment>[^\*]  ;

{SPACE} ;

.	{ return (INVALID); }
