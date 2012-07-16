%{
/* Required includes */
#include <cstdio>
#include <string>

#include "ASTNode.h"

//Get tokens
#include "parser.hpp"

/* Convenience macro, we save a string description of the token for Bison */
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }

/* handle locations */
int yycolumn = 1;

/* Executed for every token. Used for error reporting. It is taken from
http://oreilly.com/linux/excerpts/9780596155971/error-reporting-recovery.html
*/
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
        yycolumn += yyleng;
%}

/* Enable the use of the non POSIX line number feature for error reporting! */
%option yylineno

/* Solution parsing mode for parsing solutions */
%x solution
%%

<*>[ \t] ; /* Ignore whitespace */
[\n]			{ yycolumn=1; /* Reset the column counter */ }
BCONST			return TOKEN(TBCONST); /* Binary constant operator */
TRUE			return TOKEN(TTRUE); /* Truth literal token */
FALSE			return TOKEN(TFALSE); /* Truth literal token */
BAND			return TOKEN(TAND);
BOR			return TOKEN(TOR);
BXOR			return TOKEN(TXOR);
BNOT			return TOKEN(TNOT);
DGT			return TOKEN(TDGT); /* Greater than tokens */
FGT			return TOKEN(TFGT);
IGT			return TOKEN(TIGT);
LGT			return TOKEN(TLGT);
DLT			return TOKEN(TDLT); /* less than tokens */
FLT			return TOKEN(TFLT);
ILT			return TOKEN(TILT);
LLT			return TOKEN(TLLT);
DLE			return TOKEN(TDLE); /* less than or equal to tokens */
FLE			return TOKEN(TFLE);
ILE			return TOKEN(TILE);
LLE			return TOKEN(TLLE);
DGE			return TOKEN(TDGE); /* Greater than or equal to tokens */
FGE			return TOKEN(TFGE);
IGE			return TOKEN(TIGE);
LGE			return TOKEN(TLGE);
DEQ			return TOKEN(TDEQ); /* equal to tokens */
FEQ			return TOKEN(TFEQ);
IEQ			return TOKEN(TIEQ);
LEQ			return TOKEN(TLEQ);
DNE			return TOKEN(TDNE); /* not equal to tokens */
FNE			return TOKEN(TFNE);
INE			return TOKEN(TINE);
LNE			return TOKEN(TLNE);
ASDOUBLE		return TOKEN(TASDOUBLE); /* Cast to double operator */
ASINT			return TOKEN(TASINT); /* Cast to int operator */
ADD			return TOKEN(TADD); /* Math tokens */
SUB			return TOKEN(TSUB);
MUL			return TOKEN(TMUL);
DIV			return TOKEN(TDIV);
MOD			return TOKEN(TMOD);
ATAN2_			return TOKEN(TATAN2);
POW_			return TOKEN(TPOW);
SIN_			return TOKEN(TSIN);
COS_			return TOKEN(TCOS);
TAN_			return TOKEN(TTAN);
ASIN_			return TOKEN(TASIN);
ACOS_			return TOKEN(TACOS);
ATAN_			return TOKEN(TATAN);
EXP_			return TOKEN(TEXP);
LOG_			return TOKEN(TLOG);
LOG10_			return TOKEN(TLOG10);
ROUND_			return TOKEN(TROUND);
SQRT_			return TOKEN(TSQRT);
DCONST			return TOKEN(TDCONST); /* Double constant operator */
DVAR			return TOKEN(TDVAR); /* Double variable operator */
FCONST			return TOKEN(TFCONST); /* Float constant operator */
FVAR			return TOKEN(TFVAR);
ICONST			return TOKEN(TICONST);
IVAR			return TOKEN(TIVAR);
LCONST			return TOKEN(TLCONST);
LVAR			return TOKEN(TLVAR);
";"			return TOKEN(TSEMICOL);
ID_[0-9]+		SAVE_TOKEN; return TID; /* Variable identifier */
<*>[-]?[0-9]+			SAVE_TOKEN; return TINTLIT; /* Integer literal */
<*>[-]?[0-9]+(\.[0-9]+)?	SAVE_TOKEN; return TDECLIT; /* Literal decimal number token */
<*>","			return TCOMMA; /* Available for all start conditions */
"("			return TOKEN(TLBRACKET);
")"			return TOKEN(TRBRACKET);

"{"			{ BEGIN(solution); return TOKEN(TLBRACE); }
<solution>"}"		{ BEGIN(INITIAL); return TOKEN(TRBRACE); }
<solution>\$V			return TOKEN(TSOLVAR);
<solution>"="			return TOKEN(TSOLEQ);

<*>.			fprintf(stderr,"Unknown token:%s\n",yytext);yyterminate();
%%
