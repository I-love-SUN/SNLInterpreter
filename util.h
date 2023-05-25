//
// Created by Lenovo on 2023/5/10.
//

#include "global.h"
#include <string>
#include <iostream>
#ifndef SNLINTERPRETER_UTIL_H
#define SNLINTERPRETER_UTIL_H

void printTokenlist();

void ChainToFile(ChainNodeType *p);


#endif //SNLINTERPRETER_UTIL_H

TreeNode * newRootNode();

TreeNode * newPheadNode();

TreeNode * newDecANode(NodeKind kind);

TreeNode * newDecNode();

TreeNode * newProcNode();

TreeNode * newStmlNode();

void ReadNextToken(TokenType *p);

TreeNode * newStmtNode(StmtKind kind);

TreeNode * newExpNode(ExpKind kind);


void  printTree(TreeNode  *tree);

/*打印空格*/
void printTab(int tabnum);





/*****************中间代码**************/

/*创建一个新的临时变量*/
ArgRecord *NewTemp(AccessKind access);

/*创建新的标号*/
int NewLabel();

/*地址的ARG结构的创建*/
ArgRecord  *ARGAddr(string id ,int level, int off, AccessKind access);

/*标号的ARG结构的创建*/
ArgRecord *ARGLabel(int  label );

/*常量值的ARG结构的创建*/
ArgRecord  *ARGValue(int value);



/*打印四元式中间代码序列*/
void  PrintMidCode(CodeFile  *firstCode);

/*打印一条中间代码*/
void PrintOneCode(CodeFile *code);

/*打印中间代码的类别*/
void PrintCodeName(CodeKind kind);

/*打印ARG结构的内容*/
void PrintContent(ArgRecord  *arg);