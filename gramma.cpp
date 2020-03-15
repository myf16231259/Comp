#include "gramma.h"
#include "macrodef.h"
#include "symtable.h"

const char* errormsg[] =       //出错信息
{
	/*0*/"未定义的标识符",
	/*1*/"非法字符",
	/*2*/"标识符重复定义",
	/*3*/"常量或变量未定义",
	/*4*/"函数未定义",
	/*5*/"符号表已满",
	/*6*/"未定义的标识符",
	/*7*/"单引号中出现多个字符",
	/*8*/"函数参数与定义时不符",
	/*9*/"有返回值函数中缺少返回值",
	/*10*/"无返回值函数中有返回值",
	/*11*/"缺少’ = ’",
	/*12*/"缺少分号",
	/*13*/"缺少花括号",
	/*14*/"缺少方括号",
	/*15*/"缺少括号",
	/*16*/"缺少逗号",
	/*17*/"缺少main函数",
	/*18*/"类型和值不符",
	/*19*/"缺少双引号",
	/*20*/"缺少单引号",
	/*21*/"标识符或字符串长度超过最大值",
	/*22*/"定义常量或变量或函数时缺失标识符或类型标识符",
	/*23*/"定义常量或变量时缺失等号",
	/*24*/" 定义常量或变量时缺失值",
	/*25*/"表达式残缺或错误",
	/*26*/"常量被赋值",
	/*27*/"未定义的标识符",
	/*28*/"缺少数组大小或数组大小小于0",
	/*29*/"未知语法成分",
	/*30*/"函数参数不完整",
	/*31*/"循环不完整",
	/*32*/"读函数缺少标识符",
	/*33*/"调用无返回值函数",
};
int errornum;
int sym;
bool hasglovar = false;
int iffuncall = 0;

void error(int errornum)
{
	printf("%s:the linenum is %d\n", errormsg[errornum], linenum);
}

void stillvar()//in->unprint,out->unprint
{
	if (sym != INTTK && sym != CHARTK) {
		if (cursymtable->lvl == 1) {
			interPrintGLOVAR();
			interPrintGLOVAREND();
			hasglovar = true;
		}
		////fprintf(res, "<变量说明>\n");
		return;
	}
	int symtype = sym;
	int temp[100];
	temp[0] = sym;
	char temp0[1000];
	strcpy(temp0, token);
	sym = getsym();
	temp[1] = sym;
	char temp1[1000];
	strcpy(temp1, token);
	int templen = namelen;
	if (sym != IDENFR) {
		error(22);
		__nextSemicolon();
		sym = getsym();
		return;
	}
	sym = getsym();
	if (sym == LPARENT) {
		if (cursymtable->lvl == 1) {
			interPrintGLOVAR();
			interPrintGLOVAREND();
			hasglovar = true;
		}
		////fprintf(res, "<变量说明>\n");
		////fprintf(res, "%s %s\n", TYPE[temp[0]], temp0);
		////fprintf(res, "%s %s\n", TYPE[temp[1]], temp1);
		insertFun(temp1, templen, symtype - 2);
		interPrintFUNDEF(temp1, templen);
		//printf("%d ****  %s\n", sus, temp1);
		returnfuncdefine();
	}
	else if (sym == COMMA || sym == SEMICN || sym == LBRACK) {
		//fprintf(res, "%s %s\n", TYPE[temp[0]], temp0);
		//fprintf(res, "%s %s\n", TYPE[temp[1]], temp1);
		if (sym == LBRACK) {
			vardefine(temp1, templen, symtype, ARR);
		}
		else {
			vardefine(temp1, templen, symtype, NOTARR);
		}
		stillvar();
	}
	else {
		error(29);
		__nextSemicolon();
		sym = getsym();
	}

}

int getnext() {
	int num;
	num = getsym();
	if (num != 37 && num != 0) {
		//fprintf(res, "%s %s\n", TYPE[num], token);
		//printf("%s %s\n", TYPE[sym], token);
	}
	return num;
}

