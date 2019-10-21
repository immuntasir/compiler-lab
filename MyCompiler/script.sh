lex flex.l
gcc lex.yy.c y.tab.c -o test -ly -ll
./test > output.out
