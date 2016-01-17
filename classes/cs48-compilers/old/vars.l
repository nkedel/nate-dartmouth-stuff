%{
#include "vars.tab.h"
%}

%%

[ \t\n]+        ;
","             return ',';
";"             return ';';
int             return INT;
float           return FLOAT;
[A-Za-z]+       return ID;
.               ;
