%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "comp.h"
	#include "y.tab.h"
	
	
	int number;
	
	

%}

%%

[0-9]+	{number = atoi(yytext);yylval.no = createTree(number,INTEGER,NULL,CONSTANT,NULL,NULL,NULL); return NUM;}
"begin" {return PBEGIN;}
"end" 	{return PEND;}
"read"	{return PREAD;}
"write"	{return	PWRITE;}
"if"	{return PIF;}
"then"	{return PTHEN;}
"else"	{return PELSE;}
"endif"	{return PENDIF;}
"breakpoint" {return PBRKP; }
"break" {return PBREAK;}
"continue" {return PCONTINUE;}
"while"	{return PWHILE;}
"do"	{return PDO;}
"endwhile" {return PENDWHILE;}
[a-z]   {yylval.no=createTree(0,INTEGER,*yytext,VAR,NULL,NULL,NULL); return ID; }
";"	{return *yytext;}
"="	{return *yytext;}
"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MUL;}
"/"	{return DIV;}
"<"	{return PLT;} // boolean constucts
"<="	{return PLE;}
">"	{return PGT;}
">="	{return PGE;}
"=="	{return PEQ;}
"!=" 	{return PNE;}
[ \t]	{}
[()]	{return *yytext;}
[\n]	{}
.	{yyerror("unknown character\n");exit(1);}

%%

int yywrap(void) {
	return 1;
}
