//
// Created by Lenovo on 2023/5/10.
//

#include "global.h"
#include <string>
#include <iostream>
#ifndef SNLINTERPRETER_UTIL_H
#define SNLINTERPRETER_UTIL_H

#endif //SNLINTERPRETER_UTIL_H

void printTokenlist();

void ChainToFile(ChainNodeType *p);

TreeNode * newRootNode();

TreeNode * newPheadNode();

TreeNode * newDecANode(NodeKind kind);

TreeNode * newDecNode();

TreeNode * newProcNode();

TreeNode * newStmlNode();

void ReadNextToken(TokenType *p);

