Run the following commands: 
lex flex.l
gcc lex.yy.c y.tab.c -o test -ly -ll
./test > output.out

Or, alternatively, you can run the "script.sh" script.
