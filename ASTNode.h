#ifndef ASTNODE_H
#define ASTNODE_H 1
#include <string>
#include <ostream>
#include <vector>

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


template <class T>
class Variable : public ASTNode
{
	public:
		static std::vector<std::string> collection;
		Variable(std::string& s) { csymbol = s; collection.push_back(csymbol); }
		virtual void print(std::ostream& o) { o << csymbol;}
};

#endif
