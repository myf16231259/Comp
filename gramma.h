#pragma once
#ifndef GRAMMA_H
#define GRAMMA_H

#include <stdio.h>
#include <string.h>
#include "getsym.h"
#include "ICgen.h"

void stillvar();
int getnext();
void constdeclaration();
void constdefine(int x);
void funcall(char* id,int len,int* fac_type);
void dostatement(int* hasretst);
int expression(int* exp_type, int* exp_const, int* exp_value,int* counter);
int factor(int* exp_type, int* exp_const, int* exp_value, int* counter);
int term(int* exp_type, int* exp_const, int* exp_value, int* counter);
void ifstatement(int* hasretst);
void forstatement(int* hasretst);
void mainfunc();
void printfstatement();
void returnfuncdefine();
void returnstatement(int* hasretst);
void statementarray(int *hasretst);
void scanfstatement();
void statement(int* hasretst);
void voidfuncdefine();
void vardefine(char* symname, int symlen, int symtype,int ifarr);
void program();
void stepadd(char* temp, int templen, int opr, int steplen);


#endif