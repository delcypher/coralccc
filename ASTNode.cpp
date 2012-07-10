#include <iostream>
#include "ASTNode.h"

//Get Token symbols (enum yytoken type)
#include "parser.hpp"
using namespace std;

BinaryInfixOperator::BinaryInfixOperator(ASTNode* lhs, int op, ASTNode* rhs) : BinaryOperator(lhs,rhs)
{
	//Set C Symbol
	switch(op)
	{
		/* Binary operators */
		case TAND: csymbol="&"; break;
		case TOR:  csymbol="|"; break;
		case TXOR: csymbol="^"; break;
		/* Comparison operators */
		case TDGT:
		case TFGT:
		case TIGT:
		case TLGT: csymbol=">"; break;

		case TDLT:
		case TFLT:
		case TILT:
		case TLLT: csymbol="<"; break;

		case TDLE:
		case TFLE:
		case TILE:
		case TLLE: csymbol="<="; break;

		case TDGE:
		case TFGE:
		case TIGE:
		case TLGE: csymbol=">="; break;

		case TDEQ:
		case TFEQ:
		case TIEQ:
		case TLEQ: csymbol="=="; break;

		case TDNE:
		case TFNE:
		case TINE:
		case TLNE: csymbol="!="; break;

		/* Arithmetic operators */
		case TADD: csymbol="+"; break;
		case TSUB: csymbol="-"; break;
		case TMUL: csymbol="*"; break;
		case TDIV: csymbol="/"; break;
		case TMOD: csymbol="%"; break;

		default:
			cerr << "BinaryInfixOperator : Opcode " << op << " not supported!";			
	}
}

ConstantBool::ConstantBool(int value)
{
	switch(value)
	{
		case TTRUE: csymbol="true";break;
		case TFALSE: csymbol="false";break;
		default:
			cerr << "ConstantBool : Opcode " << value << " not supported!";
	}
}

UnaryOperator::UnaryOperator(ASTNode* opa, int type) : operand(opa)
{
	switch(type)
	{
		/* Boolean Not */
		case TNOT: csymbol="~";break;

		/* Unary Math functions */
		case TSIN: csymbol="sin"; break;
		case TCOS: csymbol="cos"; break;
		case TTAN: csymbol="tan"; break;
		case TASIN: csymbol="asin"; break;
		case TACOS: csymbol="acos"; break;
		case TATAN: csymbol="atan"; break;
		case TEXP: csymbol="exp"; break;
		case TLOG: csymbol="log"; break;
		case TLOG10: csymbol="log10"; break;
		case TROUND: csymbol="round("; break; /* warning no round() in C */
		case TSQRT: csymbol="sqrt"; break;

		default:
			cerr << "UnaryOperator : Opcode " << type << " not supported";

	}
}
