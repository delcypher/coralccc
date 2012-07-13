#include <iostream>
#include "ASTNode.h"

using namespace std;

extern int yyparse();
extern ASTNode* root;

int main(int argc, char **argv)
{
	yyparse();


	/* Now we will start to print out the C code */
	cout << "#include <math.h>" << endl <<
	"main()" << "{" << endl;

	/* print out double declarations and definitions */
	Variable::dumpDeclarations(std::cout);

	cout << "if(";
	root->print(std::cout);
	cout << ")" << endl << "return 0; " << endl << "else" << endl << "return -1;" << endl;

	cout << "}" << endl;

	return 0;
}
