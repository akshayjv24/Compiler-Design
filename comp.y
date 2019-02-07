%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "comp.h"
	#include "comp.c"
    	int yylex(void);
    	int yyerror(char const *);


%}

%union{
	struct tnode *no;
	//char str;
}
%type <no> expr NUM program Slist Stmt InputStmt OutputStmt AsgStmt ID Ifstmt Whilestmt
%type <no> PCONTINUE PBRKP PBREAK sp_comm
%token NUM PLUS MINUS MUL DIV NEWLINE  PREL 
%token PBEGIN PREAD PWRITE  PEND ID
%token PIF PTHEN PELSE PENDIF PWHILE PDO PENDWHILE
%token PGT PGE PLT PLE PEQ PNE 
%left PGT PGE PLT PLE PEQ PNE
%token PCONTINUE PBRKP PBREAK 
%left PLUS MINUS
%left MUL DIV

%%
program : PBEGIN Slist PEND';'      {$$=$2;codeGen($2);
                                      
					//evaluate($2);
					//inorder($2);
					}
	| PBEGIN PEND';'		{$$=NULL;}
	;
//conditional constructs
Ifstmt :PIF '('expr')' PTHEN Slist PELSE Slist PENDIF ';'	{$$=createTree(0,0,NULL,IF,$3,$6,$8);}
	| PIF '('expr')' PTHEN Slist PENDIF';'		{$$=createTree(0,0,NULL,IF,$3,$6,NULL);}
	;
Whilestmt : PWHILE '('expr')' PDO Slist PENDWHILE';'	{$$=createTree(0,0,NULL,WHILE,$3,NULL,$6);}
	;

Slist : Slist Stmt 		{$$ = createTree(0,0,NULL,CONN,$1,NULL,$2);}
	| Stmt			{$$=$1;}
	;
Stmt : InputStmt 		{$$=$1;}
	| OutputStmt 		{$$=$1;}
	| AsgStmt		{$$=$1;}
	| Ifstmt		{$$=$1;}
	| Whilestmt		{$$=$1;}
	| sp_comm 		{$$=$1;}
	;

InputStmt : PREAD'('ID')'';'	{$$ = createTree(0,0,NULL,READ,$3,NULL,NULL);}
	;

OutputStmt : PWRITE'('expr')'';'	{$$ = createTree(0,0,NULL,WRITE,$3,NULL,NULL);}
	;
	
AsgStmt : ID'='expr';' 		{$$ = createTree(0,0,'=',ASSGN,$1,NULL,$3);}
	;	

sp_comm : PBRKP';' 		{$$=createTree(0,0,NULL,BRKP,NULL,NULL,NULL);}
	   | PBREAK ';'		{$$=createTree(0,0,NULL,BREAK,NULL,NULL,NULL);}
	   | PCONTINUE ';'	{$$=createTree(0,0,NULL,CONTINUE,NULL,NULL,NULL);}
	   ;
	
expr : expr PLUS expr		{$$ = createTree(0,INTEGER,'+',EX,$1,NULL,$3);}
	 | expr MINUS expr  	{$$ = createTree(0,INTEGER,'-',EX,$1,NULL,$3);}
	 | expr MUL expr	{$$ = createTree(0,INTEGER,'*',EX,$1,NULL,$3);}
	 | expr DIV expr	{$$ = createTree(0,INTEGER,'/',EX,$1,NULL,$3);}
	 | '(' expr ')'		{$$ = $2;}
	 | expr PGT expr		{$$ = createTree(0,BOOLE,NULL,GT,$1,NULL,$3);}		
	 | expr PGE expr		{$$ = createTree(0,BOOLE,NULL,GE,$1,NULL,$3);}
	 | expr PLT expr		{$$ = createTree(0,BOOLE,NULL,LT,$1,NULL,$3);}
	 | expr PLE expr		{$$ = createTree(0,BOOLE,NULL,LE,$1,NULL,$3);}
	 | expr PEQ expr		{$$ = createTree(0,BOOLE,NULL,EQ,$1,NULL,$3);}
	 | expr PNE expr		{$$ = createTree(0,BOOLE,NULL,NE,$1,NULL,$3);}
	 | NUM			{$$ = $1;}
	 | ID			{$$ = $1;}
	 ;

%%

yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(int argc,char* argv[]) {
if(argc > 1)
	{
		FILE *fp = fopen(argv[1], "r");
		if(fp)
			yyin = fp;
}
 	xsm_fl=fopen("comp.xsm","w");
	fprintf(xsm_fl, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n ",0,2056,0,0,0,0,0,0); 
	fprintf(xsm_fl, "BRKP\n");
	fprintf(xsm_fl, "MOV SP, 4121\n");
	yyparse();
	fprintf(xsm_fl, "INT10\n");
	return 0;
}
