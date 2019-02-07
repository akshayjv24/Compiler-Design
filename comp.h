//defining constants
#define EX 1
#define ASSGN 2
#define CONN 3
#define CONSTANT 4
#define READ 5
#define VAR 6
#define WRITE 7
#define GT 8
#define GE 9 
#define LT 10
#define LE 11 
#define EQ 12 
#define NE 13
#define IF 14
#define WHILE 15
#define BREAK 16
#define CONTINUE 17
#define BRKP 18
#define INTEGER 1
#define BOOLE 2	
#define reg_index int;
FILE *yyin;
struct tnode* createTree(int val,int type,char* varname,int nodetype,struct tnode *l,struct tnode *m,struct tnode *r);
void inorder(struct tnode *t);
void tnode_print(struct tnode *t);
int getreg();
void freereg();
void typecheck(struct tnode *t);	
 FILE *xsm_fl;
typedef struct tnode { 
	int val;	// value of a number for NUM nodes.
	int type;	//type of variable
	char* varname;	//name of a variable for ID nodes  
	int nodetype;  // information about non-leaf nodes - read/write/connector/+/* etc.  
	struct tnode *left,*mid,*right;	//left, middle and right branches   
}tnode;
struct label_node
{
  int break_label_val;
  int continue_label_val;
  struct label_node* next;

};	
/*Make a leaf tnode and set the value of val field*/
struct tnode* makeLeafNode(int n);
	
/*To evaluate an expression tree*/
int evaluate(struct tnode *t);
reg_index codeGen( struct tnode *t);

void lb_call(char *arg,int file_handler,int var_pos,int p );
void backup_stack(int p,int i);
void answer_storer();
int label_pop();
void label_insert(int,int);
