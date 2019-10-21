%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
extern int yylex();
void yyerror(char *msg);
int num = 0;
char name[105];
int lptr = 0,rptr = 0;
char left[100][100];
char right[100][100];
char tmp[100];
int ii,temp;
struct exp {
	char str[100];
	int code;
};
void getName (int num) {
	char tempstr[105];
	strcpy(name, "t");
	sprintf(tempstr,"%d", num);
	strcat(name, tempstr);
}
%}

%union {
  float f;
  int i;
  char c[100];
  struct exp ee;
}

%token <f> real
%token <i> integer
%token <c> true false id 
%type <ee> assignment vars exprs expr term factor sum

%%

/*
assignment → vars := exprs;
vars → id |vars , id
exprs → expr | exprs , expr
expr → sum
sum → term | - term | sum + term | sum - term
term → factor | term * factor | term / factor
factor → true | false | integer | real | id | ( expr )
*/

/*
Input: np, x := trial*trial, 2+3;
*/

assignment : vars ':' '=' exprs ';' 	{	
                        if (DEBUG) printf("Assign %d %d %d\n",num, lptr, rptr);
						for(ii = 0;ii<lptr;ii++)printf("%s = %s\n",left[ii],right[ii]); 
					}
					;
vars : 	  id				{	
                        if (DEBUG) printf("Vars->ID %s  %d %d\n",$1, lptr,rptr);
						strcpy(left[lptr],$1);
						lptr++;			
					}
	| vars ',' id			{	
                        if (DEBUG) printf("VARS -> VARS|ID %d %d\n",lptr,rptr);
						getName(num);
						strcpy(left[lptr],$3);
						lptr++;
					}			
					;
exprs : expr				{	
                        if (DEBUG) printf("EXPRS -> EXPR %d %d\n",lptr,rptr);
						getName($1.code);
						strcpy(right[rptr],name);
						rptr++;
						$$.code = $1.code;
					}
	|exprs ',' expr 		{	
                        if (DEBUG) printf("EXPRS -> EXPRS , EXPR %d %d\n",lptr,rptr);
						getName($3.code);
						strcpy(right[rptr],name);
						rptr++;
						$$.code = $1.code;
					}
					;
expr : sum				{	
                        if (DEBUG) printf("EXPR->SUM %d\n",num);
						$$.code = $1.code;
					}				
	 				;
	 
sum : term				{	
                        if (DEBUG) printf("SUM -> TERM %d\n",num);	
						$$.code = $1.code;				
					}	
	| '-' term  			{	
                        if (DEBUG) printf("SUM -> -TERM %d\n",num);			
						num++;	
						getName(num);
						printf("%s",name);
						getName($2.code);
						printf(" = -%s\n",name);
						$$.code = num;				
					}
	| sum '+' term  		{	
                        if (DEBUG) printf("SUM -> SUM + TERM %d\n",num);
						num++;	
						getName(num);
						printf("%s",name);
						getName($1.code);
						printf(" = %s +",name); 
						getName($3.code);
						printf(" %s\n",name);
						$$.code = num; 
					}
	| sum '-' term			{	
                        if (DEBUG) printf("SUM -> SUM - TERM %d\n",num);
						num++;	
						getName(num);
						printf("%s",name);
						getName($1.code);
						printf(" = %s -",name); 
						getName($3.code);
						printf(" %s\n",name);
						$$.code = num; 
					}
					;
term :   factor				{	
                        if (DEBUG) printf("TERM -> FACTOR %d\n",num);
						num++;
						getName(num);
						printf("%s",name);
						getName($1.code);
						printf(" = %s\n",$1.str);
						$$.code = num;	
					
					}
	   | term '*' factor		{	
                        if (DEBUG) printf("TERM -> TERM*FACTOR %d\n",num);
						num++;	
						getName(num);
						printf("%s",name);
						getName($1.code);
						printf(" = %s *",name); 
						getName($3.code);
						printf(" %s\n",$3.str);
						$$.code = num; 
					}
	   | term '/' factor		{	
                        if (DEBUG) printf("TERM -> TERM/FACTOR %d\n",num);
						num++;	
						getName(num);
						printf("%s",name);
						getName($1.code);
						printf(" = %s /",name);
						printf(" %s\n",$3.str);
						$$.code = num; 
					}
					;
factor : 	true 			{
						strcpy($$.str,$1);
					}
		| false			{				
						strcpy($$.str,$1);							
					}
		| integer		{				
						sprintf($$.str,"%d",$1);
					}
		| real			{				
						sprintf($$.str,"%f",$1);								
					}
		| id			{
						strcpy($$.str,$1);								
					}
		| '(' expr ')'		{
						getName(num);
						strcpy($$.str,name);				
					}
					;
%%

void yyerror(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main() {
    printf("Welcome to Compiler X.\n");
    while(1) {
        printf("Enter your code below:\n");
        yyparse();    
    }    
    return 0;
}
