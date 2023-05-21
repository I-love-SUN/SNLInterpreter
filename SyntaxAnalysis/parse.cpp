//
// Created by long on 2023/5/13.
//

#include <bits/stdc++.h>
#include "../global.h"
#include "../util.h"
#include "parse.h"
#include "../LexicalAnalysis/scanner.h"

using std::string;

static TokenType token;

/*临时*/
string tempName;
static int line0;

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
static TreeNode* stmtList(void);

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
static TreeNode* ActParamList(void);

/*处理实参*/
static TreeNode* ActParamMore(void);

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





/*
 * 函数名：formList
 * 功能：函数声明中参数声明部分的处理函数
 * 产生式 <FormList> ::= ID FidMord
 */
void formList(TreeNode* t)
{
    if(token.Lex==ID)
    {
        strcpy(t->name[(t->idnum)],token.Sem);
        t->idnum = (t->idnum) + 1;
        match(ID);
    }
    fidMore(t);
}

/*
 * 函数名：fidMore
 * 功能：函数声明中参数声明部分的处理函数
 * 产生式 <FidMore> ::=  ε|,FormList
 */
void fidMore(TreeNode* t)
{
    switch(token.Lex)
    {
        case SEMI:
        case RPAREN:break;
        case COMMA:
            match(COMMA);
            formList(t);
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
}

/*
 * 函数名：procDecPart
 * 功能：函数声明中参数声明部分的处理函数
 * 产生式 <ProcDecPart> ::=  DeclarePart
 */
TreeNode* procDecPart(void)
{
    TreeNode* t = declarePart();
    return t;
}

/*
 * 函数名：proBody
 * 功能：过程体部分处理程序
 * 产生式 <ProBody> ::= ProgramBody
 */
TreeNode* proBody(void)
{
    TreeNode* t = programBody();
    //没有过程体
    if(t == NULL)
        syntaxError("a program body is requested!");
    return t;
}

/*
 * 函数名：programBody
 * 功能：程序体部分处理函数
 * 产生式 <ProgramBody> ::=  BEGIN stmlist END
 */
TreeNode* programBody(void)
{
    TreeNode* t = newStmlNode();
    match(BEGIN);
    if(t!= NULL)
    {
        t->lineno = 0;
        t->child[0] = stmtList();
    }
    match(END);
    return t;
}

/*
 * 函数名：stmtList
 * 功能：语句序列处理函数
 * 产生式 <StmtList> ::=  Stm StmMore
 */
TreeNode* stmtList(void)
{
    TreeNode* t = stm();
    TreeNode* p = stmMore();
    if(t!= NULL)
        if(p!= NULL)
            t->sibling = p;//使p成为t的兄弟节点
    return t;
}

/*
 * 函数名：stmMore
 * 功能：语句序列处理函数
 * 产生式 <StmMore> ::=  ε|,StmtList
 */
TreeNode* stmMore(void)
{
    TreeNode * t = NULL;
    switch(token.Lex)
    {
        case ELSE:
        case FI:
        case END:
        case ENDWH:break;
        case SEMI:
            match(SEMI);
            stmtList();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名：stm
 * 功能：语句处理函数
 * 产生式 <Stm> ::=  conditionalStm  {IF}
 *                  | loopStm         {WHILE}
 *                  | inputStm        {READ}
 *                  | outputStm       {WRITE}
 *                  | returnStm       {RETURN}
 *                  | id assCall      {id}
 */
TreeNode* stm(void)
{
    TreeNode* t = NULL;
    switch (token.Lex) {
        case IF:
            t = conditionalStm();
            break;
        case WHILE:
            t = loopStm();
            break;
        case READ:
            t = inputStm();
            break;
        case WRITE:
            t = outputStm();
            break;
        case RETURN:
            t = returnStm();
            break;
        case ID:
            tempName = copyString(token.Sem);
            match(ID);
            t = assCall();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
}

/*
 * 函数名：assCall
 * 功能：赋值语句处理函数
 * 产生式 <AssCall> ::=  assignmentRest  {:=,LMIDPAREN,DOT}
 *                     | callStmRest     {(}
 */
TreeNode* assCall(void)
{
    TreeNode* t = NULL;
    switch (token.Lex) {
        case ASSIGN:
        case LMIDPAREN:
        case DOT:
            t = assignmentRest();
            break;
        case LPAREN:
            t = callStmRest();
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名：assignmentRest
 * 功能：赋值语句处理函数
 * 产生式 <AssignmentRest> ::=  variMore := exp
 */
TreeNode* assignmentRest(void)
{
    /*创建新的赋值语句类型语法树节点t*/
    TreeNode* t = newStmtNode(AssignK);
    if(t != NULL)
    {
        t->lineno = line0;
        /*处理第一个儿子节点，为变量表达式类型节点*/
        TreeNode* child1 = newExpNode(VariK);
        if(child1!= NULL){
            child1->lineno = line0;
            strcpy(child1->name[0],tempName);
            (child1->idnum)++;
            variMore(child1);
            t->child[0] = child1;
        }

        /*赋值号匹配*/
        match(ASSIGN);
        /*处理第二个儿子节点*/
        t->child[1] = exp();
    }
    return t;
}

/*
 * 函数名:conditionalStm
 * 功能:条件语句处理函数
 * 产生式 <conditionalStm> ::= IF exp THEN stmList ELSE stmList FI
 */
TreeNode* conditionalStm(void)
{
    TreeNode* t = newStmtNode(IfK);
    match(IF);
    if(t!= NULL)
    {
        t->lineno = line0;
        t->child[0] = exp();
    }
    match(THEN);
    if(t!=NULL) t->child[1] = stmList();
    if(token.Lex == ELSE)
    {
        match(ELSE);
        if(t!=NULL) t->child[2] = stmList();
    }
    match(FI);
    return t;
}

/*
 * 函数名:loopStm
 * 功能:循环语句处理函数
 * 产生式 <loopStm> ::= WHILE exp DO stmList ENDWH
 */
TreeNode* loopStm(void)
{
    TreeNode* t = newStmtNode(WhileK);
    match(WHILE);
    if(t!= NULL)
    {
        t->lineno = line0;
        t->child[0] = exp();
        match(DO);
        t->child[1] = stmList();
        match(ENDWH);
    }
    return t;
}

/*
 * 函数名:inputStm
 * 功能:输入语句部分处理函数
 * 产生式 <inputStm> ::= READ(id)
 */
TreeNode* inputStm(void)
{
    TreeNode* t = newStmtNode(ReadK);
    match(READ);
    match(LPAREN);
    if((t!= NULL)&&(token.Lex==ID))
    {
        t->lineno = line0;
        strcpy(t->name[0],token.Sem);
        (t->idnum)++;
    }
    match(ID);
    match(RPAREN);
    return t;
}

/*
 * 函数名:outputStm
 * 功能:输出语句部分处理函数
 * 产生式 <outputStm> ::= WRITE(exp)
 */
TreeNode* outputStm(void)
{
    TreeNode* t = newStmtNode(WriteK);
    match(WRITE);
    match(LPAREN);
    if(t!= NULL)
    {
        t->lineno = line0;
        t->child[0] = exp();
    }
    match(RPAREN);
    return t;
}

/*
 * 函数名:returnStm
 * 功能:返回语句部分处理函数
 * 产生式 <returnStm> ::= RETURN
 */
TreeNode* returnStm(void)
{
    TreeNode* t = newStmtNode(ReturnK);
    match(RETURN);
    if(t!= NULL)
        t->lineno = line0;
    return t;
}

/*
 * 函数名:callStmRest
 * 功能:过程调用语句部分处理函数
 * 产生式 <CallStmRest> ::= (ActParamList)
 */
TreeNode* callStmRest(void)
{
    TreeNode* t = newStmtNode(CallK);
    match(LPAREN);
    /*函数调用时，其子节点指向实参*/
    if(t!= NULL)
    {
        t->lineno = line0;
        TreeNode* child0 = newNode(VariK);
        if(child0!= NULL)
        {
            child0->lineno = line0;
            strcpy(child0->name[0],tempName);
            (child0->idnum)++;
            t->child[0] = child0;
        }
        t->child[1] = actParamList();
    }
    match(RPAREN);
    return t;
}

/*
 * 函数名:actParamList
 * 功能:函数调用实参部分处理函数
 * 产生式 <ActParamList> ::=  ε| exp actParamMore
 */
TreeNode* actParamList(void)
{
    TreeNode* t = NULL;
    switch (token.Lex) {
        case RPAREN:
            break;
        case ID:
        case INTC:
            t = exp();
            if(t!=NULL)
                t->sibling = actParamMore();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名:actParamMore
 * 功能:函数调用实参部分处理函数
 * 产生式 <ActParamMore> ::= ε|, actParamList
 */
TreeNode* actParamMore(void)
{
    TreeNode* t = NULL;
    switch (token.Lex) {
        case RPAREN:break;
        case COMMA:
            match(COMMA);
            t = actParamList();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名:exp
 * 功能:表达式处理函数
 * 产生式 <Exp> ::= simple_exp | RelOp simple_exp
 */
TreeNode* exp(void)
{
    TreeNode* t = simple_exp();
    if((token.Lex)==LT||(token.Lex==EQ))
    {
        TreeNode* p = newExpNode(OpK);
        if(p!=NULL)
        {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = token.Lex;
            t = p;
        }
        match(token.Lex);
        if(t!=NULL)
            t->child[1] = simple_exp();
    }
    return t;
}

/*
 * 函数名:simple_exp
 * 功能:简单表达式处理函数
 * 产生式 <SimpleExp> ::=  term | PlusOp term
 */
TreeNode* simple_exp(void)
{
    TreeNode* t = term();
    while((token.Lex)==PLUS||(token.Lex==MINUS))
    {
        TreeNode* p = newExpNode(OpK);
        if(p!=NULL)
        {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = token.Lex;
            t = p;
        }
        match(token.Lex);
        if(t!=NULL)
            t->child[1] = term();
    }
    return t;
}

/*
 * 函数名 term
 * 功能 项处理函数
 * < term > ::= factor | TimesOp factor
 */
TreeNode* term(void)
{
    TreeNode *t = factor();

    while((token.Lex==TIMES)||(token.Lex==OVER))
    {
        TreeNode *p = newExpNode(Opk);

        if(p!=NULL)
        {
            p->lineno = line0;
            p->child[0] = t;
            p->attr.ExpAttr.op = token.Lex;
            t = p;
        }

        match(token.Lex);
        p->child[1] = factor();
    }
    return t;
}

/*
 * 函数名 factor
 * 功能:因子处理函数
 * < factor > ::= ( exp ) | INTC | variable
 */
TreeNode *factor(void)
{
    TreeNode *t = NULL;
    switch(token.Lex){
        case  INTC:
            t = newExpNode(ConstK);

            if(t!=NULL&&token.Lex==INTC){
                t->lineno = line0;
                t->attr.ExpAttr = atoi(token.Sem);
            }

            match(INTC);
            break;

        case ID:
            t = variable();
            break;

        case LPAREN:
            match(LPAREN);
            t = exp();
            match(RPAREN);
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名 variable
 * 功能:变量处理函数
 * 产生式 <variable> ::= ID variMore
 */
TreeNode* variable(void)
{
    TreeNode* t = newExpNode(VariK);
    if(t!=NULL&&token.Lex==ID)
    {
        t->lineno = line0;
        strcpy(t->name[0],token.Sem);
        (t->idnum)++;
    }
    match(ID);
    variMore(t);
    return t;
}

/*
 * 函数名 variMore
 * 功能:变量处理函数
 * 产生式 <variMore> ::= ε| [Exp]        {[}
 *                       | .fieldvar    {DOT}
 */
void variMore(TreeNode *t)
{
    switch (token.Lex) {
        case ASSIGN:
        case TIMES:
        case EQ:
        case LT:
        case PLUS:
        case MINUS:
        case OVER:
        case RPAREN:
        case RMIDPAREN:
        case SEMI:
        case COMMA:
        case THEN:
        case ELSE:
        case FI:
        case DO:
        case ENDWH:
        case END:
            break;
        case LBRACKET:
            match(LBRACKET);
            if(t!=NULL)
            {
                t->child[0] = exp();
                t->attr.ExpAttr.varkind = ArrayMembV;
                t->child[0]->attr.ExpAttr.varkind = IdV;
                match(RBRACKET);
            }
            break;
        case DOT:
            match(DOT);
            if(t!=NULL)
            {
                t->child[0] = fieldVar();
                t->attr.ExpAttr.varkind = FieldMembV;
                t->child[0]->attr.ExpAttr.varkind = IdV;
            }
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
}

/*
 * 函数名 fieldVar
 * 功能:字段变量处理函数
 * 产生式 <fieldvar> ::= id fieldvarMore
 */
TreeNode* fieldVar(void)
{
    TreeNode* t = newExpNode(VariK);
    if(t!=NULL&&token.Lex==ID)
    {
        t->lineno = line0;
        strcpy(t->name[0],token.Sem);
        (t->idnum)++;
    }
    match(ID);
    fieldvarMore(t);
    return t;
}

/*
 * 函数名 fieldvarMore
 * 功能:字段变量处理函数
 * 产生式 <fieldvarMore> ::= ε| [Exp]        {[}
 */
void fieldVarMore(TreeNode *t)
{
    switch(token.Lex)
    {
        case ASSIGN:
        case TIMES:
        case EQ:
        case LT:
        case PLUS:
        case MINUS:
        case OVER:
        case RPAREN:
        case SEMI:
        case COMMA:
        case THEN:
        case ELSE:
        case FI:
        case DO:
        case ENDWH:
        case END:
            break;
        case LMIDPAREN:
            match(LMIDPAREN);
            if(t!=NULL)
            {
                t->child[0] = exp();
                t->child[0]->attr.ExpAttr.varkind = IdV;
                match(RMIDPAREN);
            }
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
}





/*
 * 语法分析主函数
 * 说明：该函数把语法分析程序作为子程序调用，采用递归下降法，根据产生式调用递归处理函数
 *      为源程序创建语法分析树
 */
TreeNode* parse(void)
{
    TreeNode* t = NULL;

    /*从文件Tokenlist中取得第一个单词*/
    ReadNextToken(&token);
    /*调用语法分析处理函数，递归下降*/
    t = program();

    /*当前单词token不是ENDFILE，报代码在文件结束前提前结束的错误*/
    if (token.Lex!= ENDFILE)
    {
        syntaxError("Code ends before file\n");
    }
    return t;
}







