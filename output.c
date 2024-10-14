1a.l

%{
#include<stdio.h>
int words = 0, spaces = 0, chars = 0, lines = 0;
int yywrap();
%}

%%

\n {lines++;}
\t {spaces += 4;}
[ ] {spaces++;}
[^\n\t# ]+ {words++;chars += yyleng;}
# {return 1;}

%%


int yywrap(){
return 1;
}
int main(){
	printf("Enter the statements\n");
	yylex();
	printf("Lines: %d\nWords: %d\nCharacters: %d\nSpaces: %d\n",lines,words,chars,spaces);
	return 0;
	
}

1b.l
%{
#include "y.tab.h"
%}

%%

"a" {return 'a';}
"b" {return 'b';}
"c" {return 'c';}
. {return yytext[0];}
\n {return 0;}

%%
1b.y
%{
#include<stdio.h>
#include<stdlib.h>
int yyerror();
int yylex();
%}

%%
S: A B
;
A: 'a'A'b'
|
;
B: 'b'B'c'
|
;
%%

int main(){
	printf("Enter the string\n");
	yyparse();
	printf("String is valid\n");
}

int yyerror(){
	printf("String is Invalid\n");
	exit(0);
}

2a.l
%{
#include<stdio.h>
int posint = 0, negint = 0, posfrac = 0, negfrac = 0,invalidcount = 0;
%}

num [0-9]+
posint \+?{num}
negint -{num}
posdec \+?{num}\.{num}?|\+?\.{num}
negdec -{num}\.{num}?|-\.{num}
posnum {posint}|{posdec}
negnum {negint}|{negdec}
numbers {posnum}|{negnum}
inv {numbers}\/[+-]?0
cont [+-]{numbers}

%%

{inv} invalidcount++;
{numbers}{cont}+ invalidcount++;
{posint} posint++;
{negint} negint++;
{posdec} posfrac++;
{negdec} negfrac++;
{posnum}\/{posnum}|{negnum}\/{negnum} posfrac++;
{posnum}\/{negnum}|{negnum}\/{posnum} negfrac++;
\n return 0;


%%

int yywrap(){
	return 1;
}
int main(){
	printf("Enter the value");
	yylex();
	printf("\nPositive integers: %d\nNegative Integers: %d\nNegative Fractions: %d\nPostive Fractions: %d\nInvalidCount: %d",posint,negint,negfrac,posfrac,invalidcount);
	return 0;
	
}

2b.l
%{
#include "y.tab.h"
extern YYSTYPE yylval;
%}

%%

[0-9]+ {yylval = atoi(yytext); return NUM;}
[-+*/] {return yytext[0];}
.      {return yytext[0];}
\n     {return 0;}

%%
2b.y
%{
#include<stdio.h>
#include<stdlib.h>
int yyerror();
int yylex();
%}

%token NUM
%left '+''-'
%left '/''*'

%%
S: E {printf("Result is %d\n",$$);}
;
E: E'+'E {$$ = $1 + $3;}
|
E'-'E {$$ = $1 - $3;}
|
E'*'E {$$ = $1 * $3;}
|
E'/'E {if($3 == 0) yyerror(); else $$ = $1/$3;}
|
'('E')' {$$=$2;}
|
NUM {$$ = $1;}
|
'-'NUM {$$ = -$1;}
;

%%
int main(){
  printf("Enter the operation\n");
  yyparse();
  printf("\nValid\n");
  return 0;
}

int yyerror(){
	printf("Invalid");
	exit(0);
}

3a.l
%{

#include<stdio.h>
#include<stdlib.h>
int single = 0, multi = 0, mlines = 0;
int yylex();

%}

%%

\/\*([^*]|\*+[^*/])*\*+\/ {multi++; for(int i = 0; i < yyleng; i++) if(yytext[i] == '\n') mlines++;}
\/\/.* {single++;}
. {fprintf(yyout,"%s",yytext);}

%%

int main(){
	yyin = fopen("./input3.c","r");
	yyout = fopen("./output2.c","w");
	yylex();
	printf("\nSingle Line Comments: %d\nMultiline Comments: %d\nLines in Multline comments: %d\n",single,multi,mlines+multi);
	return 0;
}

int yywrap(){
	return 1;
	exit(0);
}
3b.l
%{
#include "y.tab.h"
int yywrap();
%}

%%
"for" {return FOR;}
[ \t\n] {}
[0-9]+ {return NUM;}
[_a-zA-Z][_a-zA-Z]* {return ID;}	
. {return yytext[0];}
%%
int yywrap(){
	return 1;
}

3b.y
%{
#include<stdio.h>
#include<stdlib.h>
int count = 0,level = 0;
int yyerror();
int yylex();
%}

%token NUM ID FOR

%%

S:I 
;
I: FOR '('E';'E';'E')' {count++; if(level < count) level = count;}
C {count--;}
;
E: ID Z ID
| ID Z NUM
| ID U
| ID	
;

Z: '='|'<''='|'>''='|'=''='|'!''='|'+'|'-'|'*'|'/'|'<'|'>'
;

U: '+''+'|'-''-'
;

C:'{'B'}'
|E';'
|I
;

B: B B
| E';'
| I
| 
;

%%

int yyerror(){
	printf("Invalid");
	exit(0);
}
int main(){
	printf("Write the code: \n");
	yyparse();
	printf("Count: %d\n",level);
	return 0;
}

4a.l
%{
#include<stdio.h>
#include<stdlib.h>
int keywords=0, identifiers=0, operators=0, rel_op=0, log_op=0, arith_op=0;
int yywrap();
%}

letters [a-zA_z_]
digits [0-9]

%%

"if"|"else"|"for"|"while"|"do"|"void"|"struct"|"int"|"return" {keywords++;}
"&&"|"||"|"!" {operators++;log_op++;}
"<"|"<="|">="|"=="|"!=" {operators++;rel_op++;}
"+"|"-"|"*"|"/"|"%" {operators++,arith_op++;}
.|[\n\t] ;
{letters}({letters}|{digits})* {identifiers++;}

%%

int main(){
	yyin = fopen("./input.c","r");
	if(!yyin){
		perror("failed to open file");
		exit(0);
	}
	yylex();
	printf("Keywords: %d\n", keywords);
  	printf("Operators: %d\n", operators);
  	printf("\tarithmetic_operators: %d\n", arith_op);
  	//printf("\tbitwise_operators: %d\n", bitwise_operators);
  	printf("\tlogical_operators: %d\n", log_op);
  	printf("\trelational_operators: %d\n", rel_op);
  	printf("Identifiers: %d\n", identifiers);

  fclose(yyin);
}

int yywrap(){
	return 1;
}

4b.l
%{
#include<string.h>
#include "y.tab.h"
%}

%%

[_a-zA-Z][_a-zA-Z0-9]* {yylval.exp = strdup(yytext);return ID;}
[0-9]+ {yylval.exp = strdup(yytext);return NUM;}
[-+*/] {return yytext[0];}
[()=] {return yytext[0];}
[\n]+ return '\n';
[\t]+ ;
. ;

%%
4b.y
%{

#include<stdio.h>
#include<stdlib.h>
int cnt = 0, level = 0;
int yyerror();
int yylex();

%}

%token NUM ID FOR

%%

S:I
;
I:FOR'('E';'E';'E')'{cnt++;if(level < cnt)level=cnt;}
C {cnt--;}
;
E:ID Z ID
|ID Z NUM
|ID N
|ID
;
Z:'='|'<''='|'>''='|'=''='|'!''='|'+'|'-'|'*'|'/'|'<'|'>'
;
N:'+''+'|'-''-';
C:'{'B'}'
|E ';'
|I;

B:B B
|E';'
|I
|
;

%%

int yyerror(){
	printf("Invalid\n");
	exit(0);
}

int main(){
 printf("Enter the statements\n");
 yyparse();
 printf("No of IF are: %d", level);
 return 0;
}

5.l
%{
#include "y.tab.h"
%}

%%

"int"|"float"|"double"|"char" {return TYPE;}
[a-zA-Z_][a-zA-Z0-9_]* {return ID;}
[0-9]+ {return NUM;}
[ \t\n]+     ;
.    {return *yytext;}

%%

int yywrap(){
	return 1;
}
5.y
%{
#include<stdio.h>
#include<stdlib.h>
#include "lex.yy.c"
int count = 0;
int yyerror(char *);
%}

%token ID NUM TYPE

%%
S: DECL {printf("Total number of variables declared are: %d\n",count);exit(0);}
;
DECL: TYPE VARLIST ';'
| TYPE VARLIST';' DECL;
VARLIST: VAR
| VAR','VARLIST
;
VAR: ID C {count++;}
| ID'='NUM {count++;}
| ID'='ID {count++;}
;
C:'['NUM']'C
|
;
%%

int yyerror(char *s){
	fprintf(stderr, "Error: %s\n", s);
  	exit(1);
}

int main(){
	printf("Enter the declaration Statements\n");
	yyparse();
	return 0;
}

6.l
%{
#include<string.h>
#include "y.tab.h"
%}

%%

[_a-zA-Z][_a-zA-Z0-9]* {yylval.exp = strdup(yytext);return ID;}
[0-9]+ {yylval.exp = strdup(yytext);return NUM;}
[-+*/] {return yytext[0];}
[()=] {return yytext[0];}
[\n]+ return '\n';
[\t]+ ;
. ;

%%
6.y
%{
#include<stdio.h>
#include<stdlib.h>
typedef char* string;
int yylex();
int yyerror();
struct{
	string op1,op2,res;
	char op;
}code[100];

int ind = -1;
string addToTable(string op1, string op2, char op);
void quadruples();
void tac();

%}

%union{
  char* exp;
}
%token<exp> NUM ID
%type<exp> EXP
%right '='
%left '+''-'
%left '*''/'

%%

STMTS: STMTS STMT
|
;
STMT: EXP '\n'
;
EXP: ID '=' EXP  {$$ = addToTable($1,$3,'=');}
   | EXP '+' EXP {$$ = addToTable($1,$3,'+');}
   | EXP '-' EXP {$$ = addToTable($1,$3,'-');}
   | EXP '*' EXP {$$ = addToTable($1,$3,'*');}
   | EXP '/' EXP {$$ = addToTable($1,$3,'/');}
   | '('EXP')'   {$$ = $2;}
   | NUM         {$$ = $1;}
   | ID          {$$ = $1;}
;

%%

int main(){
	//yyin = fopen("./input.txt","r");
	printf("Enter the exprn\n");
	yyparse();
	printf("\nThree Address Code\n");
	tac();
	printf("Quadruples\n");
	quadruples();
	//fclose(yyin);
	return 0;
}

string addToTable(string op1, string op2, char op){
	if(op == '='){
		ind++;
		code[ind].res = op1;
		code[ind].op1 = op2;
		code[ind].op2 = "";
		code[ind].op = '=';
		return op1; 
	}
	ind++;
	string res = malloc(3);
	sprintf(res,"@%c",ind+'A');
	code[ind].op1 = op1;
	code[ind].op2 = op2;
	code[ind].op = op;
	code[ind].res = res;
	return res;
}

void quadruples(){
	for(int i = 0; i <= ind; i++){
		printf("%d:\t%c\t%s\t%s\t%s\n",i,code[i].op,code[i].op1,code[i].op2,code[i].res);
	}
}
void tac(){
	for(int i = 0; i <= ind; i++){
		if(code[i].op  == '='){
			printf("%s = %s\n",code[i].res,code[i].op1);
		}
		else{
			printf("%s = %s %c %s\n",code[i].res,code[i].op1, code[i].op, code[i].op2);
		}
	}
}
int yyerror(){
	printf("Invalid\n");
	exit(0);
}


7.l
%{
#include "y.tab.h"
int yywrap();
%}

%%

[ \n\t]+ {}
"int"|"float"|"char"|"double"|"void" {return TYPE;}	
"return" {return RET;}
[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
[0-9]+ {return NUM;}
. {return yytext[0];}

%%

int yywrap(){
	return 1;
}


7.y

%{
#include<stdio.h>
#include<stdlib.h>
void yyerror();
int yylex();
%}
%token ID NUM TYPE RET

%%
S: FUNC {printf("Valid Func!\n");}
;
FUNC: TYPE ID '('PARAM')''{'BODY'}'
;

PARAM: PARAM ',' PARAM
|TYPE ID
|
;

BODY: BODY BODY
| PARAM';'
| E';'
| RET E';'
| 
;

E: ID '=' E
| E'+'E
| E'-'E
| E'*'E
| E'/'E
| ID
| NUM
;


%%

int main(){
	printf("Enter the string\n");
	yyparse();
	return 0;
}
void yyerror(){
	printf("Invalid\n");
	exit(0);
}

8.l
%{
    #include<string.h>
    #include"y.tab.h"
%}
%%
[_a-zA-Z][_a-zA-Z]* { yylval.exp = strdup( yytext); return IDEN;}
[0-9]+ { yylval.exp=strdup(yytext); return NUM;}
[-+*/] { return yytext[0];}
[()=] { return yytext[0];}
[\n]+ {return '\n';}
[\t ]+ {}
%%

8.y
%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror();

//extern FILE *yyin;

typedef char *string;

struct {
	string res, op1, op2;
	char op;
} code[100];
int idx = -1;

string addToTable(string, string, char);
void targetCode();
%}

%union {
	char *exp;
}

%token <exp> IDEN NUM
%type <exp> EXP
%right '='
%left '+' '-'
%left '*' '/'

%%

STMTS	: STMTS STMT
	|
	;

STMT	: EXP '\n'
	;

EXP	: IDEN '=' EXP { $$ = addToTable($1, $3, '='); }
    | EXP '+' EXP { $$ = addToTable($1, $3, '+'); }
	| EXP '-' EXP { $$ = addToTable($1, $3, '-'); }
	| EXP '*' EXP { $$ = addToTable($1, $3, '*'); }
	| EXP '/' EXP { $$ = addToTable($1, $3, '/'); }
	| '(' EXP ')' { $$ = $2; }
	| IDEN { $$ = $1; }
	| NUM { $$ = $1; }
	;

%%

int yyerror(const char *s) {
	printf("Error %s", s);
	exit(0);
}

int main() {
	//yyin = fopen("8.txt", "r");
	yyparse();

	printf("\nTarget code:\n");
	targetCode();
}


string addToTable(string op1, string op2, char op) {
	if(op == '=') {
		code[idx].res = op1;
		return op1;
	}

	idx++;
	string res = malloc(3);
	sprintf(res, "@%c", idx + 'A');

	code[idx].op1 = op1;
	code[idx].op2 = op2;
	code[idx].op = op;
	code[idx].res = res;
	return res;
}

void targetCode() {
	for(int i = 0; i <= idx; i++) {
		string instr;
		switch(code[i].op) {
		case '+': instr = "ADD"; break;
		case '-': instr = "SUB"; break;
		case '*': instr = "MUL"; break;
		case '/': instr = "DIV"; break;
		}

		printf("LOAD\t R1, %s\n", code[i].op1);
		printf("LOAD\t R2, %s\n", code[i].op2);
		printf("%s\t R3, R1, R2\n", instr);
		printf("STORE\t %s, R3\n", code[i].res);
	}
}

8(2).y

%{
    #include<stdio.h>
    #include<stdlib.h>
    int yylex();
    int yyerror();
    typedef char* string;
    struct{
        string op1, op2, res;
        char op;
    }code[100];
    int idx=-1;
    string addToTable(string , string , char);
    void targetCode();
%}
%union{
    char* exp;
}
%token <exp> IDEN NUM
%type <exp> EXP 
%right '='
%left '+' '-' 
%left '*' '/'
%%
STMTS: STMTS STMT
|
;
STMT: EXP '\n'
;
EXP: IDEN'='EXP { $$=addToTable($1,$3,'=');}
| EXP'+'EXP { $$=addToTable($1,$3,'+');}
| EXP'-'EXP { $$=addToTable($1,$3,'-');}
| EXP'*'EXP { $$=addToTable($1,$3,'*');}
| EXP'/'EXP { $$=addToTable($1,$3,'/');}
| '('EXP')' { $$=$2;}
| IDEN {$$=$1;}
| NUM { $$=$1;}
;
%%
int yyerror(){
    printf("Invalid");
    exit(0);
}
int main(){
    printf("Enter the statements:\n");
    yyparse();
    targetCode();
}
void targetCode(){
    printf("Target Cdde:\n");
    for(int i=0;i<=idx;i++){
        string instr ;
        switch(code[i].op){
            case '+': instr = "ADD"; break;
            case '-': instr = "SUB"; break;
            case '*': instr = "MUL"; break;
            case '/': instr = "DIV"; break;
        }
        printf("LOAD\t R1, %s\n", code[i].op1);
        printf("LOAD\t R2, %s\n", code[i].op2);
        printf("%s\t R3, R1, R2\n",instr);
        printf("STORE\t %s, R3\n", code[i].res);

    }
    
}


string addToTable(string op1, string op2, char op){
    if(op=='='){
        code[idx].res = op1;
        return op1;
    }
    idx++;
    string res = malloc(3);
    sprintf(res, "@%c", idx+'A');
    code[idx].op1=op1;
    code[idx].op2=op2;
    code[idx].op=op;
    code[idx].res = res;


}
