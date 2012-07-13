%{
	#include "ASTNode.h"
        #include <cstdio>
        #include <cstdlib>
	ASTNode* root; /* the top level root node of our final AST */

	extern int yylex();
	void yyerror(const char *s) { std::printf("Error: %s\n", s);std::exit(1); }
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




/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <node>  pathcondition
%type <node> boolexpr boolexprs boolconst booloperation
%type <node> casttodoubleexpression
%type <node> doublerelational doubleexpression doublearithmetic doubleconst doublevariable doublebinaryfunction doubleunaryfunction doublecast
%type <token> doublecomparison doublearithmeticop doublebinaryoperator doubleunaryoperator
%type <token> booltruthvalue boolbinaryoperator

%start pathcondition

%%

pathcondition : boolexprs { root = $1; }
		;

boolexpr : boolconst { $$ = $1; }
	 | booloperation {$$ = $1;}
	 | doublerelational { $$ = $1;}
	 ;

boolexprs : boolexpr { $$ = $1;}
	  | boolexprs TSEMICOL boolexpr { $$ = new BinaryInfixOperator($1, TAND, $3);}
	  ;

boolconst : TBCONST TLBRACKET booltruthvalue TRBRACKET { $$ = new ConstantBool($3);}
	  ;

booltruthvalue : TTRUE | TFALSE ;

booloperation : boolbinaryoperator TLBRACKET boolexpr TCOMMA boolexpr TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5); }
		| TNOT TLBRACKET boolexpr TRBRACKET {$$ = new UnaryOperator($3,$1);}
		;

boolbinaryoperator : TAND | TOR | TXOR ;

doublerelational : doublecomparison TLBRACKET doubleexpression TCOMMA doubleexpression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5);}
;

doublecomparison : TDGT | TDLT | TDLE | TDGE | TDEQ | TDNE ; 

doubleexpression : doublearithmetic | 
		   doubleconst | 
		   doublevariable | 
		   doublebinaryfunction | 
		   doubleunaryfunction | 
		   doublecast { $$ = $1;}
		   ;

doublearithmetic : doublearithmeticop TLBRACKET doubleexpression TCOMMA doubleexpression TRBRACKET { $$ = new BinaryInfixOperator($3,$1,$5);};

doublearithmeticop : TADD | TSUB | TMUL | TDIV | TMOD ;

doubleconst : TDCONST TLBRACKET TDECLIT TRBRACKET { $$ = new ConstantDouble(*$3); delete $3;} ;

doublevariable : TDVAR TLBRACKET TID TRBRACKET { $$ = new Variable<double>(*$3); delete $3;}  ;

doublebinaryfunction : doublebinaryoperator TLBRACKET doubleexpression TCOMMA doubleexpression TRBRACKET { $$ = new BinaryPrefixOperator($3,$1,$5);} ;

doublebinaryoperator : TATAN2 | TPOW ;

doubleunaryfunction : doubleunaryoperator TLBRACKET doubleexpression TRBRACKET { $$ = new UnaryOperator($3,$1);} ;

doubleunaryoperator : TSIN | TCOS | TTAN | TASIN | TACOS | TATAN | TEXP | TLOG | TLOG10 | TROUND | TSQRT ;

doublecast : TASDOUBLE TLBRACKET casttodoubleexpression TRBRACKET { $$ = new CastOperator($3,$1);}

casttodoubleexpression : doubleexpression /* needs correcting! */ ;
