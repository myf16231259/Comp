#pragma once
#ifndef GETSYM_H
#define GETSYM_H

#define MAX_TOKEN 1000
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "error.h"

extern const char* TYPE[38];

extern char token[MAX_TOKEN];	// 分割出的单个单词
extern FILE *FIN;				//读文件指针
extern FILE *res;         //写文件指针
extern FILE* errout;
extern int num;
extern int linenum;
extern int namelen;
extern int presym;
extern int strlength;


/***********************************************
 对于分割出的单词使用value 对其进行标记
 value定义方式：
	0：ERROR
	1：CONSTTK
	2：INTTK
	3: CHARTK
	4：VOIDTK
	5: MAINTK
	6: IFTK
	7: ELSETK
	8: DOTK
	9: WHILETK
	10: SCANFTK
	11: PRINTFTK
	12: RETURNTK
	13: IDENFR
	14: INTCON
	15: STRCON
	16: CHARCON
	17: LBRACE  {
	18: RBRACE  }
	19: LPARENT (
	20: RPARENT )
	21: MULT 
	22: DIV
	23: COMMA
	24: SEMICN
	25: PLUS
	26: MINU
	27: ASSIGN
	28: EQL
	29: LSS
	30: LEQ
	31: GRE
	32: GEQ
	33: NEQ
	34: LBRACK [
	35: RBRACK ]
	36: FORTK
	37: EOF
 ***********************************************/

int getsym();
int isLetter(char a);
int isDigit(char a);
void cleartoken();
void __nextSemicolon();
void __nextCommaOrSemicolon();
void __nextClosingBraces();
void __nextReservedSymbol();
void __nextCommaOrRightParenthesesOrSemicolon();
#endif