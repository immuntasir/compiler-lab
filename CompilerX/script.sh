#!/bin/bash
yacc -d yacc.y
lex -l flex.l
gcc lex.yy.c y.tab.c -o out
./out
