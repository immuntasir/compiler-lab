lex flex.l
yacc -d yacc.y
gcc lex.yy.c y.tab.c -o test -ly -ll
./test
