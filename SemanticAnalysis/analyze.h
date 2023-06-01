//
// Created by long on 2023/5/24.
//

#ifndef SNLINTERPRETER_ANALYZE_H
#define SNLINTERPRETER_ANALYZE_H

#include "../global.h"

void initialize(void);

TypeIR  * TypeProcess(TreeNode * t,DecKind deckind);

TypeIR * nameType(TreeNode * t);

TypeIR  * arrayType(TreeNode * t);

TypeIR * recordType(TreeNode * t);


void TypeDecPart(TreeNode * t);

void VarDecPart(TreeNode * t);

void varDecList(TreeNode * t);

void  procDecPart(TreeNode * t);

SymbTable  *  HeadProcess(TreeNode * t);

ParamTable  * ParaDecList(TreeNode * t) ;

void Body(TreeNode * t);

void statement (TreeNode * t);

TypeIR * Expr(TreeNode * t,AccessKind  *  Ekind);

static TypeIR * arrayVar(TreeNode * t);

static TypeIR * recordVar(TreeNode * t);

void assignstatement(TreeNode * t);

void callstatement(TreeNode * t);

void ifstatement(TreeNode * t );

void whilestatement(TreeNode * t);

void readstatement (TreeNode * t);

void writestatement(TreeNode * t );

void returnstatement(TreeNode * t );

void analyze(TreeNode * t);

#endif //SNLINTERPRETER_ANALYZE_H