void printInt(int id) {
	if (id == 14) {
		if (token[0] == '+' || token[0] == '-') {
			//fprintf(res, "<整数>\n");
		}
		else {
			//fprintf(res, "<无符号整数>\n");
			//fprintf(res, "<整数>\n");
		}
	}
}
//<程序>分析程序
void program()//<程序>分析程序
{
	sym = getsym();
	if (sym == CONSTTK)
	{
		constdeclaration();
	}//unprint
	while (sym == INTTK || sym == CHARTK || sym == VOIDTK) {
		if (sym == INTTK || sym == CHARTK)
		{
			int symtype = sym ;
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			if (sym == IDENFR)
			{
				char temp[1000];
				strcpy(temp, token);
				int templen = namelen;
				sym = getsym();//未fprintf
				if (sym == COMMA || sym == SEMICN || sym == LBRACK)
				{
					if (sym == LBRACK) {
						vardefine(temp, templen, symtype,ARR);
					}
					else {
						vardefine(temp, templen, symtype,NOTARR);
					}
					stillvar();
				}
				else if (sym == LPARENT) {
					if (hasglovar == false) {
						interPrintGLOVAREND();
						hasglovar = true;
					}
					//printf("insert_func%s___type_is_%d\n", temp, symtype - 2);
					insertFun(temp, templen, symtype - 2);
					interPrintFUNDEF(temp, templen);
					//printf("%d  ****** %s\n", sus, temp);
					returnfuncdefine();
				}		
				else
				{
					error(29);
					__nextSemicolon();
					sym = getsym();
				}
			}
			else 
			{
				error(22);
				__nextSemicolon();
				sym = getsym();
			}
		}//未fprintf
		else 
		{
			if (hasglovar == false) {
				interPrintGLOVAREND();
				hasglovar = true;
			}
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			if (sym == IDENFR) {
				char temp[1000];
				strcpy(temp, token);
				int templen = namelen;
				interPrintFUNDEF(temp, templen);
				voidfuncdefine();
			}
			else if (sym == MAINTK)
				mainfunc();
			else
				error(29);
		}
	}
	//fprintf(res, "<程序>\n");
	if (sym != EOFFILE)
		printf("!EOFFILE\n");
}
//常量说明分析程序
void constdeclaration()//  in->unprint,out->unprint
{
	//fprintf(res, "%s %s\n", TYPE[sym], token);
	sym = getnext();
	if (sym == INTTK || sym == CHARTK)
	{
		constdefine(sym);
		//printf("This is a constdeclartion\n");
		sym = getsym();
		if (sym == CONSTTK) {
			////fprintf(res, "%s %s\n", TYPE[sym], token);
			constdeclaration();
		}	
		else {
			//fprintf(res, "<常量说明>\n");
			////fprintf(res, "%s %s\n", TYPE[sym], token);
			return;
		}		
	}
	else {
		error(22);
		__nextSemicolon();
		sym = getsym();
	}
		
}
//
void getIden(char* name, int* nameleng) {
	int i = 0;
	for (i = 0; i < IDE_MAX_LEN; i++) {
		if (token[i] == '\0') {
			break;
		}
		name[i] = token[i];
	}
	*nameleng = i;
}
//
void __nextCorSforConst(int x) {
	__nextCommaOrSemicolon();
	if (sym == COMMA) {
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		constdefine(x);
	}
	else {
		//fprintf(res, "<常量定义>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
	}
}
//
void getvalue(int* symvalue,int symbol,int x) {
	if (symbol == 0 || symbol == 1) {
		if (x == INTTK)
			*symvalue = atoi(token);
		else
			*symvalue = token[0];
	}
	else
		*symvalue = 0 - atoi(token);
}
//常量定义分析程序
void constdefine(int x)//in->print,out->print
{
	char symname[IDE_MAX_LEN];
	int symlen, symvalue;
	int symbol = 0;//unsign 0 , + 1, - 2
	sym = getnext();
	if (sym == IDENFR)
	{
		getIden(symname, &symlen);
		sym = getnext();
		if (sym == ASSIGN) {
			sym = getnext();
			if (sym == PLUS || sym == MINU) {
				if (sym == PLUS)
					symbol = 1;
				else
					symbol = 2;
				sym = getnext();
			}
			if ((sym == INTCON && x == INTTK) || (sym == CHARCON && x == CHARTK))
			{
				getvalue(&symvalue,symbol,x);
				printInt(sym);
				addSym(symname, symlen, CONSTANT, x-2, NOTARR, ZERO, symvalue, 1);
				//printf("______%d\n", cursymtable->lvl);
				if (cursymtable->lvl == 1) {
					interPrintGLOCST(symname, symlen, x - 2, symvalue);
				} 
				else {
					int index = searchSym(symname, symlen);
					interPrintFUNCST(x-2, symvalue, cursymtable->sym[index].offset);
				}
				sym = getsym();
				if (sym == COMMA) {
					//fprintf(res, "%s %s\n", TYPE[sym], token);
					constdefine(x);
				}
				else if (sym != SEMICN) {
					error(12);//缺少分号
					err('k');
					__nextSemicolon();
				}			
				else {
					//fprintf(res, "<常量定义>\n");
					//fprintf(res, "%s %s\n", TYPE[sym], token);
				}
					return;
			}
			else {
				error(18);
				err('o');
				__nextCorSforConst(x);
			}
		}
		else {
			error(23);//缺少等号
			__nextCorSforConst(x);
		}		
	}
	else {
		error(22);//缺少标识符
		__nextCorSforConst(x);
	}
}
//变量定义
void vardefine(char *symname,int symlen,int symtype,int ifarr)//in->unprint,out->unprintf
{
	int upbound;
	if (sym == COMMA)
	{
		if (ifarr == ARR) {
			addSym(symname, symlen, VARIABLE, symtype - 2, ARR, num, ZERO, 0);
		}
		else {
			addSym(symname, symlen, VARIABLE, symtype - 2, NOTARR, ZERO, ZERO, 0);
		}
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == IDENFR)
		{
			char temp[1000];
			strcpy(temp, token);
			int templen = namelen;
			sym = getsym();
			if (sym == COMMA)
			{
				////fprintf(res, "%s %s\n", TYPE[sym], token);
				vardefine(temp, templen, symtype,NOTARR);
			}
			else if (sym == SEMICN)
			{
				//fprintf(res, "<变量定义>\n");
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				addSym(temp, templen, VARIABLE, symtype - 2, NOTARR,ZERO, ZERO, 0);
				sym = getsym();
			}
			else if (sym == LBRACK)
			{
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getnext();
				if (sym == INTCON && num > 0)
				{
					upbound = num;
					//fprintf(res, "<无符号整数>\n");
					sym = getnext();
					if (sym == RBRACK)
					{
						//addSym(temp, templen, VARIABLE, symtype - 2, ARR, upbound, ZERO, 0);
						sym = getsym();
						if (sym == COMMA || sym == SEMICN)
							vardefine(temp, templen, symtype,ARR);
						else
						{
							error(12);
							if (sym != IDENFR) {
								err('k');
							}
							__nextCommaOrSemicolon();
							presym = RBRACK;
							vardefine(symname, symlen, symtype,ARR);
						}
					}
					else {
						error(14);
						err('m');
						__nextCommaOrSemicolon();
						presym = RBRACK;
						vardefine(symname, symlen, symtype,ARR);
					}
				}
				else
				{
					error(28);
					err('i');
					__nextCommaOrSemicolon();
					presym = RBRACK;
					vardefine(symname, symlen, symtype,ARR);
				}
			}
		}
		else {
			error(22);
			__nextCommaOrSemicolon();
			presym = RBRACK;
			vardefine(symname, symlen, symtype,ARR);
		}
	}
	else if (sym == SEMICN)
	{
		if (ifarr == ARR) {
			addSym(symname, symlen, VARIABLE, symtype - 2, ARR, num, ZERO, 0);
		}
		else {
			addSym(symname, symlen, VARIABLE, symtype - 2, NOTARR, ZERO, ZERO, 0);
		}
		//fprintf(res, "<变量定义>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getsym();
	}
	else
	{
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == INTCON && num > 0)
		{
			upbound = num;
			//fprintf(res, "<无符号整数>\n");
			sym = getnext();
			if (sym == RBRACK)
			{
				//addSym(symname, symlen, VARIABLE, symtype - 2, ARR, upbound, ZERO, 0);
				sym = getsym();
				if (sym == COMMA || sym == SEMICN)
					vardefine(symname,symlen,symtype,ARR);
				else
				{
					error(12);
					if (sym != IDENFR) {
						err('k');
					}
					__nextCommaOrSemicolon();
					vardefine(symname, symlen, symtype,ARR);
				}
			}
			else {
				error(14);
				err('m');
				__nextCommaOrSemicolon();
				presym = RBRACK;
				vardefine(symname, symlen, symtype,ARR);
			}
		}
		else
		{
			error(28);
			err('i');
			__nextCommaOrSemicolon();
			presym = RBRACK;
			vardefine(symname, symlen, symtype,ARR);
		}
	}
}
//有返回值函数定义
void returnfuncdefine()//in->unprint,out->unprintf
{
	int hasretst = 0;
	resetRegPool();
	//fprintf(res, "<声明头部>\n");
	//fprintf(res, "%s %s\n", TYPE[sym], token);
	sym = getsym();
	while (sym == INTTK || sym == CHARTK)
	{
		char paraname[IDE_MAX_LEN];
		int paranamelen, paratype;
		paratype = sym;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == IDENFR)
		{
			strcpy(paraname, token);
			paranamelen = namelen;
			addPara(paraname, paranamelen, paratype - 2, 0);
			sym = getsym();
		}
		else {
			error(30);
			__nextReservedSymbol();
			return;
		}
		if (sym != COMMA)
			break;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getsym();
		if (sym != INTTK && sym != CHARTK)
		{
			error(20);
			__nextReservedSymbol();
			return;
		}
	}
	if (sym == RPARENT)
	{
		//fprintf(res, "<参数表>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == LBRACE)
		{
			sym = getsym();
			if (sym == CONSTTK)
				constdeclaration();
			if (sym == INTTK || sym == CHARTK)
			{
				int symtype = sym;
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getnext();
				if (sym == IDENFR)
				{
					char temp[1000];
					strcpy(temp, token);
					int templen = namelen;
					sym =  getsym();
					if (sym == COMMA || sym == SEMICN || sym == LBRACK)
					{
						if (sym == LBRACK) {
							vardefine(temp, templen, symtype, ARR);
						}
						else {
							vardefine(temp, templen, symtype, NOTARR);
						}
						stillvar();
					}
					else
					{
						error(12);
						__nextReservedSymbol();
						return;
					}
				}
				else
				{
					error(22);
					__nextReservedSymbol();
					return;
				}
			}//未printf
			interPrintFUNSAVE();
			statementarray(&hasretst);
			//fprintf(res, "<复合语句>\n");
			if (sym == RBRACE)
			{
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				//fprintf(res, "<有返回值函数定义>\n");
				sym = getsym();
				if (hasretst == 0) {
					interPrintRETURN();
				}			
			}
			else
			{
				error(13);
				__nextReservedSymbol();
				return;
			}
		}
		else
		{
			error(13);
			__nextReservedSymbol();
			return;
		}
	}
	else
	{
		error(15);
		__nextReservedSymbol();
		return;
	}
}
//assign
int isAssignmentStatement(char* leftname, int leftnamelen) {//赋值语句
	int index_tmp, global_flag;
	int lefttype, leftarr, leftconstant,leftoffset;
	int exp_type, exp_const, exp_value, exp_counter;
	int offset;
	index_tmp = searchSym(leftname, leftnamelen);
	//printf("++++++++++++++%d\n", index_tmp);
	if (index_tmp != -1) {
		leftoffset = cursymtable->sym[index_tmp].offset;
		leftconstant = cursymtable->sym[index_tmp].kind;
		lefttype = cursymtable->sym[index_tmp].type;
		leftarr = cursymtable->sym[index_tmp].array;
		global_flag = 0;
	}
	else {
		//showGloSym();
		index_tmp = searchGlobalSym(leftname, leftnamelen);
		//printf("++++++++++++++%d\n", index_tmp);
		if (index_tmp != -1) {
			leftoffset = cursymtable->sym[index_tmp].offset;
			leftconstant = glosymtable->sym[index_tmp].kind;
			lefttype = glosymtable->sym[index_tmp].type;
			leftarr = glosymtable->sym[index_tmp].array;
			global_flag = 1;
		}
		else {
			//err 未定义的标识符
			error(0);
			__nextSemicolon();
			return 0;
		}
	}
	if (sym == ASSIGN) {
		sym = getnext();
		//printf("####%s%d", token, sym);
		if (leftarr == ARR || leftconstant == CONSTANT) {
			//err 标识符不允许的赋值方式
			err('j');
			__nextSemicolon();
			return 0;
		}
		if (!expression(&exp_type, &exp_const, &exp_value,&exp_counter)) {
			__nextSemicolon();
			return 0;
		}
		//printf("%s,,,,,%d,,%d,,%d\n", leftname, exp_const, exp_type, exp_value);
		if (lefttype != exp_type) {
			//err wrong type of assignment
			error(25);
			__nextSemicolon();
			return 0;
		}
		if (exp_const) {
			if (global_flag) {
				interPrintASSGLOCST(leftname, leftnamelen, exp_type, exp_value);
				glosymtable->sym[index_tmp].definite = 0;
				glosymtable->sym[index_tmp].value = exp_value;
			}
			else {
				offset = cursymtable->sym[index_tmp].offset;
				interPrintASSCURCST(exp_type, offset, exp_value);
				cursymtable->sym[index_tmp].definite = 0;
				cursymtable->sym[index_tmp].value = exp_value;
			}
		}
		else {
			//printf("*****%s", leftname);
			if (global_flag) {
				interPrintASSGLOVAR(leftname, leftnamelen, exp_type, exp_counter);
				glosymtable->sym[index_tmp].definite = 0;
			}
			else {
				offset = cursymtable->sym[index_tmp].offset;
				interPrintASSCURVAR(exp_type, offset, exp_counter);
				cursymtable->sym[index_tmp].definite = 0;
			}
		}
		
	}
	else if (sym == LBRACK) {
		sym = getnext();
		int leftexp_type, leftexp_const, leftexp_value, leftexp_counter;
		if (leftarr == NOTARR) {
			//err 未定义的标识符
			error(25);
			__nextSemicolon();
			return 0;
		}
		if (!expression(&leftexp_type, &leftexp_const, &leftexp_value,&leftexp_counter)) {
			__nextSemicolon();
			return 0;
		}
		if (leftexp_type == CHAR) {
			//err 需要整型表达式
			error(25);
			err('i');
			__nextSemicolon();
			return 0;
		}
		if (sym == RBRACK) {
			sym = getnext();
			if (sym == ASSIGN) {
				sym = getnext();
				if (!expression(&exp_type, &exp_const, &exp_value,&exp_counter)) {
					__nextSemicolon();
					return 0;
				}
				if (exp_type != lefttype) {
					//err 赋值语句左右类型不同
					error(25);
					__nextSemicolon();
					return 0;
				}
				interPrintASSARR(global_flag, leftname, leftnamelen, exp_type, leftoffset, leftexp_const, leftexp_value, leftexp_counter, exp_const, exp_value, exp_counter);
				if (!leftexp_const) {
					releaseRegCounter(leftexp_counter);
				}
				if (!exp_const) {
					releaseRegCounter(exp_counter);
				}
			}
			else {
				//need =
				error(25);
				__nextSemicolon();
				return 0;
			}
		}
		else {
			//need ]
			err('m');
			__nextSemicolon();
			return 0;
		}
	}
	else {
		error(15);
		__nextSemicolon();
		return 0;
	}
	return 1;
}
//语句列
void statementarray(int* hasretst)//in->unprint,out->unprint
{

	while (sym == IFTK || sym == DOTK || sym == FORTK || sym == LBRACE || sym == IDENFR || sym == PRINTFTK || sym == SCANFTK || sym == RETURNTK || sym == SEMICN || sym ==WHILETK)
	{
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		////fprintf(res, "<############>\n");
		statement(hasretst);
		////fprintf(res, "%s************** %s\n", TYPE[sym], token);
	}
	//fprintf(res, "<语句列>\n");
	//	printf("This is a statementarray\n");
}
//语句
void statement(int* hasretst)//in->print,out->unprintf
{

	if (sym == IFTK)
		ifstatement(hasretst);
	else if (sym == DOTK || sym == WHILETK )
		dostatement(hasretst);
	else if (sym == FORTK)
		forstatement(hasretst);
	else if (sym == LBRACE)//in->print,out->unprint
	{
		sym = getsym();
		statementarray(hasretst);
		if (sym == RBRACE) {
			
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getsym();
			////fprintf(res, "%s %s\n", TYPE[sym], token);
		}
		else
		{
			error(13);
			__nextSemicolon();
			sym = getsym();
		}
	}
	else if (sym == SCANFTK)
	{
		scanfstatement();
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		if (sym == SEMICN)
			sym = getsym();
		else
		{
			error(12);
			__nextSemicolon();
			sym = getsym();
		}
	}
	else if (sym == PRINTFTK)
	{
		printfstatement();
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		if (sym == SEMICN)
			sym = getsym();
		else
		{
			error(12);
			__nextSemicolon();
			sym = getsym();
		}
	}
	else if (sym == SEMICN) {
		sym = getsym();
		//printf("This is a NULL statement\n");
	}
	else if (sym == RETURNTK)
	{
		returnstatement(hasretst);
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		if (sym == SEMICN)
			sym = getsym();
		else
		{
			error(12);
			__nextSemicolon();
			sym = getsym();
		}
	}
	else if (sym == IDENFR)
	{
		char temp[1000];
		strcpy(temp, token);
		int templen = namelen;
		sym = getnext();
		if (sym == LBRACK || sym == ASSIGN)
		{
			isAssignmentStatement(temp, templen);

			//fprintf(res, "<赋值语句>\n");
			//fprintf(res, "%s %s\n", TYPE[sym], token);	
			//printf("####%s%d", token,sym);
			if (sym == SEMICN)
				sym = getsym();
			else {
				err('k');
				//printf("**here");
				error(12);
			}
		}
		else if (sym == LPARENT)
		{
			int functype;
			funcall(temp,templen,&functype);
			//getTmpRegisters();
		}
		else
		{
			error(12);
			__nextSemicolon();
			sym = getsym();
		}

	}
	else
		{
		error(29);
		__nextSemicolon();
		sym = getsym();
		}
	//fprintf(res, "<语句>\n");
	/*if (sym == SEMICN) {
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		//fprintf(res, "<语句>\n");
		sym = getsym();
	} 
	else {
		//fprintf(res, "<语句>\n");
	}*/
}


