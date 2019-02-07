//#include "arith.h"
//extern int yyparse(void);
int var_array[26];
int g=0;
int lct=0;
struct label_node* head;
void typecheck(struct tnode *t)
{  
  switch(t->nodetype)
  {
   case EX:
    		if((t->left->type!=INTEGER) || (t->right->type!=INTEGER))
    			{
    			  yyerror("Type mismatch! Expected integer");
    			  exit(1);
    			  
    			}
    			break;
   case EQ:
   case NE:
   case GE:
   case GT:
   case LE:
   case LT:
    			
  		if((t->left->type!=INTEGER) || (t->right->type!=INTEGER))
    			{
    			  yyerror("Type mismatch! Expected integer for boolean operaion");
    			  exit(1);
    			  
    			}
    			break;
  case IF:
  case WHILE:
  		if(t->left->type!=BOOLE)
  		{
  		   yyerror("Type mismatch...Expected boolean type");
    			  exit(1);
  		} 
  		break;
		
  case WRITE:
		if((t->left->type != INTEGER))
		{
		    yyerror("Invalid argument type to Write() function");
		        exit(1);
		}
			break; 
  case ASSGN:
  		if((t->left->type != INTEGER))
		{
		    yyerror("LHS of '=' operator expect integer type");
		    exit(1);  
		} 
 		if((t->right->type != INTEGER))
		{
		    yyerror("RHS of '=' operator expect integer type");
		    exit(1);  
		} 			
  }

}


