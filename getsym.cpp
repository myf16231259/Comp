#include "getsym.h"
#include <ctype.h>

FILE *FIN;
char token[MAX_TOKEN];
int num;
int presym = 0;
int linenum = 1;
int namelen = 0;
int strlength = 0;
const char *Keyword[] = {					//关键字
	"const","int","char","void",
	"main","if","else", "do","while",
	"scanf","printf", "return","for"
};
int isLetter(char a)
{
	if ((a >= 'a'&&a <= 'z') || (a >= 'A'&&a <= 'Z')||(a == '_'))
		return 1;
	else
		return 0;
}
int isDigit(char a)
{
	if (a >= '0'&&a <= '9')
		return 1;
	else
		return 0;
}
void cleartoken()
{
	int i;
	for (i = 0; i < MAX_TOKEN; i++)
	{
		token[i] = '\0';
	}
	return;
}
int getsym()
{
	cleartoken();
	char ch;
	int i = 0;
	int j = 0;
	ch = fgetc(FIN);
	while (isspace(ch)) 
	{
		if (ch == '\n') {
			linenum++;
		}
		ch = fgetc(FIN);
	}
	token[i] = ch;
	i++;
	if (isLetter(ch))
	{
		ch = fgetc(FIN);
		while (isLetter(ch) || isDigit(ch))//将读取到的字母或数字拼接
		{
			token[i] = ch;
			i++;
			ch = fgetc(FIN);
		}
		ungetc(ch, FIN);
		//token[i] = '\0';
		for (j = 0; j < 13; j++)
		{
			if (!(strcmp(Keyword[j], token))) {
				if (j == 12) {
					presym = 36;
			        return 36;
				}
				else {
					presym = j + 1;
					return j + 1;   //返回保留字对应的类别编码
				}
			}		
		}
		if (j == 13) {
			presym = 13;
			namelen = i ;
			return 13; //标识符的类别码
		}	
		else {
			presym = 0;
			err('a');
			return 0;
		}	
	}
	else if (isDigit(ch))
	{
		if (ch == '0') {
			presym = 14;
			num = 0;
			return 14;
		}
		ch = fgetc(FIN);
		while (isDigit(ch))
		{
			token[i] = ch;
			i++;
			ch = fgetc(FIN);
		}
		ungetc(ch, FIN);
		//token[i] = '\0';
		num = atoi(token);
		presym = 14;
		return 14;// 整常数的类别码
	}
	else if (ch == '\"')
	{
		ch = fgetc(FIN);
		i--;
		while (ch >= 32 && ch <= 126 && ch != 34)
		{
			token[i] = ch;
			i++;
			ch = fgetc(FIN);
		}
		strlength = i;
		if (ch == '\"')
		{
			//token[i] = '\0';
			presym = 15;
			return 15; //字符串的类别码
		}
		else
		{
			ungetc(ch, FIN);
			err('a');
			//printf("缺少双引号！");
			presym = 0;
			return 0;
		}
	}
	else if (ch == '\'')
	{
		ch = fgetc(FIN);
		if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || isDigit(ch) || isLetter(ch))
		{
			token[0] = ch;
			i++;
			ch = fgetc(FIN);
			if (ch == '\'')
			{
				//token[i] = '\0';
				presym = 16;
				return 16; //字符的类别码
			}
			else
			{
				ungetc(ch, FIN);
				err('a');
				//printf("缺少单引号！");
				presym = 0;
				return 0;
			}
		}
		else
		{
			fgetc(FIN);
			err('a');
			//printf("单引号后不是字符！");
			presym = 0;
			return 0;
		}
	}
	else if (ch == '{') {
	     presym = 17;
		 return 17;//右花括号的类别码
    }
	else if (ch == '}') {
	     presym = 18;
		 return 18;
    }
	else if (ch == '(') {
	     presym = 19;
		 return 19;
    }	
	else if (ch == ')') {
		presym = 20;
		return 20;
	}
	else if (ch == '*') {
	    presym = 21;
		return 21;
    }
	else if (ch == '/') {
		presym = 22;
		return 22;
    }
	else if (ch == ',') {
		presym = 23;
		return 23;
    }
	else if (ch == ';') {
		presym = 24;
		return 24;
	}
	else if (ch == '+')
	{
		/*ch = fgetc(FIN);
		if (isDigit(ch) && presym!= 14 && presym!= 13)
		{
			while (isDigit(ch))
			{
				token[i] = ch;
				i++;
				ch = fgetc(FIN);
			}
			ungetc(ch, FIN);
			num = atoi(token);
			presym = 14;
			return 14;//正整数
		}
		else
		{
			ungetc(ch, FIN);
			presym = 25;
			return 25;
		}*/
		presym = 25;
		return 25;
	}
	else if (ch == '-')
	{
	    /*ch = fgetc(FIN);
		if (isDigit(ch) && presym != 14 && presym != 13)
		{
			while (isDigit(ch))
			{
				token[i] = ch;
				i++;
				ch = fgetc(FIN);
			}
			ungetc(ch, FIN);
			num = atoi(token);
			presym = 14;
			return 14;//负整数
		}
		else
		{
			ungetc(ch, FIN);
			presym = 26;
			return 26;
		}*/
		presym = 26;
		return 26;
	}
	else if (ch == '=')
	{
		ch = fgetc(FIN);
		if (ch == '=')
		{
			token[i] = ch;
			presym = 28;
			return 28;//==的类别码
		}
		else
		{
			ungetc(ch, FIN);
			presym = 27;
			return 27;
		}
	}
	else if (ch == '<')
	{
		ch = fgetc(FIN);
		if (ch == '=')
		{
			token[i] = ch;
			presym = 30;
			return 30;//小于等于
		}
		else
		{
			ungetc(ch, FIN);
			presym = 29;
			return 29;
		}
	}
	else if (ch == '>')
	{
		ch = fgetc(FIN);
		if (ch == '=')
		{
			token[i] = ch;
			presym = 32;
			return 32;//大于等于
		}
		else
		{
			ungetc(ch, FIN);
			presym = 31;
			return 31;
		}
	}
	else if (ch == '!')
	{
		ch = fgetc(FIN);
		if (ch == '=')
		{
			token[i] = ch;
			presym = 33;
			return 33;//不等于
		}
		else
		{
			ungetc(ch, FIN);
			err('a');
			//printf("不等于缺少等号！");
			presym = 0;
			return 0;
		}
	}
	else if (ch == '[') 
	{
		presym = 34;
		return 34;
	}
	else if (ch == ']')
	{
		presym = 35;
		return 35;
	}
	else
	{
		if (ch != EOF) {
			//printf("非法符号");
			presym = 0;
			return 0;
		}
		else {
			presym = 37;
			return 37;
		}
	}
}

void __nextSemicolon() {//能够一直读直到下一个分号
	while (getsym() != 0 && getsym() != SEMICN) {
	}
}

void __nextCommaOrSemicolon() {//能够一直读到逗号或者分号
	while ((getsym() != SEMICN &&getsym() != COMMA)) {
	}
}

void __nextClosingBraces() {//能够一直读直到下一个分号
	while (getsym() != 0 && getsym() != RBRACE) {
	}
}



void __nextReservedSymbol() {//能够一直读直到RESSYM 
	while (getsym() != 0 &&!((0<getsym()&&getsym()<13)|| getsym() == FORTK)) {
	}
}

void __nextCommaOrRightParenthesesOrSemicolon() {//能够一直读到逗号或者右括号 
	while (getsym() != 0 && getsym() != COMMA && getsym() != RPARENT && getsym() != SEMICN) {
	}
}