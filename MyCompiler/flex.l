%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct exp
{
	char part[100];
	int code;
};
#include "y.tab.h" 
%}

%option noyywrap

%%
[A-Za-z][A-Za-z0-9]*	{strcpy(yylval.c,yytext);
			return id;}
[0-9]+[.][0-9]+   {yylval.f = atof(yytext);return real;}
[0-9]+    {yylval.i = atoi(yytext); return integer;}
[-+()*/,:=;]                         {return yytext[0];}
[t][r][u][e] {strcpy(yylval.c,yytext);return true;}
[f][a][l][s][e] {strcpy(yylval.c,yytext);return false;}

[ \t\f\v]                      { ; }
[\n] { return EOF;}

%%