reg_index codeGen( struct tnode *t) {
  int x,y,z;
  int l,m,n;
  int lb1=0;
  int lb2=0;
  int lb3=0;
   
 switch(t->nodetype){
 
          
	case READ:                //considering only one small letter var and assigning in top of stack..........
		x=getreg();
		int stackpos;
		stackpos=4096+(*(t->left->varname))-'a';
		backup_stack(x,0);
		lb_call("Read",-1,stackpos,x);
		restore_stack(x,x-1);
		return x;
		break;
	case WRITE:
		x=getreg();
		l=codeGen(t->left);
		backup_stack(x,0);
		lb_call("Write",-2,l,x);
		restore_stack(x,x-1);
		freereg();
		return x;
		break;
	case CONSTANT:
		x=getreg();
                fprintf(xsm_fl,"MOV R%d, %d\n",x,t->val);
		return x;
		break;	
	case CONN:
		if(t->left)
		    {
		    l= codeGen(t->left);
		    freereg();
		    
		    }	  	
		if(t->right)
		    {	
		    l= codeGen(t->right);
		    freereg();		    
		    }
		    return l;
		    break;
	case VAR:
		    x=4096+*(t->varname)-'a';
		    y=getreg();
		    fprintf(xsm_fl,"MOV R%d, [%d]\n",y,x);
		    return y;
		    break;
	case ASSGN:
		    m=codeGen(t->right);
	            x=4096+*((t->left)->varname)-'a';
		    fprintf(xsm_fl,"MOV [%d],R%d\n",x,m);
		    return m;
		    break;	     	    	  		  
	case EX:
	     l=codeGen(t->left);
	     m=codeGen(t->right);
	     switch(*(t->varname)){
	     
	     	case '+':
	     	     fprintf(xsm_fl,"ADD R%d, R%d\n",l,m);
     	  	     freereg();
     		     break;
     	  	case '-':	     
	             fprintf(xsm_fl,"SUB R%d, R%d\n",l,m);
     	             freereg();
     	             break;
     	  	case '*':
     	  	     fprintf(xsm_fl,"MUL R%d, R%d\n",l,m);
     	             freereg();
     	             break;
     	  	case '/':
     	  	     fprintf(xsm_fl,"DIV R%d, R%d\n",l,m);
     	  	     freereg();
     	  	     break;
	     }
	     return l;
	     break;
	case EQ:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"EQ R%d, R%d\n",l,m);
		freereg();
		return l;
		break;
	case NE:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"NE R%d, R%d\n",l,m);
		freereg();
		return l;
		break; 
	case GT:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"GT R%d, R%d\n",l,m);
		freereg();
		return l;
		break; 
	case GE:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"GE R%d, R%d\n",l,m);
		freereg();
		return l;
		break; 
	case LT:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"LT R%d, R%d\n",l,m);
		freereg();
		return l;
		break; 
	case LE:
		l=codeGen(t->left);
		m=codeGen(t->right);
		fprintf(xsm_fl,"LE R%d, R%d\n",l,m);
		freereg();
		return l;
		break;
	case IF:
		lb1=getLabel();
		l=codeGen(t->left);
		fprintf(xsm_fl,"JZ R%d, L%d\n",l,lb1);
            	freereg();
	        m=codeGen(t->mid); 
	        if(t->right!=NULL)
	        {
	         lb2=getLabel();
	         fprintf(xsm_fl,"JZ R%d, L%d\n",l,lb2);
	        }    	  
 	         fprintf(xsm_fl,"L%d:\n",lb1);
 	        if(t->right!=NULL)
 	        {
 	        freereg();
 	        m=codeGen(t->right);
 	        fprintf(xsm_fl,"L%d:\n",lb2);
 	        } 
 	        return m;
 	        break;
	case WHILE:
		 lb1=getLabel();	        
		 lb2=getLabel();
		 label_insert(lb2,lb1);
		 fprintf(xsm_fl,"L%d:\n",lb1);
    	         l=codeGen(t->left);
    	         fprintf(xsm_fl,"JZ R%d, L%d\n",l,lb2);
    	         m=codeGen(t->right);
    	         freereg();
    	         fprintf(xsm_fl,"JMP L%d\n",lb1);
    	         fprintf(xsm_fl,"L%d:\n",lb2);
		 return l;
		 break; 
	case BRKP:
      		x=getreg();
        	fprintf(xsm_fl,"BRKP\n");
        	return x;
        	
        case BREAK:
      		x=getreg();
      		 z=label_pop();
      		fprintf(xsm_fl,"JMP L%d\n",z);
      		return x;
      		
      	
        case CONTINUE: 
      	        x=getreg();
      		int z=head->continue_label_val;
      		fprintf(xsm_fl,"JMP L%d\n",z);
      		return x;  	  
 }

}
//evaluate function......................................
int evaluate(struct tnode* t)
{   int lval,rval;
    int varct;
    switch((t->nodetype))
    {
    	case CONN:
    	       if(t->left!=NULL)
    	         {
    	         lval= evaluate(t->left);
    	      //   return lval;
    	         }
    	       if(t->right!=NULL)
    	          {
    	          rval=evaluate(t->right);
    	          return rval;
    	          }  
    		break;
    	case VAR:
    		varct=*(t->varname)-'a';
    		return var_array[varct];
    		break;
    	case ASSGN:
    		varct=*(t->left->varname)-'a';
    		rval=evaluate(t->right);
    		var_array[varct]=rval;
    		return rval;
    		break;			
    	case CONSTANT:
    	       return t->val;
    	       break;	
        case EX:
             switch(*(t->varname)){
            case '+' : return evaluate(t->left) + evaluate(t->right);
                       break;
            case '-' : return evaluate(t->left) - evaluate(t->right);
                       break;
            case '*' : return evaluate(t->left) * evaluate(t->right);
                       break;
            case '/' : return evaluate(t->left) / evaluate(t->right);
                       break;
        }
       case EQ:
	  	if(evaluate(t->left)==evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break; 
       case NE:
		if(evaluate(t->left)!=evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break;
       case GE:
		if(evaluate(t->left)>=evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break;
       case GT:
		if(evaluate(t->left)>evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break;
      case LE:
		if(evaluate(t->left)<=evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break;
      case LT:
		if(evaluate(t->left)<evaluate(t->right)){
			lval = 1;
			return lval;			}
		else {lval=0;
		return lval;}
		break; 
       case READ:
       	     varct=*(t->left->varname)-'a';
       	     scanf("%d",&var_array[varct]);
       	     return var_array[varct];
       	     break;
       case WRITE: 	     	 	       
    	     rval=evaluate(t->left);
       	     printf("%d\n",rval);
       	     return rval;
       	     break;
       case IF: 
	    lval= evaluate(t->left);
	    if(lval){
		lval = evaluate(t->mid);
		return lval;
		}
	   else {rval =evaluate(t->right);
		return rval;	
			}
	   break;
       case WHILE:
	    while(evaluate(t->left)){
		rval = evaluate(t->right);
		}
		return rval;
	   break;
       case BREAK:exit(0);
	    break;
	case CONTINUE: return 0;
	    break;
	     	     
    }


}

void label_insert(int break_val,int cont_val)
{
  struct label_node *temp;
  temp=(struct label_node*)malloc(sizeof(struct label_node));
  temp->break_label_val=break_val;
  temp->continue_label_val=cont_val;
  temp->next=head;
  head=temp;
}
int label_pop()
{
if(head==NULL)
{
  return -1; 
}
else
{  int temp=head->break_label_val;
   head=head->next;
   return temp;
}}
//function to check AST
void tnode_print(struct tnode *t)
{  printf("\n[");
  printf("val %d\n",t->val);
  printf("type %d\n",t->type);
  printf("nodetype %d\n",t->nodetype);
  switch(t->nodetype){
  case 1:
    printf("EXPR\n");
    break;
    case 2:
    printf("ASSGN\n");
    break;
    case 3:
    printf("CONN\n");
    break;
    case 4:
    printf("CONSTANT\n");
    break;
    case 5:
    printf("READ\n");
    break;
    case 6:
    printf("VAR\n");
    break;
    case 7:
    printf("WRITE\n");
    break;
   
  }
  
  if(t->varname!=NULL)
  printf("varname %c\n",*(t->varname));
 printf("]\n");
}
// 
void inorder(struct tnode *t)
{ //printf("inside inorder");
if(t==NULL)
   return;
  inorder(t->left);
  tnode_print(t);
  inorder(t->right);

}
struct tnode* createTree(int val,int type,char* varname,int nodetype,struct tnode *l,struct tnode *m,struct tnode *r){
    //creating tnode.....
    struct tnode *temp;
    temp = (struct tnode*)malloc(sizeof(struct tnode));
    //assigning val,type,nodetype
    temp->val=val;
    temp->type=type;
    temp->nodetype=nodetype;
    //mallocing for varname
    if(varname!=NULL)
    {
    temp->varname=(char*)malloc(sizeof(char));
    *(temp->varname)=varname;
    }
    else
    {
    temp->varname=NULL;
    }
    //assigning left and right nodes....
    temp->left = l;
    temp->right = r;
    temp->mid =m;
    typecheck(temp);
    return temp;
}


/*int evaluate(struct tnode *t){
    if(t->varname == NULL){
      //  printf("%d\n",t->val);
        return t->val;
    }
    else{
        switch(*(t->varname)){
            case '+' : return evaluate(t->left) + evaluate(t->right);
                       break;
            case '-' : return evaluate(t->left) - evaluate(t->right);
                       break;
            case '*' : return evaluate(t->left) * evaluate(t->right);
                       break;
            case '/' : return evaluate(t->left) / evaluate(t->right);
                       break;
        }
    }
}*/
// library call function
void lb_call(char *arg,int file_handler,int var_pos,int p )
{
   		    fprintf(xsm_fl,"MOV R%d, \"%s\"\n",p,arg);
		    fprintf(xsm_fl,"PUSH R%d\n",p);
		    fprintf(xsm_fl,"MOV R%d, %d\n",p,file_handler);
		    fprintf(xsm_fl,"PUSH R%d\n",p);
            if(arg[0]=='R')
		    fprintf(xsm_fl,"MOV R%d, %d\n",p,var_pos);
	     if(arg[0]=='R')
                   fprintf(xsm_fl,"PUSH R%d\n",p);
		  else
                  fprintf(xsm_fl,"PUSH R%d\n",var_pos);
                    fprintf(xsm_fl,"PUSH R0\n");
		    fprintf(xsm_fl,"PUSH R0\n");
		    fprintf(xsm_fl,"CALL 0\n");
		    fprintf(xsm_fl,"POP R%d\n",p);
		    fprintf(xsm_fl,"POP R0\n");
		    fprintf(xsm_fl,"POP R0\n");
		    fprintf(xsm_fl,"POP R0\n");
		    fprintf(xsm_fl,"POP R0\n");
}
void backup_stack(int p,int i)
{
  if(p==i)
     return ;
  fprintf(xsm_fl,"PUSH R%d\n",i);
  backup_stack(p,++i);   
}
void restore_stack(int p,int i)
{
  if(i<0)
   return ;
  fprintf(xsm_fl,"POP R%d\n",i);
  backup_stack(p,i--);   
}
int getLabel()
{
return lct++;
}

//To get and allocate register
int getreg()
{ if(g<20)
  return g++;
  else
  {
  printf("Out of Registers..\n"); 
  exit(1);
  }
}
//To free register
void freereg()
{if(g>0)
return g--;
}
