#include <iostream>
#include "ASTNode.h"

//Get Token symbols (enum yytoken type)
#include "parser.hpp"
using namespace std;


map<string,Variable::Container> Variable::collection = map<string,Variable::Container>();

void Variable::dumpDeclarations(std::ostream& o)
{
	std::map<std::string,Container>::iterator it;

	ASTNode* printer=0;

	//loop over elements and output a C defintion and declaration
	for(it=collection.begin(); it != collection.end(); it++)
	{
		string type=(it->second).typeName;
		o << type << " " << it->first << "="; 
		
		//We should take advantage of The Constant Classes so we output the correct format
		if(type == "double")
			printer= new ConstantDouble((it->second).varValue);
		else if(type == "float")
			printer= new ConstantFloat((it->second).varValue);
		else if(type == "int") 
			printer= new ConstantInt((it->second).varValue);
		else if(type == "long") 
			printer= new ConstantLong((it->second).varValue);
		else
			cerr << "Variable::dumpDeclarations : Data type " << type << " not supported!" << endl;


		//print out the value
		printer->print(o);
		o << ";"  << endl;

		delete printer; //free up memory for next loop.
	}
}

Variable::Variable(std::string& s, int dataType)
{
	csymbol=s;

	//check if symbol is not stored
	if(collection.count(s) == 0)
	{
		string empty; //We don't know its value yet (that's for coral to figure out)
		Container temp(empty,"");

		//Set the correct data type
		switch(dataType)
		{
			case TDVAR: temp.typeName="double"; break;
			case TFVAR: temp.typeName="float"; break;
			case TIVAR: temp.typeName="int"; break;
			case TLVAR: temp.typeName="long"; break;

			default:
				cerr << "Variable : Data type " << dataType << " not supported!" << endl;
				return;
		}

		//Not in collection so add it with an empty string value
		collection.insert(std::pair<std::string,Variable::Container>(s,temp));
	}
}


void Variable::setVariableValue(std::string& id, std::string& value)
{
	//check variable is present first
	string name="ID_";
	name+=id;

	//check variable is in map
	if(collection.count(name) == 0)
	{
		cerr << "Variable ID_" << id << " does not appear to be stored!" << endl;
		return;
	}

	//Pull current value out of map
	map<string,Variable::Container>::iterator it=collection.find(name);

	//Set the numeric value
	cerr << "Requesting set ID_" << id << " to value " << value << endl;
	(it->second).varValue = value;
}

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

BinaryPrefixOperator::BinaryPrefixOperator(ASTNode* lhs, int op, ASTNode* rhs) : BinaryOperator(lhs,rhs)
{
	switch(op)
	{
		case TATAN2: csymbol="atan2"; break;
		case TPOW: csymbol="pow"; break;

		default:
			cerr << "BinaryPrefixOperator : Opcode " << op << " not supported!";

	}
}

CastOperator::CastOperator(ASTNode* opa, int type) : UnaryOperator(opa)
{
	//Override what the parent constructor did and change the csymbol

	switch(type)
	{
		case TASDOUBLE: csymbol="double";break;
		case TASINT: csymbol="int"; break;

		default:
			cerr << "CastOperator : Opcode " << type << " not supported!";
	}
}
