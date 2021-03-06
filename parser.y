%{
	#include "ASTNode.h"
        #include <cstdio>
        #include <cstdlib>
	ASTNode* root; /* the top level root node of our final AST */

	extern int yylex();
	void yyerror(const char* s);
%}

/* Represents the many different ways we can access our data */
%union {
	ASTNode* node;
	std::string *string;
	int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TID TDECLIT TINTLIT 
%token <token> TSOLVAR
%token <token> TBCONST TTRUE TFALSE TAND TOR TXOR TNOT
%token <token> TDGT TFGT TIGT TLGT
%token <token> TDLT TFLT TILT TLLT
%token <token> TDLE TFLE TILE TLLE
%token <token> TDGE TFGE TIGE TLGE
%token <token> TDEQ TFEQ TIEQ TLEQ
%token <token> TDNE TFNE TINE TLNE
%token <token> TASDOUBLE TASINT
%token <token> TADD TSUB TMUL TDIV TMOD 
%token <token> TATAN2 TPOW 
%token <token> TSIN TCOS TTAN TASIN TACOS TATAN TEXP TLOG TLOG10 TROUND TSQRT
%token <token> TDCONST TDVAR TFCONST TFVAR TICONST TIVAR TLCONST TLVAR
%token <token> TLBRACKET TRBRACKET TSEMICOL TCOMMA 
%token <token> TLBRACE TRBRACE TSOLEQ




/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <node>  path_condition syntax 
%type <node> bool_expression bool_expressions bool_const bool_operation
%type <node> castable_to_double castable_to_int
%type <node> double_relational double_expression double_arithmetic double_const double_variable double_binary_function double_unary_function double_cast
%type <node> float_relational float_expression float_arithmetic float_const float_variable
%type <node> integer_relational integer_expression integer_arithmetic integer_const integer_variable integer_cast
%type <node> long_relational long_expression long_arithmetic long_const long_variable
%type <token> double_comparison arithmetic_operator double_binary_operator double_unary_operator
%type <token> float_comparison
%type <token> integer_comparison 
%type <token> long_comparison
%type <token> bool_truth_value bool_binary_operator

/* These are dummys. We don't care about the type */
%type <node> solution_group solutions solution
%type <string> solution_value

%start syntax

%locations

%%

/* Our parser supports parsing path conditions (input constraints to CORAL) or
CORAL's output (listing the value of variables) */
syntax : path_condition | solution_group ;

/* Path condition grammar */

path_condition : bool_expressions { root = $1; }
		;

/* Bool constraints grammar */

bool_expression : bool_const 
	 | bool_operation 
	 | double_relational 
	 | float_relational 
	 | integer_relational 
	 | long_relational { $$ = $1; }
	 ;

bool_expressions : bool_expression { $$ = $1;}
	  | bool_expressions TSEMICOL bool_expression { $$ = new BinaryInfixOperator($1, TAND, $3);}
	  ;

bool_const : TBCONST TLBRACKET bool_truth_value TRBRACKET { $$ = new ConstantBool($3);}
	  ;

bool_truth_value : TTRUE | TFALSE ;

bool_operation : bool_binary_operator TLBRACKET bool_expression TCOMMA bool_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); }
		| TNOT TLBRACKET bool_expression TRBRACKET {$$ = new UnaryOperator($3,$1);}
		;

bool_binary_operator : TAND | TOR | TXOR ;

/* Double constraints grammar */

double_relational : double_comparison TLBRACKET double_expression TCOMMA double_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5);}
;

double_comparison : TDGT | TDLT | TDLE | TDGE | TDEQ | TDNE ; 

double_expression : double_arithmetic | 
		   double_const | 
		   double_variable | 
		   double_binary_function | 
		   double_unary_function | 
		   double_cast { $$ = $1;}
		   ;

double_arithmetic : arithmetic_operator TLBRACKET double_expression TCOMMA double_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5);};

arithmetic_operator : TADD | TSUB | TMUL | TDIV | TMOD ;

