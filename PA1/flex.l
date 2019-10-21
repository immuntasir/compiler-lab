%{ 
#include "y.tab.h"
#include <math.h>
extern double vbltable[26];
%}

%%


(0|4|8)[ ] |
((00|04|08|12|16|20|24|28|32|36|40|44|48|52|56|60|64|68|72|76|80|84|88|92|96))[ ] |
([0-9](00|04|08|12|16|20|24|28|32|36|40|44|48|52|56|60|64|68|72|76|80|84|88|92|96))[ ] |
([0-9][0-9](00|04|08|12|16|20|24|28|32|36|40|44|48|52|56|60|64|68|72|76|80|84|88|92|96))[ ]  {
       printf("DIV_4* %s\n", yytext);
       }

([+,-])([0-9]+)[ ]  {
       printf("Signed %s\n", yytext);
       }

([0-9]+)([.])([0-9]+)[ ]  {
       printf("Decimal %s\n", yytext);
       }

((([+,-])([0-9]+)|([0-9]+)([.])([0-9]+))[eE]([+,-])([0-9]+)[ ])   {
       printf("Scientific %s\n", yytext);
       }

0x[0-9][ ] |
0x[0-9][0-9][ ] |
0x[0-9][0-9][0-9][ ] |
0x[0-9][0-9][0-9][ ] {
       printf("Hex %s\n", yytext);
       }

[0-9][0-9][0-9][0-9][0-9]+[ ] {
       printf("Overflow %s\n", yytext);
       }

[a-zA-Z][a-zA-Z0-9]*[ ] {
       printf("Id %s\n", yytext);
       }

= {
       printf("AssignOp %s\n", yytext);
       }

[+][ ] {
       printf("AddOp %s\n", yytext);
       }

[*][ ] {
       printf("MultOp %s\n", yytext);
       }

[>][ ] |
[<][ ] |
(>=)[ ] |
(<=)[ ] {
       printf("RelOp %s\n", yytext);
       }

[a-zA-Z0-9]+ | 
[ ]+ |
[+-]+ {  }
.|\n { ECHO; /* normal default anyway */ }
%%

int main ( )
{
    if(!(yyin = fopen("input.in", "r"))) {
        printf("File opened successfully");        
        return (1);
    }
yylex() ;
}
