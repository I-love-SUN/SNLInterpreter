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

/* 静态变量indentno在函数printTree中	*
 * 用于存储当前子树缩进格数,初始为0		*/
static int indentno = 0;

/** 增量/减量缩进宏定义 **/
#define INDENT indentno+=4
#define UNINDENT indentno-=4

void  printTree(TreeNode  *tree);