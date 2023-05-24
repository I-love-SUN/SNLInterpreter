//
// Created by long on 2023/5/13.
//

#ifndef SNLINTERPRETER_PARSE_H
#define SNLINTERPRETER_PARSE_H

#include "../global.h"

TreeNode * parse();
/*递归调用的函数原型*/


/*处理参数声明 t是要生成的当前语法树的节点指针*/
static void formList(TreeNode* t);

/*处理参数声明 t是要生成的当前语法树的节点指针*/
static void fidMore(TreeNode* t);

/*处理函数中的声明*/
static TreeNode* procDecPart(void);

/*处理函数体*/
static TreeNode* proBody(void);

/*处理程序体*/
static TreeNode* programBody(void);

/*处理语句序列*/
static TreeNode* stmList(void);

/*处理语句序列*/
static TreeNode* stmMore(void);

/*处理语句*/
static TreeNode* stm(void);

/*处理赋值和调用*/
static TreeNode* assCall(void);

/*处理赋值*/
static TreeNode* assignmentRest(void);

/*处理条件语句*/
static TreeNode* conditionalStm(void);

/*处理循环语句*/
static TreeNode* loopStm(void);

/*处理输入语句*/
static TreeNode* inputStm(void);

/*处理输出语句*/
static TreeNode* outputStm(void);

/*处理返回语句*/
static TreeNode* returnStm(void);

/*处理调用语句*/
static TreeNode* callStmRest(void);

/*处理实参*/
static TreeNode* actParamList(void);

/*处理实参*/
static TreeNode* actParamMore(void);

/*处理表达式*/
static TreeNode* exp(void);

/*处理简单表达式函数*/
static TreeNode* simple_exp(void);

/*处理项*/
static TreeNode* term(void);

/*处理因子*/
static TreeNode* factor(void);

/*处理变量*/
static TreeNode* variable(void);

/*处理变量*/
static void variMore(TreeNode* t);

/*处理域变量*/
static TreeNode* fieldVar(void);

/*处理域变量*/
static void fieldVarMore(TreeNode* t);

/*处理总程序*/
static TreeNode *program();
/*处理程序头*/
static TreeNode *programHead();
/*处理声明部分*/
static TreeNode *declarePart();
/*处理类型声明*/
static TreeNode *typeDec();
/*处理类型声明*/
static TreeNode *typeDeclaration();
/*处理类型声明*/
static TreeNode *typeDecList();
/*处理类型声明*/
static TreeNode *typeDecMore();
/*处理类型标识符*/
static void typeId(TreeNode* t);
/*处理类型名*/
static void typeName(TreeNode* t);
/*处理基本类型*/
static void baseType(TreeNode* t);
/*处理结构类型*/
static void structureType(TreeNode* t);
/*处理数组类型*/
static void arrayType(TreeNode* t);
/*处理记录类型*/
static void recType(TreeNode* t);
/*处理域*/
static TreeNode *fieldDecList();
/*处理域*/
static TreeNode *fieldDecMore();
/*处理标识符*/
static void idList(TreeNode *t);
/*处理标识符*/
static void idMore(TreeNode *t);
/*处理变量声明*/
static TreeNode *varDec();
/*处理变量声明*/
static TreeNode *varDeclaration();
/*处理变量声明*/
static TreeNode *varDecList();
/*处理变量声明*/
static TreeNode *varDecMore();
/*处理标识符*/
static void varIdList(TreeNode *t);
/*处理标识符*/
static void varIdMore(TreeNode *t);
/*处理函数声明*/
static TreeNode *procDec();
/*处理函数声明*/
static TreeNode *procDeclaration();
/*处理参数声明*/
static void paramList(TreeNode *t);
/*处理参数声明*/
static TreeNode *paramDecList();
/*处理参数声明*/
static TreeNode *param();
/*处理参数声明*/
static TreeNode * paramMore();
/*处理函数体*/
static TreeNode *programBody();
/*语法错误处理函数*/
static void syntaxError(string message);


static TreeNode *procDecPart();

static TreeNode *procBody();

#endif //SNLINTERPRETER_PARSE_H
