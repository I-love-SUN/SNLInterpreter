

#ifndef SNLINTERPRETER_SYMBTABLE_H
#define SNLINTERPRETER_SYMBTABLE_H

#include "../global.h"

#include "stdio.h"

#include "string.h"

#include "../util.h"

SymbTable * NewTable(void);
void CreateTable(SymbTable * table);
void DestroyTable(void);

int Enter(string id,AttributeIR *attribP,SymbTable ** entry);

int Find(string id,SymbTable ** entry);

AttributeIR FindAttr(SymbTable *entry);

int Compat(TypeIR *tp1,TypeIR *tp2);

typeIR *NewTy(TypeKind kind);

fieldChain *NewBody(void);

ParamTable *NewParams(void);

void ErrorPrompt(int line,string name,string message);

/*打印空格*/
void printTab(int tabnum);

/*输出符号表*/
void printSymbTable();

/*输出符号表的一层*/
void printOneLay(int level);

void   PrintFieldChain(fieldChain  *currentP);

bool  FindField( Id , fieldChain  *head,fieldChain  **Entry );

#endif //SNLINTERPRETER_SYMBTABLE_H

