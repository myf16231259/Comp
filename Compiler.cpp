
#include "getsym.h"
#include "gramma.h"
#include "symtable.h"

 
FILE* res;
FILE* errout;
const char* TYPE[] = {
	"ERROR","CONSTTK","INTTK","CHARTK","VOIDTK",
	"MAINTK","IFTK","ELSETK","DOTK","WHILETK",
	"SCANFTK","PRINTFTK","RETURNTK","IDENFR","INTCON",
	"STRCON","CHARCON","LBRACE","RBRACE",
	"LPARENT","RPARENT","MULT","DIV",
	"COMMA","SEMICN","PLUS","MINU","ASSIGN",
	"EQL","LSS","LEQ","GRE","GEQ","NEQ",
	"LBRACK","RBRACK","FORTK","EOFFILE"
};

int main()
{
	//int sym;
	FIN = fopen("testfile.txt", "r");
	res = fopen("output.txt", "w");
	errout = fopen("error.txt", "w");
	interPrintStart();
	ftable = newFunTable();
	glosymtable = newSymTable();
	cursymtable = glosymtable;
	if (FIN == NULL)
	{
		//printf("file not found\n");
		return 0;
	}
	
	program();
	interPrintEnd();
	finalPrintProceed();
	fclose(FIN);
	fclose(res);
	fclose(errout);
	
	return 0;
}