double_const : TDCONST TLBRACKET TDECLIT TRBRACKET { $$ = new ConstantDouble(*$3); delete $3;} ;

double_variable : TDVAR TLBRACKET TID TRBRACKET { $$ = new Variable(*$3,$1); delete $3;}  ;

double_binary_function : double_binary_operator TLBRACKET double_expression TCOMMA double_expression TRBRACKET { $$ = new BinaryPrefixOperator($3,$1,$5);} ;

double_binary_operator : TATAN2 | TPOW ;

double_unary_function : double_unary_operator TLBRACKET double_expression TRBRACKET { $$ = new UnaryOperator($3,$1);} ;

double_unary_operator : TSIN | TCOS | TTAN | TASIN | TACOS | TATAN | TEXP | TLOG | TLOG10 | TROUND | TSQRT ;

double_cast : TASDOUBLE TLBRACKET castable_to_double TRBRACKET { $$ = new CastOperator($3,$1);}

castable_to_double : float_expression | integer_expression | long_expression ;

/* Float constraints grammar */

float_relational : float_comparison TLBRACKET float_expression TCOMMA float_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); } ;

float_comparison : TFGT | TFLT | TFLE | TFGE | TFEQ | TFNE ;

float_expression : float_arithmetic
		| float_const
		| float_variable { $$ = $1;} 
		;

float_arithmetic : arithmetic_operator TLBRACKET float_expression TCOMMA float_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); } ;

float_const : TFCONST TLBRACKET TDECLIT TRBRACKET { $$ = new ConstantFloat(*$3); delete $3;} ;

float_variable : TFVAR TLBRACKET TID TRBRACKET { $$ = new Variable(*$3,$1); delete $3; } ;

/* Integer constraints grammar */

integer_relational : integer_comparison TLBRACKET integer_expression TCOMMA integer_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); } ;

integer_comparison : TIGT | TILT | TILE | TIGE | TIEQ | TINE ;

integer_expression : integer_arithmetic
		| integer_const
		| integer_variable
		| integer_cast { $$ = $1;}
		;

integer_arithmetic : arithmetic_operator TLBRACKET integer_expression TCOMMA integer_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); } ;

integer_const : TICONST TLBRACKET TINTLIT TRBRACKET  { $$ = new ConstantInt(*$3); delete $3; };

integer_variable : TIVAR TLBRACKET TID TRBRACKET  { $$ = new Variable(*$3,$1); delete $3; };

integer_cast :  TASINT TLBRACKET castable_to_int TRBRACKET  { $$ = new CastOperator($3,$1); };

castable_to_int : double_expression | float_expression | long_expression ; 

/* Long constraints grammar */

long_relational : long_comparison TLBRACKET long_expression TCOMMA long_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); };

long_comparison : TLGT | TLLT | TLLE | TLGE | TLEQ | TLNE ;

long_expression : long_arithmetic 
		| long_const
		| long_variable { $$ = $1;} ;

long_arithmetic : arithmetic_operator TLBRACKET long_expression TCOMMA long_expression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); };

long_const : TLCONST TLBRACKET TINTLIT TRBRACKET { $$ = new ConstantLong(*$3); delete $3; } ;

long_variable : TLVAR TLBRACKET TID TRBRACKET { $$ = new Variable(*$3,$1); delete $3; };

/* Solutions grammar */

solution_group : TLBRACE solutions TRBRACE { $$ = $2;}

solutions : solutions TCOMMA solution | solution ;

solution : TSOLVAR TINTLIT TSOLEQ solution_value { Variable::setVariableValue(*$2,*$4); }

solution_value : TDECLIT | TINTLIT ;
%%

/* This definition of yyerror should indicate where syntax problems are */
void yyerror(const char *s) { std::printf("Error: %s. Between l:%d,c:%d and l:%d,c:%d\n", s,yylloc.first_line, yylloc.first_column, yylloc.last_line, yylloc.last_column);  std::exit(1); }
