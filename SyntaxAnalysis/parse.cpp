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


/*函数名：match
 * 功能：终极符匹配处理函数
 * 说明：函数参数expected给定期望单词符号与当前单词符号token相匹配
 * 如果不匹配，则报错
 */
static void match(LexType expected) {
    if(expected == token.Lex){
        ReadNextToken(&token);
        line0 = token.Lineshow;
    }else{
        syntaxError("not match error ");
        fprintf(listing,"'%s'\n",token.Sem);
        ReadNextToken(&token);
        exit(0);
    }
}


/*函数名：syntaxError
 * 功能：语法错误处理函数
 * 说明：将函数参数message指定的错误信息格式化写入列表文件listing
 *      设置错误追踪表示Error为TRUE
 * */
static void syntaxError(string message) {
    fprintf(listing,"\n>>> error: ");
    fprintf(listing,"Syntax Error at line %d: %s\n", token.Lineshow, message.c_str());
    Error = TRUE;
}

/*
 * 函数名：formList
 * 功能：函数声明中参数声明部分的处理函数
 * 产生式 <FormList> ::= ID FidMord
 */
void formList(TreeNode* t)
{
    if(token.Lex==ID)
    {
        t->name[(t->idnum)]=token.Sem;
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

/*函数中声明部分处理函数
 * 产生式 < procDecPart > ::=  declarePart
 * */
TreeNode  *procBody() {
    TreeNode * t = programBody();
    if(t==NULL){
        syntaxError("a program body is required!");
    }
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
        t->child[0] = stmList();
    }
    match(END);
    return t;
}

/*
 * 函数名：stmtList
 * 功能：语句序列处理函数
 * 产生式 <StmtList> ::=  Stm StmMore
 */
TreeNode* stmList(void)
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
 * 产生式 <StmMore> ::=  ε|,StmList
 */
TreeNode* stmMore(void) {
    TreeNode *t = NULL;
    switch (token.Lex) {
        case ELSE:
        case FI:
        case END:
        case ENDWH:
            break;
        case SEMI:
            match(SEMI);
            t=stmList();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}

/*
 * 函数名：program
 * 功能：总程序的处理函数
 * 产生式< program > ::= programHead declarePart programBody.
 * 说明：根据递归下降的文法，调用相应的递归处理函数，生成语法树节点，语法树
 *      的根节点的第一个子节点指向程序头部分programHead,DeclaraPart
 *      为programHead的兄弟节点，程序体部分programBody为declarePart
 *      的兄弟节点。
 * */
TreeNode *program(){
    TreeNode * t = programHead();
    TreeNode * q = declarePart();
    TreeNode * s = programBody();

    TreeNode * root = newRootNode();
    if(root!=NULL)
    {
        root->lineno = 0;
        if(t!=NULL)
            root->child[0]=t;
        else
            syntaxError("a program head is expected !");

        if(q!=NULL) root->child[1]=q;
        if(s!=NULL) root->child[2]=s;
        else syntaxError("a program body is expected !");
    }

    match(DOT);
    return root;
}

/*函数名：programHead
 * 功能：程序头的处理函数
 * 产生式：<programHead> ::= PROGRAM ProgramName
 * 说明：递归调用处理函数，生成语法树结点
 * */
TreeNode * programHead(){
    TreeNode * t = newPheadNode();
    match(PROGRAM);
    if((t!=NULL)&&(token.Lex==ID)){
        t->lineno = 0;
        t->name[0] = token.Sem;
    }
    match(ID);
    return t;
}

/*----------------------------声明部分-------------------------*/
/*声明部分的处理函数
 *产生式 <declarePart> ::= typeDec  varDec  procDec
 * 说明：创建新的类型声明标志节点、变量声明标志节点。调用类型声明处理函数typeDec()
 * (其返回指针tp1位类型声明标志节点的第一个子节点)、变量声明处理函数VarDec()(其返回
 * 值tp2为变量声明标志节点的第一个子节点)、函数声明处理函数procDec()随后使变量声明标
 * 志节点为类型声明标志节点的兄弟节点，函数声明节点为变量声明节点的兄弟节点。如果处理成功，
 * 则函数返回指向变量声明标志节点。否则返回null
 * */
TreeNode * declarePart(){
    //类型
    /*创建类型声明标志节点*/
    TreeNode *typeP = newDecANode(TypeK);
    TreeNode * pp = typeP;
    if(typeP!=NULL){
        typeP->lineno = 0;
        /*调用类型声明处理函数*/
        TreeNode * tp1 = typeDec();
        if(tp1!=NULL)
            typeP->child[0] = tp1;
        else{
            delete typeP;
            typeP = NULL;
        }
    }
    //变量
    /*创建变量声明标志节点*/
    TreeNode * varP = newDecANode(VarK);
    if(varP!=NULL)
    {
        varP->lineno = 0;
        /*变量声明处理*/
        TreeNode * tp2 = varDec();
        if(tp2!=NULL)
            varP->child[0] = tp2;
        else{
            delete varP;
            varP = NULL;
        }
    }
    //函数
    /*创建函数声明节点*/
    TreeNode * s = procDec();
    if(s==NULL){}
    if(varP == NULL){varP = s;}
    if(typeP == NULL){
        pp = typeP = varP;
    }
    if(typeP != varP){
        typeP->sibling = varP;
        typeP = varP;
    }
    if(varP!=s){
        varP->sibling = s;
        varP = s;
    }
    return pp;
}

/*功能：类型声明部分处理函数
 * 产生式：< typeDec > ::= ε | TypeDeclaration
 * 说明：生成相应的语法树节点
 * */
TreeNode * typeDec(){
    TreeNode * t = NULL;
    switch(token.Lex)
    {
        case TYPE:
            t = typeDeclaration();
            break;
        case VAR:
        case PROCEDURE:
        case BEGIN:
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!\n");
            break;
    }
    return t;
}

/*功能：类型声明部分处理函数
 * 产生式 < TypeDeclaration > ::= TYPE  TypeDecList
 * 说明：函数根据文法产生式,调用相应的递归处理函数,生成语法树节点
 * */
TreeNode * typeDeclaration(){
    match(TYPE);
    TreeNode * t = typeDecList();
    if(t==NULL){
        syntaxError("a type declaration is expected");
    }
    return t;
}

/*功能：类型声明部分处理函数
 * 产生式 < TypeDecList > ::= typeId = typeName ; typeDecMore
 * 说明：生成语法树节点
 * */
TreeNode * typeDecList(){
    TreeNode * t = newDecNode();
    if (t!=NULL){
        t->lineno = line0;
        typeId(t);
        match(EQ);
        typeName(t);
        match(SEMI);
        TreeNode * p = typeDecMore();
        if(p!=NULL)
            t->sibling = p;
    }
    return t;
}

/*功能：类型声明部分处理函数
 * 产生式 < typeDecMore > ::= ε | TypeDecList
 * */
TreeNode * typeDecMore(){
    TreeNode *t = NULL;
    switch(token.Lex)
    {
        case VAR:
        case PROCEDURE:
        case BEGIN:
            break;
        case ID:
            t = typeDecList();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
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
TreeNode* stm(void) {
    TreeNode *t = NULL;
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
            tempName = token.Sem;
            match(ID);
            t = assCall();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}


/*功能：类型声明部分处理函数
 * 产生式 < typeId > ::= id
 * */
void typeId(TreeNode *t){
    int num = t->idnum;
    if((token.Lex==ID)&&(t!=NULL))
    {
        t->name[num]= token.Sem;
        num++;
    }
    t->idnum = num;
    match(ID);
}

/*功能：类型声明部分处理函数
 * 产生式 < typeName > ::= baseType | structureType | id
 * */
void typeName(TreeNode* t){
    if(t!=NULL){
        switch(token.Lex)
        {
            case INTEGER:
            case CHAR:
                baseType(t);    //类型为INTEGER和CHAR时，调用基类型处理的函数
                break;
            case ARRAY:
            case RECORD:
                structureType(t);   //类型为数组或结构体型时，调用结构体/数组的处理函数
                break;
            case ID:
                t->kind.dec = IdK;
                t->attr.typeName = token.Sem;
                match(ID);      //是标识符时，直接处理
                break;
            default:
                ReadNextToken(&token);
                syntaxError("unexpected token is here");
                break;
        }
    }
}

/*类型声明部分处理函数
 * 产生式 < baseType > ::=  INTEGER | CHAR
 * */
void baseType(TreeNode* t){
    switch (token.Lex) {
        case INTEGER:    //匹配到整数类型
            match(INTEGER);
            t->kind.dec = IntegerK;
            break;
        case CHAR: //
            match(CHAR);
            t->kind.dec = CharK;
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
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
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/*功能：类型声明部分处理函数
 * 产生式 < structureType > ::=  arrayType | recType
 * */
void structureType(TreeNode *t){
    switch(token.Lex)
    {
        case ARRAY:
            arrayType(t);
            break;
        case RECORD:
            t->kind.dec = RecordK;
            recType(t);
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
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
            child1->name[0]=tempName;
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
        t->name[0]=token.Sem;
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
        TreeNode* child0 = newExpNode(VariK);
        if(child0!= NULL)
        {
            child0->lineno = line0;
            child0->name[0]=tempName;
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
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/*功能：类型声明部分处理函数
 *  产生式 < arrayType > ::=  ARRAY [low..top] OF baseType
 * */
void arrayType(TreeNode* t){
    //匹配ARRAY
    match(ARRAY);
    //匹配左括号[
    match(LMIDPAREN);
    if(token.Lex==INTC)
    {
        t->attr.ArrayAttr.low = atoi(token.Sem);
    }
    //匹配数组上界
    match(INTC);
    match(UNDERANGE);
    if(token.Lex==INTC){
        t->attr.ArrayAttr.up = atoi(token.Sem);
    }
    //匹配数组下界
    match(INTC);
    match(RMIDPAREN);
    match(OF);
    baseType(t);
    t->attr.ArrayAttr.childtype=t->kind.dec;
    t->kind.dec = ArrayK;
}

/*类型声明部分处理函数
 * 产生式 < recType > ::=  RECORD fieldDecList END
 * */
void recType(TreeNode * t){
    TreeNode *p = NULL;
    match(RECORD);
    p=fieldDecList();
    if(p!=NULL)
        t->child[0]=p;
    else
        syntaxError("a record body is required!");
    match(END);
}

/*功能：类型声明部分处理函数
 * 产生式 < fieldDecList > ::=   baseType idList ; fieldDecMore
 *                              | arrayType idList; fieldDecMore
 * */
TreeNode * fieldDecList(){
    TreeNode * t = newDecNode();
    TreeNode * p = NULL;
    if(t!=NULL){
        t->lineno = line0;
        switch (token.Lex)
        {
            case INTEGER:
            case CHAR:
                baseType(t);
                idList(t);
                match(SEMI);
                p=fieldDecMore();
                break;
            case ARRAY:
                arrayType(t);
                idList(t);
                match(SEMI);
                p=fieldDecMore();
                break;
            default:
                ReadNextToken(&token);
                syntaxError("unexpected token is here！");
                break;
        }
        t->sibling=p;
    }
    return t;
}

/*功能：类型声明部分处理函数
 * 产生式 < fieldDecMore > ::=  ε | fieldDecList
 * */
TreeNode * fieldDecMore(){
    TreeNode * t = NULL;
    switch(token.Lex)
    {
        case END: break;
        case INTEGER:
        case CHAR :
        case ARRAY :
            t = fieldDecList();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
    return t;
}
/*-----------------------------变量声明部分------------------------------*/

/*类型声明部分的处理函数
 * 产生式 < idList > ::=  id  idMore
 * */
void idList(TreeNode* t){
    if(token.Lex == ID){
        t->name[t->idnum] = token.Sem;
        match(ID);
        t->idnum++;
    }
    idMore(t);
}

/*功能：类型声明部分处理函数
 * 产生式 < idMore > ::=  ε |  , idList
 * */
void idMore(TreeNode * t){
    switch(token.Lex)
    {
        case SEMI: break;
        case COMMA:
            match(COMMA);
            idList(t);
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
}

/*功能：变量声明部分处理函数
 * 产生式 < varDec > ::=  ε |  varDeclaration
 * */
TreeNode * varDec(){
    TreeNode * t = NULL;
    switch(token.Lex)
    {
        case PROCEDURE:
        case BEGIN:
            break;
        case VAR:
            t=varDeclaration();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/*功能：变量声明部分函数处理
 *产生式 < varDeclaration > ::=  VAR  varDecList
 * */
TreeNode * varDeclaration(){
    match(VAR);
    TreeNode *t = varDecList();
    if(t==NULL)
        syntaxError("a var declaration is required");
    return t;
}

/*功能：变量声明部分处理函数
 * 产生式 < varDecList > ::=  typeName varIdList; varDecMore
 * */
TreeNode * varDecList(){
    TreeNode *t = newDecNode();
    TreeNode * p = NULL;

    if(t!=NULL){
        t->lineno = line0;
        typeName(t);
        varIdList(t);
        match(SEMI);
        p = varDecMore();
        t->sibling = p;
    }
    return t;
}

/*功能：变量声明部分处理函数
 * 产生式 < varDecMore > ::=  ε |  varDecList
 * */
TreeNode * varDecMore(){
    TreeNode * t = NULL;
    switch (token.Lex) {
        case PROCEDURE:
        case BEGIN:
            break;
        case INTEGER:
        case CHAR:
        case RECORD:
        case ARRAY:
        case ID:
            t = varDecList();
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
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
}

/*变量声明部分处理函数
 * 产生式 < varIdList > ::=  id  varIdMore
 * */
void varIdList(TreeNode* t) {
    if(token.Lex == ID){
        t->name[t->idnum]=token.Sem;
        match(ID);
        t->idnum++;
    }else{
        syntaxError("a varid is required here");
        ReadNextToken(&token);
    }
    varIdMore(t);
}

/*变量声明部分处理函数
 *  产生式 < varIdMore > ::=  ε |  , varIdList
 * */
void varIdMore(TreeNode* t){
    switch (token.Lex){
        case SEMI:
            break;
        case COMMA:
            match(COMMA);
            varIdList(t);
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
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
    while((token.Lex==PLUS)||(token.Lex==MINUS))
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
        TreeNode *p = newExpNode(OpK);

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
    switch(token.Lex) {
        case INTC:
            t = newExpNode(ConstK);

            if (t != NULL && token.Lex == INTC) {
                t->lineno = line0;
                t->attr.ExpAttr.val = atoi(token.Sem);
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

/*-------------------------过程声明部分----------------------------*/

/*功能：过程声明部分处理函数
 * 产生式 < procDec > ::=  ε |  procDeclaration
 * */
TreeNode * procDec(){
    TreeNode * t = NULL;
    switch (token.Lex){
        case BEGIN:break;
        case PROCEDURE:
            t=procDeclaration();
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
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
        t->name[0]=token.Sem;
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
        case LMIDPAREN:
            match(LMIDPAREN);
            t->child[0] = exp();
            t->attr.ExpAttr.varkind = ArrayMembV;
            t->child[0]->attr.ExpAttr.varkind = IdV;
            match(RMIDPAREN);
            break;
        case DOT:
            match(DOT);
            t->child[0] = fieldVar();
            t->attr.ExpAttr.varkind = FieldMembV;
            t->child[0]->attr.ExpAttr.varkind = IdV;
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here");
            break;
    }
}
/*函数声明部分处理函数
* 产生式 < procDeclaration > ::=  PROCEDURE
*                                ProcName(paramList);
*                                procDecPart
*                                procBody
*                                procDec
* 说  明 函数根据文法产生式,调用相应的递归处理函数,生成语法树节点函数的
        根节点用于记录该函数的名字；第一个子节点指向参数节点，第二个节点
        指向函数中的声明部分节点；第三个节点指向函数体。
 * */
TreeNode * procDeclaration(){
    TreeNode * t = newProcNode();
    match(PROCEDURE);
    if(t!=NULL){
        t->lineno = line0;
        if(token.Lex == ID){
            t->name[0]=token.Sem;
            t->idnum++;
            match(ID);
        }
        match(LPAREN);
        paramList(t);
        match(RPAREN);
        match(SEMI);
        t->child[1]=procDecPart();
        t->child[2] = procBody();
        t->sibling = procDec();
    }
    return t;
}

/*函数声明中参数什么部分处理函数
 * 产生式 < paramList > ::=  ε |  paramDecList
 * */
void paramList(TreeNode* t) {
    TreeNode * p =NULL;
    switch (token.Lex) {
        case RPAREN:
            break;
        case INTEGER:
        case CHAR:
        case ARRAY:
        case RECORD:
        case ID:
        case VAR:
            p=paramDecList();
            t->child[0]=p;
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
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
        t->name[0]=token.Sem;
        (t->idnum)++;
    }
    match(ID);
    fieldVarMore(t);
    return t;
}

/*
 * 函数名 fieldvarMore
 * 功能:字段变量处理函数
 * 产生式 <fieldvarMore> ::= ε| [Exp]        {[}
 */
void fieldVarMore(TreeNode *t)
{
    switch(token.Lex) {
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
            if (t != NULL) {
                t->child[0] = exp();
                t->child[0]->attr.ExpAttr.varkind = ArrayMembV;
                match(RMIDPAREN);
            }
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
}
/*功能：函数声明中参数说明部分处理函数
 * 产生式 < paramDecList > ::=  param  paramMore
 * */
TreeNode * paramDecList(){
    TreeNode * t = param();
    TreeNode * p = paramMore();
    if(p!=NULL){
        t->sibling=p;
    }
    return t;
}

/*功能：函数声明中参数说明部分处理函数
 * 产生式 < paramMore > ::=  ε | ; paramDecList
 * */
TreeNode * paramMore(){
    TreeNode * t = NULL;
    switch (token.Lex) {
        case RPAREN:
            break;
        case SEMI:
            match(SEMI);
            t=paramDecList();
            if(t==NULL)
                syntaxError("a param declaration is required!");
            break;
        default:
            ReadNextToken(&token);
            syntaxError("unexpected token is here!");
            break;
    }
    return t;
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

/*功能：函数声明中参数说明部分处理函数
 * 产生式 < param > ::=  typeName formList | VAR typeName formList
 * */
TreeNode * param(){
    TreeNode * t = newDecNode();
    if(t!=NULL){
        t->lineno=lineno;
        switch (token.Lex) {
            case INTEGER:
            case CHAR:
            case ARRAY:
            case RECORD:
            case ID:
                t->attr.ProcAttr.paramt=valparamType;
                typeName(t);
                formList(t);
                break;
            case VAR:
                match(VAR);
                t->attr.ProcAttr.paramt=varparamType;
                typeName(t);
                formList(t);
                break;
            default:
                ReadNextToken(&token);
                syntaxError("unexpected token is here!");
                break;
        }
    }
    return t;
}






