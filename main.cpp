#include <iostream>
#include <cstdio>
#include <cstring>
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

extern int yydebug;

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
			//Virtually put the character back.
			ungetc(c,solutionsFile);
		}

		yyparse();
		fclose(solutionsFile);
	}


	generateCCode();

	return 0;
}

void handleArgs(int argc, char** argv)
{
	if(argc < 2 || argc > 4)
		usage();

	int argNum=1;

	//check for debug arg
	if(strcmp(argv[argNum],"--debug")==0)
	{
		yydebug=1;//Enable Bison's debuggin output
		argNum++;
	}


	//try to open files
	constraintsFile=fopen(argv[argNum],"r");

	if(constraintsFile==NULL)
	{
		cerr << "Failed to open file " << argv[argNum] << endl;
		exit(1);
	}

	argNum++;
	// Check if there is the additional argument
	if( argNum == (argc -1) )
	{
		//the path to a solution file is present so let's try to open the file

		solutionsFile=fopen(argv[argNum],"r");

		if(solutionsFile==NULL)
		{
			cerr << "Failed to open file " << argv[argNum] << endl;
			fclose(constraintsFile);
			exit(1);
		}
	}

}

void usage()
{
	cerr << "Usage: coralccc [ --debug ] <contraint file> <solutions file>" << endl <<
		"       coralccc [ --debug ] <constraints file>" << endl << endl << 
		"--debug            : Show Bison's debug output for parsing" << endl <<
		"<constraints file> : A file containing a set of constraints in CORAL's constraint input language" << endl <<
		"<solutions file>   : A file containing a set of solutions (variable values for the corresponding constraints)." << 
		endl << endl <<
		"This will generate on stdout C Source code that will test the provided constraints." <<
		endl << 
		"The behaviour of the command without a <solutions file> will not have values set for the variables, however the rest of the C code is generated." << endl;

	exit(1);
}

void generateCCode()
{

	/* Now we will start to print out the C code */
	cout << "#include <math.h>" << endl;

	/* We insert an implementation of a round function */
	cout << "double round(double v)" << endl <<
	"{" << endl <<
	"    double intpart=0;" << endl <<
	"    double fractional = modf(v,&intpart);" <<
	"    return (fabs(fractional) >= 0.5)?(intpart + (double) 1):(intpart);" << endl <<
	"}" << endl << endl;


	cout << "main()" << "{" << endl;

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
