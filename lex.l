%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <cstdlib>
    #include <memory>

    #include "./inc/Symbol.hpp"
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
"ASSIGN"    { return ASSIGN; }
"PLUS"      { return PLUS; }
"MINUS"     { return MINUS; }
"TIMES"     { return TIMES; }
"DIV"       { return DIV; }
"MOD"       { return MOD; }
"READ"      { return READ; }
[0-9]+      { yylval.num = atoi(yytext); return num; }
";"         { return yytext[0]; }
","         { return yytext[0]; }
"-"         { return yytext[0]; }
[_a-z]      {  yylval.pidentifier = new std::string(yytext); return pidentifier; }
\n          ;
.           { std::cout << yytext; }

%%