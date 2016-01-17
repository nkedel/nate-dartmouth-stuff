/* 
** tokens.h
** Nate Edel, 13 Apr 1999
**
** list of tokens for lexer
*/ 

#define BASE   258

/* single character tokens */
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



/* longer tokens */
#define INVALID BASE+ 0
#define IDENT  BASE + 1
#define INUM   BASE + 2
#define FNUM   BASE + 3

/* reserved words */
#define RES    BASE + 4  /* This is not a token, but a category */
#define IF     RES + 0
#define ELSE   RES + 1
#define INT    RES + 2
#define VOID   RES + 3
#define CHAR   RES + 4
#define DOUBLE RES + 5
#define RETURN RES + 6
#define WHILE  RES + 7
#define DO     RES + 8
#define FOR    RES + 9

/* multi-byte symbols */
#define SYM    BASE + 14
#define LTOE   SYM + 0        /* <= */
#define GTOE   SYM + 1        /* >= */
#define CEQUAL SYM + 2        /* == */
#define NEQUAL SYM + 3        /* != */
#define INCR   SYM + 4        /* ++ */
#define DECR   SYM + 5        /* -- */
#define LAND   SYM + 6        /* && */
#define LOR    SYM + 7        /* || */

/* escape sequences -- not needed? */
#define ESC    BASE + 22
#define LF     ESC + 0        /* \n */
#define TAB    ESC + 1        /* \t */
#define VTAB   ESC + 2        /* \v */
#define BS     ESC + 3        /* \b */
#define CR     ESC + 4        /* \r */
#define FF     ESC + 5        /* \f */
#define BELL   ESC + 6        /* \a */
#define CBSLASH ESC + 7       /* \\ */
#define CSQUOTE ESC + 8       /* \' */
#define CDQUOTE ESC + 9       /* \" */
#define CNULL  ESC + 10       /* \0 */