//if
void ifstatement(int* hasretst)//in->print,out->unfprintf
{
	sym = getnext();
	int if_index;
	interPrintIfStart(&if_index);
	if (sym == LPARENT)
	{
		int opertype;
		int exp1_type, exp1_const, exp1_value,exp1_counter;
		int exp2_type, exp2_const, exp2_value,exp2_counter;
		sym = getnext();
		//printf("%s", token);
		expression(&exp1_type, &exp1_const, &exp1_value,&exp1_counter);
		//printf("%d__\n",exp1_value);
		if (exp1_type == CHAR) {
			//err need intexp
			err('f');
			__nextSemicolon();
			return;
		}
		if (sym == LSS || sym == LEQ || sym == GRE || sym == GEQ || sym == EQL || sym == NEQ)
		{
			opertype = sym;
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			expression(&exp2_type, &exp2_const, &exp2_value,&exp2_counter);
			if (exp2_type == CHAR) {
				//err need intexp
				err('f');
				__nextSemicolon();
				return;
			}
			interPrintJudge2(IFTK, if_index, opertype,
				exp1_const, exp1_value, exp1_counter,
				exp2_const, exp2_value, exp2_counter);
			if (!exp1_const) {
				releaseRegCounter(exp1_counter);
			}
			if (!exp2_const) {
				releaseRegCounter(exp2_counter);
			}
		}
		else {
			interPrintJudge1(IFTK, if_index, exp1_const, exp1_value, exp1_counter);
		}
		if (!exp1_const) {
			releaseRegCounter(exp1_counter);
		}
		if (sym != RPARENT) {
			error(15);
			err('l');
			__nextSemicolon();
			return;
		}
		//fprintf(res, "<条件>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		statement(hasretst);
		interPrintJElseEnd(if_index);
		interPrintIfEnd(if_index);
		if (sym == ELSETK)
		{
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			statement(hasretst);
		}
		interPrintELSEEnd(if_index);
		//fprintf(res, "<条件语句>\n");
		//sym = getsym();
	}
	else
	{
		error(15);
		__nextSemicolon();
		return;
	}
}
//do_while
void dostatement(int* hasretst)//in->print,out ->unprint
{

	if (sym == DOTK) {
		int while_index;
		interPrintWhileStart(&while_index);
		sym = getnext();
		statement(hasretst);
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		if (sym != WHILETK)
		{
			error(31);
			err('n');
			__nextSemicolon();
			return;
		}
		sym = getnext();
		if (sym == LPARENT)
		{
			int opertype;
			int exp1_type, exp1_const, exp1_value, exp1_counter;
			int exp2_type, exp2_const, exp2_value, exp2_counter;
			sym = getnext();
			expression(&exp1_type, &exp1_const, &exp1_value, &exp1_counter);
			if (exp1_type == CHAR) {
				//err need intexp
				err('f');
				__nextSemicolon();
				return;
			}
			if (sym == LSS || sym == LEQ || sym == GRE || sym == GEQ || sym == EQL || sym == NEQ)
			{
				opertype = sym;
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getnext();
				expression(&exp2_type, &exp2_const, &exp2_value, &exp2_counter);
				if (exp2_type == CHAR) {
					//err need intexp
					err('f');
					__nextSemicolon();
					return;
				}
				interPrintJudge2(DOTK, while_index, opertype,
					exp1_const, exp1_value, exp1_counter,
					exp2_const, exp2_value, exp2_counter);
				if (!exp1_const) {
					releaseRegCounter(exp1_counter);
				}
				if (!exp2_const) {
					releaseRegCounter(exp2_counter);
				}
			}
			else {
				interPrintJudge1(DOTK, while_index, exp1_const, exp1_value, exp1_counter);
			}
			if (!exp1_const) {
				releaseRegCounter(exp1_counter);
			}
			if (sym != RPARENT)
			{
				error(15);
				__nextSemicolon();
				return;
			}
			//fprintf(res, "<条件>\n");
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getsym();
			//statement();
			//interPrintBackWhile(while_index);
			interPrintWhileEnd(while_index);
			//fprintf(res, "<循环语句>\n");
			//sym = getsym();
		}
		else
		{
			error(15);
			__nextSemicolon();
			return;
		}
	}
	else {
		int while_index;
		interPrintWhileStart(&while_index);
		sym = getnext();
		if (sym == LPARENT)
		{
			int opertype;
			int exp1_type, exp1_const, exp1_value,exp1_counter;
			int exp2_type, exp2_const, exp2_value,exp2_counter;
			sym = getnext();
			expression(&exp1_type, &exp1_const, &exp1_value,&exp1_counter);
			//printf("%d\n", exp1_value);
			if (exp1_type == CHAR) {
				//err need intexp
				err('f');
				__nextSemicolon();
				return;
			}
			if (sym == LSS || sym == LEQ || sym == GRE || sym == GEQ || sym == EQL || sym == NEQ)
			{
				opertype = sym;
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getnext();
				expression(&exp2_type, &exp2_const, &exp2_value,&exp2_counter);
				if (exp2_type == CHAR) {
					//err need intexp
					err('f');
					__nextSemicolon();
					return;
				}
				interPrintJudge2(WHILETK, while_index, opertype,
					exp1_const, exp1_value, exp1_counter,
					exp2_const, exp2_value, exp2_counter);
				if (!exp1_const) {
					releaseRegCounter(exp1_counter);
				}
				if (!exp2_const) {
					releaseRegCounter(exp2_counter);
				}
			}
			else {
				interPrintJudge1(WHILETK, while_index, exp1_const, exp1_value, exp1_counter);
			}
			if (!exp1_const) {
				releaseRegCounter(exp1_counter);
			}
			if (sym != RPARENT)
			{
				error(15);
				__nextSemicolon();
				return;
			}
			//fprintf(res, "<条件>\n");
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			statement(hasretst);
			interPrintBackWhile(while_index);
			interPrintWhileEnd(while_index);
			//fprintf(res, "<循环语句>\n");
			//sym = getsym();
		}
		else
		{
			error(15);
			__nextSemicolon();
			return;
		}
	}
	
}
//for_
void forstatement(int *hasretst)//in->print ,out->unprint
{
	int for_index;
	sym = getnext();
	if (sym == LPARENT)
	{
		sym = getnext();
		if (sym == IDENFR)
		{
			char temp[1000];
			strcpy(temp, token);
			int templen = namelen;
			sym = getnext();
			if (isAssignmentStatement(temp,templen))
			{
				if (sym == SEMICN)
				{
					interPrintForStart(&for_index);
					int opertype;
					int exp1_type, exp1_const, exp1_value,exp2_counter;
					int exp2_type, exp2_const, exp2_value,exp1_counter;
					sym = getnext();
					expression(&exp1_type, &exp1_const, &exp1_value,&exp1_counter);
					if (exp1_type == CHAR) {
						//err need intexp
						err('f');
						__nextSemicolon();
						return;
					}
					if (sym == LSS || sym == LEQ || sym == GRE || sym == GEQ || sym == EQL || sym == NEQ)
					{
						opertype = sym;
						//fprintf(res, "%s %s\n", TYPE[sym], token);
						sym = getnext();
						expression(&exp2_type, &exp2_const, &exp2_value,&exp2_counter);
						if (exp2_type == CHAR) {
							//err need intexp
							err('f');
							__nextSemicolon();
							return;
						}
						interPrintJudge2(FORTK, for_index, opertype,
							exp1_const, exp1_value, exp1_counter,
							exp2_const, exp2_value, exp2_counter);
						if (!exp1_const) {
							releaseRegCounter(exp1_counter);
						}
						if (!exp2_const) {
							releaseRegCounter(exp2_counter);
						}
					}
					else {
						interPrintJudge1(FORTK, for_index, exp1_const, exp1_value, exp1_counter);
					}
					//fprintf(res, "<条件>\n");
					//fprintf(res, "%s %s\n", TYPE[sym], token);
					if (sym == SEMICN)
					{
						sym = getnext();
						if (sym == IDENFR)
						{
							char temp[1000];
							strcpy(temp, token);
							int templen = namelen;
							sym = getnext();
							if (sym == ASSIGN) {
								sym = getnext();
								char temp1[1000];
								strcpy(temp1, token);
								int templen1 = namelen;
								sym = getnext();
								if (sym == PLUS || sym == MINU) {
									int opr = sym;
									sym = getnext(); 
									if (sym == INTCON) {
										int steplen = num;
										sym = getnext();
										if (sym == RPARENT) {
											sym = getnext();
											statement(hasretst);
											//printf("%s = %s %d %d\n", temp, temp, opr, steplen);
											stepadd(temp, templen,opr,steplen);
											interPrintBackFor(for_index);
											interPrintForEnd(for_index);
											//fprintf(res, "<循环语句>\n");
											//sym = getsym();
										}
									}
								}
							}
							else
							{
								error(31);
								__nextSemicolon();
								return;
							}
						}
						else
						{
							error(31);
							__nextSemicolon();
							return;
						}
					}
					else
					{
						error(31);
						__nextSemicolon();
						return;
					}
				}
				else
				{
					error(31);
					__nextSemicolon();
					return;
				}
			}
			else
			{
				error(31);
				__nextSemicolon();
				return;
			}
		}
		else
		{
			error(31);
			__nextSemicolon();
			return;
		}
	}
	else
		{
		error(15);
		__nextSemicolon();
		return;
		}
}
void stepadd(char* temp, int templen, int opr, int steplen) {
	int index_tmp;
	int offset;
	index_tmp = searchSym(temp, templen);
	//printf("++++++++++++++%d\n", index_tmp);
	if (index_tmp != -1) {
		offset = cursymtable->sym[index_tmp].offset;
	}
	interPrintStepadd(offset,opr,steplen);
	
}
//expression   
int expression(int *exp_type,int *exp_const,int *exp_value,int *counter)//in->print,out->unprintf
{
	int addflag = PLUS;
	int type_tmp, cst_tmp, value_tmp,counter_tmp;
	if (sym == PLUS || sym == MINU)
	{
		addflag = sym;
		sym = getnext();
	}
	if (!term(exp_type, exp_const, exp_value,counter)) {
		return 0;
	}
	if (*exp_const) {
		if (addflag == MINU) 
			*exp_value *= -1;
	}
	else {
		if (addflag == MINU)
			interPrintMinusCounter(counter, addflag - 24);
	}
	while (sym == PLUS || sym == MINU)
	{
		addflag = sym;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (!term(&type_tmp, &cst_tmp, &value_tmp,&counter_tmp)) {
			return 0;
		}
		//printf("%d\n", value_tmp);
		*exp_type = INT;
		if (!*exp_const) {
			if (!cst_tmp) {
				if (*counter < counter_tmp) {
					interPrintExpNDND(*counter, counter_tmp, addflag - 24);
					releaseRegCounter(counter_tmp);
				}
				else {
					interPrintExpNDND(counter_tmp, *counter, addflag - 24);
					releaseRegCounter(*counter);
					*counter = counter_tmp;
				}
			}
			else {
				interPrintExpDND(*counter, value_tmp, addflag-24, 2);
			}
		}
		else if (!cst_tmp) {
			interPrintExpDND(counter_tmp, *exp_value, addflag-24, 1);
			*exp_const = 0;
			*counter = counter_tmp;
		}
		else {
			if (addflag == PLUS)
				*exp_value += value_tmp;
			else
				*exp_value -= value_tmp;
		}
	}
	//fprintf(res, "<表达式>\n");
	////fprintf(res, "%s %s\n", TYPE[sym], token);
	return 1;
}
//项
int term(int* exp_type, int* exp_const, int* exp_value,int *counter)//in->print,out->unprint
{
	int mulflag;
	int type_tmp, cst_tmp, value_tmp,counter_tmp;
	if (!factor(exp_type, exp_const, exp_value,counter)) {
		return 0;
	}
	while (sym == MULT || sym == DIV)
	{
		mulflag = sym;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (!factor(&type_tmp,&cst_tmp,&value_tmp,&counter_tmp)) {
			return 0;
		}
		*exp_type = INT;
		if (!*exp_const) {
			if (!cst_tmp) {
				if (*counter < counter_tmp) {
					interPrintTrmNDND(*counter, counter_tmp, mulflag - 20);
					releaseRegCounter(counter_tmp);
				}
				else {
					interPrintTrmNDND(counter_tmp, *counter, mulflag - 20);
					releaseRegCounter(*counter);
					*counter = counter_tmp;
				}
			}
			else {
				interPrintTrmDND(*counter, value_tmp, mulflag - 20, 2);
			}
		}
		else if (!cst_tmp) {
			interPrintTrmDND(counter_tmp, *exp_value, mulflag - 20, 1);
			*exp_const = 0;
			*counter = counter_tmp;
		}
		else {
			if (mulflag == MULT/*'*'*/) {
				*exp_value *= value_tmp;
			}
			else {
				*exp_value /= value_tmp;
			}
		}
	}
	//fprintf(res, "<项>\n");
	return 1;
}
//因子
int factor(int* fac_type, int* fac_const, int* fac_value,int *counter)//in-print,out->unprint
{
	int index;
	int label;
	if (sym == IDENFR)
	{
		//printf("%s", token);
		char temp[1000];
		strcpy(temp, token);
		int templen = namelen;
		sym = getsym();
		if (sym == LBRACK)
		{
			int upbound_tmp, global_flag;
			index = searchSym(temp, templen);
			if (index != -1 && cursymtable->sym[index].array == ARR) {
				*fac_type = cursymtable->sym[index].type;
				*fac_const = 0;
				upbound_tmp = cursymtable->sym[index].upbound;
				global_flag = 0;
			}
			else {
				index = searchGlobalSym(temp, templen);
				//printf("++++++++++++++%d\n", index);
				if (index != -1 && glosymtable->sym[index].array == ARR) {
					*fac_type = glosymtable->sym[index].type;
					*fac_const = 0;
					upbound_tmp = glosymtable->sym[index].upbound;
					global_flag = 1;
				}
				else {//不存在符号
					//err using of a identifier without declaration
					err('c');
					//__nextSemicolon();
					return 0;
				}
			}
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			int index_type, index_const, index_value,index_counter;
			sym = getnext();
			if (!expression(&index_type, &index_const, &index_value,&index_counter)) {
				return 0;
			}
			if (index_type == CHAR) {
				//err expect an int expression here
				err('i');
				return 0;
			}
			//printf("%%%d______%d____%d\n", index_const, index_value,upbound_tmp);
			if (index_const) {
				if (index_value >= upbound_tmp || index_value < 0) {
					//err invalid index value
					error(35);
					return 0;
				}
				*counter = newRegCounter();
				//@利用value取数组值并赋给新的t
				if (global_flag) {
					int offset = index_value * (glosymtable->sym[index].type == INT ? 4 : 1);
					//printf("************%d", offset);
					interPrintGLOARRACC1(*counter, temp, templen, glosymtable->sym[index].type, offset);
				}
				else {
					int offset = index_value * (cursymtable->sym[index].type == INT ? 4 : 1) + cursymtable->sym[index].offset;
					interPrintCURARRACC1(*counter, cursymtable->sym[index].type, offset,iffuncall);
				}
			}
			else {
				if (global_flag) {
					interPrintGLOARRACC2(index_counter,temp, templen, glosymtable->sym[index].type, index_counter);
				}
				else {
					int offset = cursymtable->sym[index].offset;
					interPrintCURARRACC2(index_counter, cursymtable->sym[index].type, index_counter, offset,iffuncall);
				}
				*counter = index_counter;
				//@利用expression的t(index_counter)取到数组值赋给新的t
			}
			if (sym == RBRACK) {
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				//fprintf(res, "<因子>\n");
				sym = getsym();
				return 1;
			}		
			else {
				err('m');
				return 0;
			}	
		}
		else if (sym == LPARENT)
		{
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			funcall(temp,templen,fac_type);
			//showFunTable();
			//printf("%s\n", token);
			*fac_const = 0;
			//getTmpRegisters();
			*counter = newRegCounter();
			interPrintRTRVLE(*counter);
			//fprintf(res, "<因子>\n");
			return 1;
		}
		else {
			
			//fprintf(res, "<因子>\n");
			index = searchSym(temp, templen);
			if (index != -1 && cursymtable->sym[index].array == NOTARR) {
				*fac_type = cursymtable->sym[index].type;
				*fac_const = cursymtable->sym[index].definite;
				*fac_value = cursymtable->sym[index].value;
				if (!*fac_const) {
					*counter = newRegCounter();
					interPrintCURVARACC(*counter, *fac_type, cursymtable->sym[index].offset,iffuncall);
				}
			}
			else {
				index = searchGlobalSym(temp, templen);
				if (index != -1 && glosymtable->sym[index].array == NOTARR) {
					*fac_type = glosymtable->sym[index].type;
					*fac_const = glosymtable->sym[index].definite;
					*fac_value = glosymtable->sym[index].value;
					if (!*fac_const) {
						*counter = newRegCounter();
						interPrintGLOVARACC(*counter, temp, templen, *fac_type);
					}
				}
				else {//不存在符号
				//err using of a identifier without declaration
					error(32);
					return 0;
				}
			}
			return 1;
		}
	}
	else if (sym == LPARENT)
	{
		////fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (!expression(fac_type, fac_const, fac_value,counter)) {
			return 0;
		}
		if (sym == RPARENT) {
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			//fprintf(res, "<因子>\n");
			*fac_type = INT;
			sym = getsym();
			return 1;
		}
		else {
			err('l');
			return 0;
		}
	}
	else if (sym == INTCON) {
		////fprintf(res, "%s %s\n", TYPE[sym], token);
		printInt(sym);
		//fprintf(res, "<因子>\n");
		//printf("%s******%d\n", token, num);
		*fac_value = num;
		*fac_type = INT;
		*fac_const = 1;
		sym = getsym();
		return 1;
	}
	else if (sym == PLUS || sym == MINU) {
		label = sym;
		sym = getsym();
		if (sym == INTCON) {
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			printInt(sym);
			//fprintf(res, "<因子>\n");
			if (label == PLUS) {
				*fac_value = num;
				*fac_type = INT;
				*fac_const = 1;
			}
			else {
				*fac_value = 0 - num;
				*fac_type = INT;
				*fac_const = 1;
			}
			sym = getsym();
			return 1;
		}
	}
	else if (sym == CHARCON) {
		////fprintf(res, "%s %s\n", TYPE[sym], token);
		//fprintf(res, "<因子>\n");
		*fac_value = token[0];
		*fac_type = CHAR;
		*fac_const = 1;
		sym = getsym();
		return 1;
	}	
	else
	{
		error(29);
		return 0;
	}
}
//youfanhuizhi hanshu

