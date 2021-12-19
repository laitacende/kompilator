%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <cstdlib>
    #include <memory>

    #include "./inc/Symbol.hpp"
    #include "./inc/Cond.hpp"
    #include "parser.tab.hpp"

    int yylex();
%}

%x COMMENT

%%

\\\n            ; /* skip empty lines */
[ \t]+          ;
"("             { BEGIN(COMMENT); }

<COMMENT>{
    ")"    { BEGIN(INITIAL); }
    .|\n
}

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
"IF"        { return IF; }
"THEN"      { return THEN; }
"ELSE"      { return ELSE; }
"ENDIF"     { return ENDIF; }
"EQ"        { return EQ; }
"NEQ"       { return NEQ; }
"LE"        { return LE; }
"GE"        { return GE; }
"LEQ"       { return LEQ; }
"GEQ"       { return GEQ; }
"WHILE"     { return WHILE; }
"DO"        { return DO; }
"ENDWHILE"  { return ENDWHILE; }
"REPEAT"    { return REPEAT; }
"UNTIL"     { return UNTIL; }
[0-9]+      { yylval.num = atoi(yytext); return num; }
";"         { return yytext[0]; }
","         { return yytext[0]; }
"-"         { return yytext[0]; }
"["         { return yytext[0]; }
"]"         { return yytext[0]; }
":"         { return yytext[0]; }
[_a-z]+     {  yylval.pidentifier = new std::string(yytext); return pidentifier; }
\n          ;
.           { return ERROR; }

%%