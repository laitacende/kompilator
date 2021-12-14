%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <cstdlib>

    #include "parser.tab.hpp"

    int yylex();
%}

%%

\\\n            ; /* skip empty lines */
[ \t]+          ;


"BEGIN"     { return BEGIN_; }
"END"       { return END; }
"VAR"       { return VAR; }
"WRITE"     { return WRITE; }

[0-9]+      { yylval = atoi(yytext); return num; }
.

%%