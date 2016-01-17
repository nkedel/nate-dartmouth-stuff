/* lexer.c
** Nate Edel, 13 Apr 1999
**
** defines a few things we'll need that should be mixed in with the tokens
*/

/* number of reserved words */
#define N_RES_WORDS	 13

/* reserved words */
#define S_IF     "if"
#define S_ELSE   "else"
#define S_INT    "int"
#define S_VOID   "void"
#define S_CHAR   "char"
#define S_DOUBLE "double"
#define S_RETURN "return"
#define S_WHILE  "while"
#define S_DO     "do"
#define S_FOR    "for"
#define S_READ   "read"
#define S_WRITE  "write"
#define S_BREAK  "break"
#define S_CONTINUE  "continue"

char *res_words[] = { S_IF, S_ELSE, S_INT, S_VOID, S_CHAR, 
	S_DOUBLE, S_RETURN, S_WHILE, S_DO, S_FOR, S_READ, S_WRITE, 
	S_BREAK, S_CONTINUE };

int t_res_words[] = { IF, ELSE, INT, VOID, CHAR, DOUBLE,
	RETURN, WHILE, DO, FOR, READ, WRITE, BREAK, CONTINUE };

/* single character tokens:
 * because I defined 'em symbolically in lexer.l, and bash doesn't 
 * output these to foo.tab.h 
 */
#define PLUS   '+'
#define MINUS  '-'
#define TIMES  '*'
#define SLASH  '/'
#define LTHAN  '<'
#define GTHAN  '>'
#define LBRACK '['
#define RBRACK ']'
#define EQUAL  '='
#define SEMI   ';'
#define COMMA  ','
#define LPAREN '('
#define RPAREN ')'
#define LSQB   '{'
#define RSQB   '}'
#define SQUOTE '\''
#define DQUOTE '\"'
#define BANG   '!'

/* we may not need this stuff */
/* escape sequence ascii values */
#define NUM_ESCAPES 11

#define V_LF     '\n'
#define V_TAB    '\t'
#define V_VTAB   '\v'
#define V_BS     '\b'
#define V_CR     '\r'
#define V_FF     '\f'
#define V_BELL	 '\a'
#define V_CBSLASH '\\'
#define V_CSQUOTE '\''
#define V_CDQUOTE '\"'
#define V_CNULL  '\0'

/* character only for escape sequences */
#define C_LF     'n'
#define C_TAB    't'
#define C_VTAB   'v'
#define C_BS     'b'
#define C_CR     'r'
#define C_FF     'f'
#define C_BELL	 'a'
#define C_CBSLASH '\\'
#define C_CSQUOTE '\''
#define C_CDQUOTE '\"'
#define C_CNULL  '0'

char escapes[] = { C_LF, C_TAB, C_VTAB, C_BS, C_CR, C_FF, C_BELL, C_CBSLASH, 
	C_CSQUOTE, C_CDQUOTE, C_CNULL };

char t_escapes[] = { V_LF, V_TAB, V_VTAB, V_BS, V_CR, V_FF, V_BELL, V_CBSLASH, 
	V_CSQUOTE, V_CDQUOTE, V_CNULL };
 