#ifndef ASTNODE_H
#define ASTNODE_H 1
#include <string>
#include <ostream>
#include <map>

class ASTNode
{
	public:
		virtual void print(std::ostream& o)=0;

	protected:
		ASTNode() : csymbol() {};	
		std::string csymbol;
};

class BinaryOperator : public ASTNode
{
	protected:
		BinaryOperator(ASTNode* lhs, ASTNode* rhs) : left(lhs), right(rhs) { }
		ASTNode* left;
		ASTNode* right;
};

class UnaryOperator : public ASTNode
{
	public:
		UnaryOperator(ASTNode* opa, int type);
		virtual void print(std::ostream& o) { o << csymbol << "("; operand->print(o); o << ")";}
	protected:
		UnaryOperator(ASTNode* opa) : operand(opa) { } //For sub classes
		ASTNode* operand;
};

//e.g ( (double) a)
class CastOperator : public UnaryOperator
{
	public:
		CastOperator(ASTNode* opa, int type);
		virtual void print(std::ostream& o) { o << "( (" << csymbol << ") "; operand->print(o); o << ")";}
};

//Eg. A + B
class BinaryInfixOperator : public BinaryOperator
{
	public:
		BinaryInfixOperator(ASTNode* lhs, int op, ASTNode* rhs);
		virtual void print(std::ostream& o) { o << "("; left->print(o); o << csymbol; right->print(o); o << ")";}
};

//E.g TAN2()
class BinaryPrefixOperator : public BinaryOperator
{
	public:
		BinaryPrefixOperator(ASTNode* lhs, int op, ASTNode* rhs);
		virtual void print(std::ostream& o) { o << csymbol << "("; left->print(o); o << ","; right->print(o); o << ")";}
};


class Constant : public ASTNode
{
	protected:
		Constant() { };
	public:
		virtual void print(std::ostream& o) { o << csymbol;}
};

class ConstantInt : public Constant
{
	public:
		ConstantInt(std::string& s) { csymbol = s;}
};

class ConstantLong : public Constant
{
	public:
		ConstantLong(std::string& s) { csymbol = s; csymbol+="L";}
};

class ConstantFloat : public Constant
{
	public:
		ConstantFloat(std::string& s) { csymbol = s; csymbol+="F";}
};

class ConstantDouble : public Constant
{
	public:
		ConstantDouble(std::string& s) { csymbol = s;}
};

class ConstantBool : public Constant
{
	public:
		ConstantBool(int value);
};



class Variable : public ASTNode
{
	public:
		struct Container
		{
			std::string varValue; //e.g.  0.255
			std::string typeName; //e.g. double

			Container(std::string& _varValue, const char* _typeName) : varValue(_varValue), typeName(_typeName) { }
		};
		static std::map<std::string,Container> collection;
		static void dumpDeclarations(std::ostream& o);
		static void setVariableValue(std::string& id, std::string& value);
		virtual void print(std::ostream& o) { o << csymbol;}
		Variable(std::string& s, int dataType);

};

#endif
