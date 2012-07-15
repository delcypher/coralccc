#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "ASTNode.h"

using namespace std;

extern int yyparse();
extern void yyrestart(FILE* new_file);
extern ASTNode* root;

const char indent[] = "    ";

void handleArgs(int argc, char** argv);

void usage();

void generateCCode();

FILE* constraintsFile=0;
FILE* solutionsFile=0;

extern FILE* yyin;

int main(int argc, char** argv)
{
	handleArgs(argc,argv);

	//set the input file for the parser to use
	yyin=constraintsFile;

	//Start parsing the constraints
	yyparse();

	//We're done with constraints file now
	fclose(constraintsFile);

	//Optionally parse solutions to set variable values
	if(solutionsFile!=NULL)
	{
		yyrestart(solutionsFile);

		/* Do a basic sanity check to make
		*  sure we are parsing variable values
		*/
		char c = fgetc(solutionsFile);
		if(c != '{')
		{
			//check failed
			cerr << "<solutions> file invalid" << endl;
			fclose(solutionsFile);
			exit(1);
		}
		else
		{
			//rewind to the beginning for the parser
			if(fseek(solutionsFile,0L, SEEK_SET)!=0)
			{
				cerr << "Failed to rewind!" << endl;
				exit(1);
			}
		}

		yyparse();
		fclose(solutionsFile);
	}


	generateCCode();

	return 0;
}

void handleArgs(int argc, char** argv)
{
	if(argc != 2 && argc!=3)
		usage();

	//try to open files
	constraintsFile=fopen(argv[1],"r");

	if(constraintsFile==NULL)
	{
		cerr << "Failed to open file " << argv[1] << endl;
		exit(1);
	}

	if(argc==3)
	{
		//the path to a solution file is present so let's try to open the file

		solutionsFile=fopen(argv[2],"r");

		if(solutionsFile==NULL)
		{
			cerr << "Failed to open file " << argv[2] << endl;
			fclose(constraintsFile);
			exit(1);
		}
	}

}

void usage()
{
	cerr << "Usage: coralccc <contraint file> <solutions file>" << endl <<
		" <constraints file> - A file containing a set of constraints in CORAL's constraint input language" << endl <<
		" <solutions file> - A file containing a set of solutions (variable values for the corresponding constraints)." << 
		endl << endl <<
		"This will generate on stdout C Source code that will test the provided constraints." <<
		endl << endl <<
		"coralccc  <constraints file>" <<
		endl <<
		"Behaviour is the same as above except that the generated C code will not have values set for the variables" << endl;

	exit(1);
}

void generateCCode()
{

	/* Now we will start to print out the C code */
	cout << "#include <math.h>" << endl <<
	"main()" << "{" << endl;

	/* print out double declarations and definitions */
	Variable::dumpDeclarations(std::cout);

	cout << indent << "if(";

	root->print(std::cout);

	cout << ")" << endl << 
	indent << indent << "return 0;//Success " << endl 
	<< indent << "else" << endl 
	<< indent << indent << "return 1;//Failure" << endl;

	cout << "}" << endl;
}
