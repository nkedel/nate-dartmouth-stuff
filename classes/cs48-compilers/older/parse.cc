
/*  A Bison parser, made from parse.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	INT	258
#define	VOID	259
#define	DOUBLE	260
#define	CHAR	261
#define	ID	262
#define	NUM	263
#define	FNUM	264
#define	IF	265
#define	ELSE	266
#define	FOR	267
#define	DO	268
#define	WHILE	269
#define	RETURN	270
#define	L_OR	271
#define	L_AND	272
#define	NEQ	273
#define	CEQ	274
#define	LTE	275
#define	GTE	276
#define	UMINUS	277
#define	DECR	278
#define	INCR	279
#define	SUB	280
#define	CALL	281
#define	FN_DEF	282
#define	FN_DECL	283

#line 1 "parse.y"

// initial lib choices snarfed from class examples
// much of this initial code based on class "tree" examples  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "treenode.h"

#define YYSTYPE pTreeNode
#define YYDEBUG 1

extern "C" int lines;
extern "C" int yylex();
extern "C" void yyerror(char *);
extern "C" char *yytext;
extern "C" char *lastLIT;
extern "C" char *lastID;

extern void DMESG(int t, const char *m);
extern pTreeNode root;
extern bool Error;
#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		146
#define	YYFLAG		-32768
#define	YYNTBASE	46

#define YYTRANSLATE(x) ((unsigned)(x) <= 283 ? yytranslate[x] : 76)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    30,     2,     2,     2,    28,     2,     2,    41,
    42,    26,    24,    38,    25,     2,    27,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    37,    22,
    45,    23,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    39,     2,    40,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    43,     2,    44,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    29,    31,    32,    33,
    34,    35,    36
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    17,    19,    21,
    23,    25,    29,    31,    33,    38,    45,    52,    54,    56,
    60,    62,    65,    70,    75,    76,    79,    80,    83,    85,
    87,    89,    91,    93,    95,    97,   100,   102,   108,   116,
   126,   127,   129,   137,   143,   146,   150,   152,   157,   161,
   163,   167,   171,   175,   179,   183,   187,   191,   195,   199,
   203,   207,   211,   215,   218,   221,   224,   227,   229,   233,
   235,   237,   239,   241,   246,   247,   249,   253
};

static const short yyrhs[] = {    47,
     0,    47,    48,     0,    48,     0,    49,     0,    54,     0,
    53,     0,    50,    51,    37,     0,     3,     0,     4,     0,
     5,     0,     6,     0,    51,    38,    52,     0,    52,     0,
     7,     0,     7,    39,     8,    40,     0,    50,     7,    41,
    55,    42,    37,     0,    50,     7,    41,    55,    42,    58,
     0,    56,     0,     4,     0,    56,    38,    57,     0,    57,
     0,    50,     7,     0,    50,     7,    39,    40,     0,    43,
    59,    60,    44,     0,     0,    59,    49,     0,     0,    60,
    61,     0,    62,     0,    58,     0,    63,     0,    64,     0,
    66,     0,    67,     0,    68,     0,    70,    37,     0,    37,
     0,    10,    41,    70,    42,    61,     0,    10,    41,    70,
    42,    61,    11,    61,     0,    12,    41,    65,    37,    65,
    37,    65,    42,    61,     0,     0,    70,     0,    13,    61,
    14,    41,    70,    42,    37,     0,    14,    41,    70,    42,
    61,     0,    15,    37,     0,    15,    70,    37,     0,     7,
     0,     7,    39,    70,    40,     0,    69,    45,    70,     0,
    71,     0,    71,    24,    71,     0,    71,    25,    71,     0,
    71,    26,    71,     0,    71,    27,    71,     0,    71,    28,
    71,     0,    71,    21,    71,     0,    71,    20,    71,     0,
    71,    22,    71,     0,    71,    23,    71,     0,    71,    19,
    71,     0,    71,    18,    71,     0,    71,    16,    71,     0,
    71,    17,    71,     0,    25,    72,     0,    30,    72,     0,
    32,    69,     0,    31,    69,     0,    72,     0,    41,    70,
    42,     0,    69,     0,    73,     0,     8,     0,     9,     0,
     7,    41,    74,    42,     0,     0,    75,     0,    75,    38,
    70,     0,    70,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    59,    65,    76,    82,    83,    84,    87,    94,    95,    96,
    97,   100,   111,   114,   118,   125,   133,   142,   146,   152,
   163,   169,   174,   181,   191,   192,   204,   205,   217,   218,
   219,   220,   221,   222,   223,   226,   227,   230,   236,   245,
   255,   258,   264,   272,   281,   285,   292,   296,   303,   309,
   315,   321,   327,   333,   339,   345,   351,   357,   363,   369,
   375,   381,   387,   393,   398,   403,   409,   414,   416,   417,
   418,   419,   420,   423,   430,   431,   434,   445
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","INT","VOID",
"DOUBLE","CHAR","ID","NUM","FNUM","IF","ELSE","FOR","DO","WHILE","RETURN","L_OR",
"L_AND","NEQ","CEQ","LTE","GTE","'<'","'>'","'+'","'-'","'*'","'/'","'%'","UMINUS",
"'!'","DECR","INCR","SUB","CALL","FN_DEF","FN_DECL","';'","','","'['","']'",
"'('","')'","'{'","'}'","'='","program","decl_list","decl","var_decl","type_spec",
"id_list","id_decl","fun_decl","fun_defn","params","param_list","param","comp_stmt",
"local_decl","stmt_list","statement","expr_stmt","if_stmt","for_stmt","for_expr",
"do_stmt","while_stmt","return_stmt","var","expr","exp","expval","call","args",
"arg_list", NULL
};
#endif

static const short yyr1[] = {     0,
    46,    47,    47,    48,    48,    48,    49,    50,    50,    50,
    50,    51,    51,    52,    52,    53,    54,    55,    55,    56,
    56,    57,    57,    58,    59,    59,    60,    60,    61,    61,
    61,    61,    61,    61,    61,    62,    62,    63,    63,    64,
    65,    65,    66,    67,    68,    68,    69,    69,    70,    70,
    71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
    71,    71,    71,    71,    71,    71,    71,    71,    72,    72,
    72,    72,    72,    73,    74,    74,    75,    75
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     3,     1,     1,     1,
     1,     3,     1,     1,     4,     6,     6,     1,     1,     3,
     1,     2,     4,     4,     0,     2,     0,     2,     1,     1,
     1,     1,     1,     1,     1,     2,     1,     5,     7,     9,
     0,     1,     7,     5,     2,     3,     1,     4,     3,     1,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     2,     2,     2,     2,     1,     3,     1,
     1,     1,     1,     4,     0,     1,     3,     1
};

static const short yydefact[] = {     0,
     8,     9,    10,    11,     1,     3,     4,     0,     6,     5,
     2,    14,     0,    13,     0,     0,     7,     0,     0,     9,
     0,     0,    18,    21,    14,    12,    15,    22,     0,     0,
     0,    16,    25,    17,    20,    23,    27,    26,     0,     0,
    47,    72,    73,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    37,     0,    24,    30,    28,    29,    31,    32,
    33,    34,    35,    70,     0,    50,    68,    71,     0,    75,
     0,    41,     0,     0,    45,     0,    70,    64,    65,    47,
    67,    66,     0,     0,    36,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,    78,
     0,    76,     0,     0,    42,     0,     0,    46,    69,    49,
    62,    63,    61,    60,    57,    56,    58,    59,    51,    52,
    53,    54,    55,    48,    74,     0,     0,    41,     0,     0,
    77,    38,     0,     0,    44,     0,    41,     0,    39,     0,
    43,     0,    40,     0,     0,     0
};

static const short yydefgoto[] = {   144,
     5,     6,     7,     8,    13,    14,     9,    10,    22,    23,
    24,    56,    37,    40,    57,    58,    59,    60,   104,    61,
    62,    63,    64,    65,    66,    67,    68,   101,   102
};

static const short yypact[] = {    33,
-32768,-32768,-32768,-32768,    33,-32768,-32768,    23,-32768,-32768,
-32768,   -27,   -21,-32768,    35,   113,-32768,    75,    49,    50,
    84,    51,    57,-32768,    62,-32768,-32768,    85,   -24,    33,
    83,-32768,-32768,-32768,-32768,-32768,    33,-32768,    75,    90,
     1,-32768,-32768,    87,    88,   128,    89,   142,    -6,    -6,
   118,   118,-32768,   155,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,    81,    95,    41,-32768,-32768,   155,   155,
   155,   155,   125,   155,-32768,   107,-32768,-32768,-32768,   106,
-32768,-32768,   104,   155,-32768,   155,   155,   155,   155,   155,
   155,   155,   155,   155,   155,   155,   155,   155,   108,-32768,
   105,   114,   112,   119,-32768,   116,   124,-32768,-32768,-32768,
   193,    52,    86,    86,    -4,    -4,    -4,    -4,     6,     6,
-32768,-32768,-32768,-32768,-32768,   155,   128,   155,   155,   128,
-32768,   144,   131,   133,-32768,   128,   155,   139,-32768,   135,
-32768,   128,-32768,   161,   170,-32768
};

static const short yypgoto[] = {-32768,
-32768,   173,   145,   -12,-32768,   163,-32768,-32768,-32768,-32768,
   154,   159,-32768,-32768,   -46,-32768,-32768,-32768,  -122,-32768,
-32768,-32768,   -42,   -43,   111,    38,-32768,-32768,-32768
};


#define	YYLAST		221


static const short yytable[] = {    73,
    41,    42,    43,    21,    76,   133,    77,    77,    81,    82,
    83,    15,    32,    16,   140,    17,    18,    21,    33,    94,
    95,    96,    97,    98,    39,    99,   100,   103,   105,    12,
   107,    96,    97,    98,    54,     1,     2,     3,     4,    69,
   110,    70,    19,    77,    77,    77,    77,    77,    77,    77,
    77,    77,    77,    77,    77,    77,    86,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    88,
    89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
   132,    25,   131,   135,   105,   134,    78,    79,    27,   139,
    28,   -19,    29,   105,    30,   143,    41,    42,    43,    44,
    15,    45,    46,    47,    48,    90,    91,    92,    93,    94,
    95,    96,    97,    98,    49,     1,    20,     3,     4,    50,
    51,    52,    36,    31,    80,    84,    53,    71,    72,    74,
    54,    85,    33,    55,    41,    42,    43,    44,   106,    45,
    46,    47,    48,   108,    69,   109,   125,   124,    41,    42,
    43,   126,    49,   127,   136,   128,   129,    50,    51,    52,
   145,    41,    42,    43,    53,   130,    49,   137,    54,   146,
    33,    50,    51,    52,   138,   141,   142,    11,    75,    49,
    26,    38,    54,    35,    50,    51,    52,    34,     0,     0,
     0,     0,     0,     0,     0,    54,   111,   112,   113,   114,
   115,   116,   117,   118,   119,   120,   121,   122,   123,    87,
    88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
    98
};

static const short yycheck[] = {    46,
     7,     8,     9,    16,    48,   128,    49,    50,    51,    52,
    54,    39,    37,    41,   137,    37,    38,    30,    43,    24,
    25,    26,    27,    28,    37,    69,    70,    71,    72,     7,
    74,    26,    27,    28,    41,     3,     4,     5,     6,    39,
    84,    41,     8,    86,    87,    88,    89,    90,    91,    92,
    93,    94,    95,    96,    97,    98,    16,    17,    18,    19,
    20,    21,    22,    23,    24,    25,    26,    27,    28,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
   127,     7,   126,   130,   128,   129,    49,    50,    40,   136,
     7,    42,    42,   137,    38,   142,     7,     8,     9,    10,
    39,    12,    13,    14,    15,    20,    21,    22,    23,    24,
    25,    26,    27,    28,    25,     3,     4,     5,     6,    30,
    31,    32,    40,    39,     7,    45,    37,    41,    41,    41,
    41,    37,    43,    44,     7,     8,     9,    10,    14,    12,
    13,    14,    15,    37,    39,    42,    42,    40,     7,     8,
     9,    38,    25,    42,    11,    37,    41,    30,    31,    32,
     0,     7,     8,     9,    37,    42,    25,    37,    41,     0,
    43,    30,    31,    32,    42,    37,    42,     5,    37,    25,
    18,    37,    41,    30,    30,    31,    32,    29,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    41,    86,    87,    88,    89,
    90,    91,    92,    93,    94,    95,    96,    97,    98,    17,
    18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    28
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 59 "parse.y"
{
    root = yyval = yyvsp[0];
    DMESG(0, "program -> decl_list");
    ;
    break;}
case 2:
#line 65 "parse.y"
{
        DMESG(0, "decl_list -> decl_list decl");
        pTreeNode tNode = yyvsp[-1];
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib(yyvsp[0]);
                yyval = yyvsp[-1];
                }
        else yyval = yyvsp[0];
        ;
    break;}
case 3:
#line 76 "parse.y"
{
    DMESG(0, "decl_list -> decl");
    yyval = yyvsp[0];
    ;
    break;}
case 4:
#line 82 "parse.y"
{ DMESG(0, "decl -> var_decl"); yyval = yyvsp[0]; ;
    break;}
case 5:
#line 83 "parse.y"
{ DMESG(0, "decl -> fun_defn"); yyval = yyvsp[0]; ;
    break;}
case 6:
#line 84 "parse.y"
{ DMESG(0, "decl -> fun_decl"); yyval = yyvsp[0]; ;
    break;}
case 7:
#line 87 "parse.y"
{
        DMESG(0, "var_decl -> type_spec id_list"); 
        yyval = yyvsp[-2];
        yyval->addChild(yyvsp[-1]);
        ;
    break;}
case 8:
#line 94 "parse.y"
{ yyval = new TreeNode(INT);   DMESG(INT,    "type_spec -> INT"); ;
    break;}
case 9:
#line 95 "parse.y"
{ yyval = new TreeNode(VOID);  DMESG(VOID,   "type_spec -> VOID"); ;
    break;}
case 10:
#line 96 "parse.y"
{ yyval = new TreeNode(DOUBLE);DMESG(DOUBLE, "type_spec -> DOUBLE"); ;
    break;}
case 11:
#line 97 "parse.y"
{ yyval = new TreeNode(CHAR);  DMESG(CHAR,   "type_spec -> CHAR"); ;
    break;}
case 12:
#line 100 "parse.y"
{
        pTreeNode tNode = yyvsp[-2];
        DMESG(0, "id_list -> id_list, id_decl");
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib(yyvsp[0]);
                yyval = yyvsp[-2];
                }
        else yyval = yyvsp[0];
        ;
    break;}
case 13:
#line 111 "parse.y"
{ yyval = yyvsp[0]; DMESG(0, "id_list -> id_decl"); ;
    break;}
case 14:
#line 114 "parse.y"
{
            yyval = new TreeNode(ID, lastID);
            DMESG(0, "id_list -> id_list, id_decl");
            ;
    break;}
case 15:
#line 118 "parse.y"
{
        DMESG(SUB, lastID);
        yyval = new TreeNode(SUB, lastID);
        yyval->addChild(new TreeNode(NUM, lastLIT));
        ;
    break;}
case 16:
#line 125 "parse.y"
{ 
        DMESG(FN_DECL, lastID);
        yyval = new TreeNode(FN_DEF, lastID);
        yyval -> addChild(yyvsp[-5]);
        yyval -> addChild(yyvsp[-2]);
        ;
    break;}
case 17:
#line 133 "parse.y"
{
        DMESG(FN_DEF, lastID);
        yyval = new TreeNode(FN_DEF, lastID);
        yyval -> addChild(yyvsp[-5]);
        yyval -> addChild(yyvsp[-2]);
        yyval -> addChild(yyvsp[0]);
        ;
    break;}
case 18:
#line 142 "parse.y"
{
    DMESG(0, "params->param_list");
    yyval = yyvsp[0];
    ;
    break;}
case 19:
#line 146 "parse.y"
{
    DMESG(VOID, "params -> VOID");
    yyval = new TreeNode(VOID);
    ;
    break;}
case 20:
#line 152 "parse.y"
{ 
        DMESG(VOID, "param_list -> param_list, param");
        pTreeNode tNode = yyvsp[-2];
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib(yyvsp[0]);
                yyval = yyvsp[-2];
                }
        else yyval = yyvsp[0];
        ;
    break;}
case 21:
#line 163 "parse.y"
{
    DMESG(VOID, "param_list -> param");
    yyval = yyvsp[0];
    ;
    break;}
case 22:
#line 169 "parse.y"
{
        DMESG(VOID, "param -> type_spec ID");
        yyval = yyvsp[-1];
        yyval->addChild(new TreeNode(ID, lastID));
        ;
    break;}
case 23:
#line 174 "parse.y"
{
        DMESG(VOID, "param -> type_spec ID[]");
        yyval = yyvsp[-3];
        yyval->addChild(new TreeNode(SUB, lastID));
        ;
    break;}
case 24:
#line 181 "parse.y"
{
        DMESG(0, "comp_stmt -> { local decl stmt_list }");
        if ( yyvsp[-3] == NULL ) yyval = yyvsp[-2];
        else {
             yyval = yyvsp[-3];
             yyvsp[-3]->addSib(yyvsp[-2]);
             }
        ;
    break;}
case 25:
#line 191 "parse.y"
{ DMESG(0, "local_decl -> /e/"); yyval = NULL ;
    break;}
case 26:
#line 192 "parse.y"
{
        pTreeNode wNode = yyvsp[-1];
        DMESG(0, "local_decl -> local_decl var_decl");
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib(yyvsp[0]);
           yyval = yyvsp[-1];
           }
        else yyval = yyvsp[0];
        ;
    break;}
case 27:
#line 204 "parse.y"
{ DMESG(0, "stmt_list -> /e/"); yyval = NULL; ;
    break;}
case 28:
#line 205 "parse.y"
{
        pTreeNode wNode = yyvsp[-1];
        DMESG(0, "stmt_list -> stmt_list statement");
        if (wNode != NULL) {
           while (wNode->getSib() !=NULL) wNode = wNode->getSib();
           wNode->addSib(yyvsp[0]);
           yyval = yyvsp[-1];
           }
        else yyval = yyvsp[0];
        ;
    break;}
case 29:
#line 217 "parse.y"
{ DMESG(0, "statement -> expr_stmt");   yyval = yyvsp[0] ;
    break;}
case 30:
#line 218 "parse.y"
{ DMESG(0, "statement -> comp_stmt");   yyval = yyvsp[0] ;
    break;}
case 31:
#line 219 "parse.y"
{ DMESG(0, "statement -> if_stmt");     yyval = yyvsp[0] ;
    break;}
case 32:
#line 220 "parse.y"
{ DMESG(0, "statement -> for_stmt");    yyval = yyvsp[0] ;
    break;}
case 33:
#line 221 "parse.y"
{ DMESG(0, "statement -> do_stmt");     yyval = yyvsp[0] ;
    break;}
case 34:
#line 222 "parse.y"
{ DMESG(0, "statement -> while_stmt");  yyval = yyvsp[0] ;
    break;}
case 35:
#line 223 "parse.y"
{ DMESG(0, "statement -> return_stmt"); yyval = yyvsp[0] ;
    break;}
case 36:
#line 226 "parse.y"
{ DMESG(0, "expr_stmt -> expr"); yyval = yyvsp[-1];   ;
    break;}
case 37:
#line 227 "parse.y"
{ DMESG(0, "expr_stmt -> /e/");  yyval = NULL; ;
    break;}
case 38:
#line 230 "parse.y"
{
        DMESG(IF, "if_stmt -> IF (expr) statement");
        yyval = new TreeNode(IF);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 39:
#line 236 "parse.y"
{
        DMESG(ELSE, "if_stmt -> IF (expr) statement");
        yyval = new TreeNode(ELSE);
        yyval->addChild(yyvsp[-4]);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 40:
#line 245 "parse.y"
{
        DMESG(FOR, "for_stmt -> FOR (expr;expr;expr) statement");
        yyval = new TreeNode(FOR);
        yyval->addChild(yyvsp[-6]);
        yyval->addChild(yyvsp[-4]);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 41:
#line 255 "parse.y"
{
    DMESG(0, "for_expr -> /e/");
    yyval = NULL; ;
    break;}
case 42:
#line 258 "parse.y"
{
    DMESG(0, "for_expr -> expr");
    yyval = yyvsp[0];
    ;
    break;}
case 43:
#line 264 "parse.y"
{
        DMESG(DO, "do_stmt -> DO statement WHILE (expr)");
        yyval = new TreeNode(DO);
        yyval->addChild(yyvsp[-5]);
        yyval->addChild(yyvsp[-2]);
        ;
    break;}
case 44:
#line 272 "parse.y"
{
        DMESG(WHILE, "while_stmt -> WHILE (expr) statement");
        yyval = new TreeNode(WHILE);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 45:
#line 281 "parse.y"
{
    DMESG(RETURN, "return_stmt -> RETURN");
    yyval = new TreeNode(RETURN);
    ;
    break;}
case 46:
#line 285 "parse.y"
{
    DMESG(RETURN, "return_stmt -> RETURN expr");
    yyval = new TreeNode(RETURN);
    yyval->addChild(yyvsp[0]);
    ;
    break;}
case 47:
#line 292 "parse.y"
{
    DMESG(ID, lastID);
    yyval = new TreeNode(ID, lastID);
    ;
    break;}
case 48:
#line 296 "parse.y"
{
    DMESG(SUB, lastID);
    yyval = new TreeNode(SUB, lastID);
    yyval->addChild(yyvsp[-1]);
    ;
    break;}
case 49:
#line 303 "parse.y"
{
    DMESG('=', "expr -> var = expr");
    yyval = new TreeNode('=');
    yyval->addChild(yyvsp[-2]);
    yyval->addChild(yyvsp[0]);
    ;
    break;}
case 50:
#line 309 "parse.y"
{
    yyval = yyvsp[0];
    DMESG(0, "expr -> exp");
    ;
    break;}
case 51:
#line 315 "parse.y"
{
        DMESG('+', "exp -> exp + exp");
        yyval = new TreeNode('+');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 52:
#line 321 "parse.y"
{
        DMESG('-', "exp -> exp - exp");
        yyval = new TreeNode('-');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 53:
#line 327 "parse.y"
{
        DMESG('*', "exp -> exp * exp");
        yyval = new TreeNode('*');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 54:
#line 333 "parse.y"
{
        DMESG('/', "exp -> exp / exp");
        yyval = new TreeNode('/');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 55:
#line 339 "parse.y"
{
        DMESG('%', "exp -> exp % exp");
        yyval = new TreeNode('%');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 56:
#line 345 "parse.y"
{
        DMESG(GTE, "exp -> exp GTE exp");
        yyval = new TreeNode(GTE);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 57:
#line 351 "parse.y"
{
        DMESG(LTE, "exp -> exp LTE exp");
        yyval = new TreeNode(LTE);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 58:
#line 357 "parse.y"
{
        DMESG('<', "exp -> exp < exp");
        yyval = new TreeNode('<');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 59:
#line 363 "parse.y"
{
        DMESG('>', "exp -> exp > exp");
        yyval = new TreeNode('>');
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 60:
#line 369 "parse.y"
{
        DMESG(CEQ, "exp -> exp CEQ exp");
        yyval = new TreeNode(CEQ);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 61:
#line 375 "parse.y"
{
        DMESG(NEQ, "exp -> exp NEQ exp");
        yyval = new TreeNode(NEQ);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 62:
#line 381 "parse.y"
{
        DMESG(L_OR, "exp -> exp L_OR exp");
        yyval = new TreeNode(L_OR);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 63:
#line 387 "parse.y"
{
        DMESG(L_AND, "exp -> exp L_AND exp");
        yyval = new TreeNode(L_AND);
        yyval->addChild(yyvsp[-2]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 64:
#line 393 "parse.y"
{
        DMESG(UMINUS, "exp -> -expval");
        yyval = new TreeNode(UMINUS);
        yyval->addChild(yyvsp[-1]);
        ;
    break;}
case 65:
#line 398 "parse.y"
{
        DMESG('!', "exp -> !expval");
        yyval = new TreeNode('!');
        yyval->addChild(yyvsp[-1]);
        ;
    break;}
case 66:
#line 403 "parse.y"
{
        DMESG(INCR, "exp -> ++var");
        yyval = new TreeNode(INCR);
        yyval->addChild(yyvsp[-1]);
        yyval->addChild(yyvsp[0]);
        ;
    break;}
case 67:
#line 409 "parse.y"
{
        DMESG(DECR, "exp -> --var");
        yyval = new TreeNode(DECR);
        yyval->addChild(yyvsp[-1]);
        ;
    break;}
case 68:
#line 414 "parse.y"
{ yyval = yyvsp[0]; DMESG(0, "exp -> expval"); ;
    break;}
case 69:
#line 416 "parse.y"
{ yyval = yyvsp[-1]; DMESG(0, "expval -> (expr)"); ;
    break;}
case 70:
#line 417 "parse.y"
{ yyval = yyvsp[0]; DMESG(0, "expval -> var"); ;
    break;}
case 71:
#line 418 "parse.y"
{ yyval = yyvsp[0]; DMESG(0, "expval -> call"); ;
    break;}
case 72:
#line 419 "parse.y"
{ yyval = new TreeNode(NUM, lastLIT);  DMESG(NUM, lastLIT);;
    break;}
case 73:
#line 420 "parse.y"
{ yyval = new TreeNode(FNUM, lastLIT); DMESG(FNUM, lastLIT); ;
    break;}
case 74:
#line 423 "parse.y"
{
       DMESG(CALL, lastID);
       yyval = new TreeNode(CALL, lastID);
       if (yyvsp[-1] != NULL) yyval->addChild(yyvsp[-1]);
       ;
    break;}
case 75:
#line 430 "parse.y"
{ yyval = NULL; DMESG(0, "args -> /e/"); ;
    break;}
case 76:
#line 431 "parse.y"
{ yyval = yyvsp[0];   DMESG(0, "args -> arg_list"); ;
    break;}
case 77:
#line 434 "parse.y"
{
        DMESG(0, "arg_list -> arg_list, expr");
        pTreeNode tNode = yyvsp[-2];
        if (tNode == NULL) {
                while (tNode->getSib() != NULL)
                        tNode = tNode->getSib();
                tNode->addSib(yyvsp[0]);
                yyval = yyvsp[-2];
                }
        else yyval = yyvsp[0];
        ;
    break;}
case 78:
#line 445 "parse.y"
{
    DMESG(0, "arg_list -> expr");
    yyval = yyvsp[0];
    ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 451 "parse.y"