//函数调用
void funcall(char* funname,int funnamelen,int *fun_type)//in->print,out->unprint
{
	int index_tmp;
	int funsize, paratype[1000], paraoffset[1000], numofpara;
	index_tmp = searchFun(funname, funnamelen);
	if (index_tmp == -1/*不存在函数*/) {
		//err no function
		error(4);
		__nextSemicolon();
		return ;
	}
	funsize = ftable->fun[index_tmp].stp->usespace;
	numofpara = ftable->fun[index_tmp].stp->numofpara;
	for (int i = 0; i < numofpara; i++) {
		paraoffset[i] = ftable->fun[index_tmp].stp->sym[i].offset;
		paratype[i] = ftable->fun[index_tmp].stp->sym[i].type;
	}
	*fun_type = ftable->fun[index_tmp].value;
	interPrintFUNUSE(funname, funnamelen, funsize);
	//saveTmpRegisters();
	sym = getsym();
	if (sym != RPARENT)
	{
		iffuncall = 1;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		int exp_type, exp_const, exp_value, exp_counter;
		int numnow = 0;	
		expression(&exp_type, &exp_const, &exp_value,&exp_counter);
		numnow++;
		if (numnow > numofpara) {
			//wrong amount of para
			err('d');
			return;
		}
		if (paratype[numnow - 1] != exp_type) {
			//wrong type of para
			err('e');
			return;
		}
		//interPrintFUNUSE(funname, funnamelen, funsize);
		interPrintPara(paraoffset[numnow - 1], exp_type, exp_const, exp_value, exp_counter);
		if (!exp_const) {
			releaseRegCounter(exp_counter);
		}
		while (sym == COMMA)
		{
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			expression(&exp_type, &exp_const, &exp_value,&exp_counter);
			numnow++;
			if (numnow > numofpara) {
				//wrong amount of para
				err('d');
				return;
			}
			if (paratype[numnow - 1] != exp_type) {
				//wrong type of para
				err('e');
				return;
			}
			//interPrintFUNUSE(funname, funnamelen, funsize);
			interPrintPara(paraoffset[numnow - 1], exp_type, exp_const, exp_value, exp_counter);
			if (!exp_const) {
				releaseRegCounter(exp_counter);
			}
		}
		if (numnow != numofpara) {
			error(30);
		}
		iffuncall = 0;
		if (sym == RPARENT)
		{
			if (numnow != numofpara) {
				//wrong amount of para
				err('d');
				return;
			}
			//interPrintFUNUSE(funname, funnamelen, funsize);
			saveTmpRegisters();
			interPrintFUNJMP(funname, funnamelen);
			getTmpRegisters();
			interPrintResetsp(funsize);
			//fprintf(res, "<值参数表>\n");
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			/*if (ftable->fun[index_tmp].value != VOID)
				//fprintf(res, "<有返回值函数调用语句>\n");
			else 
				//fprintf(res, "<无返回值函数调用语句>\n");
				*/
			sym = getsym();
		}
		else
		{
			err('l');
			__nextSemicolon();
		}
	}
	else
	{
		if (numofpara != 0) {
			err('d');
			//无参

		}
		saveTmpRegisters();
		interPrintFUNJMP(funname, funnamelen);
		getTmpRegisters();
		interPrintResetsp(funsize);
		//fprintf(res, "<值参数表>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		/*if (ftable->fun[index_tmp].value != VOID)
			//fprintf(res, "<有返回值函数调用语句>\n");
		else
			//fprintf(res, "<无返回值函数调用语句>\n");*/
		sym = getsym();
	}
}
//scanf
void scanfstatement()//in->print,out->unprint
{
	char idenname[IDE_MAX_LEN];
	int idennamelen, index_tmp;
	sym = getnext();
	if (sym == LPARENT)
	{
		sym = getnext();
		if (sym == IDENFR)
		{
			//printf("%s", token);
			strcpy(idenname, token);
			idennamelen = namelen;
			index_tmp = searchSym(idenname, idennamelen);
			if (index_tmp == -1) {
				index_tmp = searchGlobalSym(idenname, idennamelen);
				if (index_tmp == -1) {
					err('c');
					__nextCommaOrRightParenthesesOrSemicolon();
				}
				else {
					interPrintSCANFGLO(idenname, idennamelen, glosymtable->sym[index_tmp].type);
				}
			}
			else {
				interPrintSCANFCUR(cursymtable->sym[index_tmp].type, cursymtable->sym[index_tmp].offset);
			}
			sym = getnext();
			while (sym == COMMA)
			{
				sym = getnext();
				strcpy(idenname, token);
				idennamelen = namelen;
				index_tmp = searchSym(idenname, idennamelen);
				if (index_tmp == -1) {
					index_tmp = searchGlobalSym(idenname, idennamelen);
					if (index_tmp == -1) {
						err('c');
						__nextCommaOrRightParenthesesOrSemicolon();
					}
					else {
						interPrintSCANFGLO(idenname, idennamelen, glosymtable->sym[index_tmp].type);
					}
				}
				else {
					//printf("+++");
					interPrintSCANFCUR(cursymtable->sym[index_tmp].type, cursymtable->sym[index_tmp].offset);
				}
				sym = getnext();
			}
			//if (sym!= COMMASY)
			//	sym = getsym();
			if (sym == RPARENT)
			{
				sym = getsym();
				//fprintf(res, "<读语句>\n");
			}
			else
			{
				error(15);
				__nextSemicolon();
				return;
			}
		}
	}
	else
	{
		error(15);
		__nextSemicolon();
		return;
	}
}
//printf
void printfstatement()
{
	int exp_type, exp_value, exp_const,exp_counter;
	sym = getnext();
	if (sym == LPARENT)
	{
		sym = getnext();
		if (sym == STRCON)
		{
			interPrintPRINTFSTR(token, strlength);
			//fprintf(res, "<字符串>\n");
			sym = getnext();
			if (sym == COMMA)
			{
				//interPrintPRINTFSPACE();
				sym = getnext();
				if (!expression(&exp_type, &exp_const, &exp_value,&exp_counter)) {
					__nextSemicolon();
					return;
				}
				if (sym == RPARENT)
				{
					interPrintPRINTFEXP(exp_type, exp_const, exp_value, exp_counter);
					if (!exp_const) {
						releaseRegCounter(exp_counter);
					}
					interPrintPRINTFENTER();
					//fprintf(res, "%s %s\n", TYPE[sym], token);
					//fprintf(res, "<写语句>\n");
					sym = getsym();
				}
				else
				{
					error(15);
					__nextSemicolon();
					return;
				}
			}
			else if (sym == RPARENT)
			{
				interPrintPRINTFENTER();
				//fprintf(res, "<写语句>\n");
				sym = getsym();
			}
			else
			{
				error(15);
				__nextSemicolon();
				return;
			}
		}
		else
		{
			if (!expression(&exp_type, &exp_const, &exp_value,&exp_counter)) {
				__nextSemicolon();
				return;
			}
			if (sym == RPARENT)
			{
				interPrintPRINTFEXP(exp_type, exp_const, exp_value, exp_counter);
				interPrintPRINTFENTER();
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				//fprintf(res, "<写语句>\n");
				sym = getsym();
			}
			else
			{
				error(15);
				__nextSemicolon();
				return;
			}
		}
	}
	else
	{
		error(15);
		__nextSemicolon();
		return;
	}
}
//return 
void returnstatement(int* hasretst)//in->print,out->unprint
{
	*hasretst = 1;
	int exp_type, exp_const, exp_value,exp_counter;
	sym = getsym();
	if (sym == LPARENT)
	{
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym != RPARENT) {
			if (!expression(&exp_type, &exp_const, &exp_value, &exp_counter)) {
				__nextSemicolon();
				return;
			}
			if (sym == RPARENT)
			{
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getsym();
			}
			else
			{
				error(15);
				__nextSemicolon();
				return;
			}
			interPrintStoreReturnValue(exp_const, exp_value, exp_counter);
			if (!exp_const) {
				releaseRegCounter(exp_counter);
			}
		}
		else {
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getsym();
		}	
	}
	interPrintRETURN();
	//fprintf(res, "<返回语句>\n");
}
//void
void voidfuncdefine()//in ->print,out->
{
	int hasretst = 0;
	insertFun(token, namelen, VOID);
	sym = getnext();
	if (sym != LPARENT)
	{
		error(15);
		__nextReservedSymbol();
		return;
	}
	sym = getsym();
	while (sym == INTTK || sym == CHARTK)
	{

		char paraname[IDE_MAX_LEN];
		int paranamelen, paratype;
		paratype = sym;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == IDENFR)
		{
			strcpy(paraname, token);
			paranamelen = namelen;
			addPara(paraname, paranamelen, paratype - 2, 0);
			sym = getsym();
		}
		else {
			error(30);
			__nextReservedSymbol();
			return;
		}
		if (sym != COMMA)
			break;
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getsym();
		if (sym != INTTK && sym != CHARTK)
		{
			error(20);
			__nextReservedSymbol();
			return;
		}
	}
	if (sym == RPARENT)
	{
		//fprintf(res, "<参数表>\n");
		//fprintf(res, "%s %s\n", TYPE[sym], token);
		sym = getnext();
		if (sym == LBRACE)
		{
			sym = getsym();
			if (sym == CONSTTK)
				constdeclaration();
			if (sym == INTTK || sym == CHARTK)
			{
				int symtype = sym;
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				sym = getnext();
				if (sym == IDENFR)
				{
					char temp[1000];
					strcpy(temp, token);
					int templen = namelen;
					sym = getsym();
					if (sym == COMMA || sym == SEMICN || sym == LBRACK)
					{
						if (sym == LBRACK) {
							vardefine(temp, templen, symtype, ARR);
						}
						else {
							vardefine(temp, templen, symtype, NOTARR);
						}
						stillvar();
					}
					else
					{
						error(12);
						__nextReservedSymbol();
						return;
					}
				}
				else
				{
					error(22);
					__nextReservedSymbol();
					return;
				}
			}//未printf
			interPrintFUNSAVE();
			statementarray(&hasretst);
			//fprintf(res, "<复合语句>\n");
			if (sym == RBRACE)
			{
				//fprintf(res, "%s %s\n", TYPE[sym], token);
				//fprintf(res, "<无返回值函数定义>\n");
				sym = getsym();
				if (hasretst == 0) {
					interPrintRETURN();
				}
			}
			else
			{
				error(13);
				__nextReservedSymbol();
				return;
			}
		}
		else
		{
			error(13);
			__nextReservedSymbol();
			return;
		}
	}
	else
	{
		error(15);
		__nextReservedSymbol();
		return;
	}
}
//main函数分析程序
void mainfunc()
{
	int hasretst = 0;
	insertFun(token, 4, VOID);
	interPrintFUNDEF(token, 4);
	sym = getnext();
	if (sym == LPARENT)
	{
		sym = getsym();
		if (sym == RPARENT)
		{
			//fprintf(res, "<参数表>\n");
			//fprintf(res, "%s %s\n", TYPE[sym], token);
			sym = getnext();
			if (sym == LBRACE)
			{
				sym = getsym();
				if (sym == CONSTTK)
					constdeclaration();
				if (sym == INTTK || sym == CHARTK)
				{
					int symtype = sym;
					//fprintf(res, "%s %s\n", TYPE[sym], token);
					sym = getnext();
					if (sym == IDENFR)
					{
						char temp[1000];
						strcpy(temp, token);
						int templen = namelen;
						sym = getsym();
						if (sym == COMMA || sym == SEMICN || sym == LBRACK)
						{
							if (sym == LBRACK) {
								vardefine(temp, templen, symtype, ARR);
							}
							else {
								vardefine(temp, templen, symtype, NOTARR);
							}
							stillvar();
						}
						else
						{
							error(12);
							__nextReservedSymbol();
							return;
						}
					}
					else
					{
						error(22);
						__nextReservedSymbol();
						return;
					}
				}//未printf
				interPrintFUNSAVE();
				statementarray(&hasretst);
				//fprintf(res, "<复合语句>\n");
				if (sym == RBRACE)
				{
					//fprintf(res, "%s %s\n", TYPE[sym], token);
					//fprintf(res, "<主函数>\n");
					if (hasretst == 0) {
						interPrintRETURN();
					}				
					sym = getsym();
				}
				else
				{
					error(13);
					__nextReservedSymbol();
					return;
				}
			}
			else
			{
				error(13);
				__nextReservedSymbol();
				return;
			}
		}
		else
		{
			error(15);
			__nextReservedSymbol();
			return;
		}
	}
	else
	{
		error(15);
		__nextReservedSymbol();
		return;
	}
}