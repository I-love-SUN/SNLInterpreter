//
// Created by long on 2023/5/12.
//

#include "util.h"


//extern FILE *fp;

std::string path = "../outFile/";
std::string filename = "Tokenlist.txt";
int fp_num = 0; //输出时记录token的数量

/*
 * 函数名：printTokenlist()
 * 功能：将文件tokenlist中的信息作为返回值输出
 * 说明：用于显示词法分析的结果
 * */
void printTokenlist(){
    TokenType token;

    fp = fopen((path+filename).c_str(), "rb");
    if(fp == NULL)
    {
        std::cout<<"Can't open the file "+filename+"!\n";
        Error = TRUE;
    }
    for (int i = 1; i <= Tokennum; ++i) {
        fread(&token,TOKENLEN,1,fp);
        fprintf(listing,"\t%d: ",token.Lineshow);//打印行号

        /*对函数参数Lex给定单词进行分类处理*/
        switch (token.Lex)
        {
            /* 单词为保留字,将保留字词元写入列表文件listing */
            case PROGRAM:
            case PROCEDURE:
            case TYPE:
            case VAR:
            case IF:
            case THEN:
            case ELSE:
            case FI:
            case INTEGER:
            case CHAR:
            case WHILE:
            case DO:
            case ENDWH:
            case BEGIN:
            case END:
            case READ:
            case WRITE:
            case ARRAY:
            case OF:
            case RECORD:
            case RETURN:
                fprintf(listing,"reserved word: %s\n",token.Sem);
                break;

            case ASSIGN: fprintf(listing,":=\n"); break;

            case LT: fprintf(listing,"<\n"); break;

            case EQ: fprintf(listing,"=\n"); break;

            case LPAREN: fprintf(listing,"(\n"); break;

            case RPAREN: fprintf(listing,")\n"); break;

            case SEMI: fprintf(listing,";\n"); break;

            case PLUS: fprintf(listing,"+\n"); break;

            case MINUS: fprintf(listing,"-\n"); break;

            case TIMES: fprintf(listing,"*\n"); break;

            case OVER: fprintf(listing,"/\n");  break;

            case DOT:  fprintf(listing,".\n"); break;

            case COMMA: fprintf(listing,",\n"); break;

            case LMIDPAREN: fprintf(listing,"[\n"); break;

            case RMIDPAREN:
                fprintf(listing,"]\n");
                break;

            case UNDERANGE:
                fprintf(listing,"..\n");
                break;

            case ENDFILE:
                fprintf(listing,"EOF\n");
                break;

            case INTC:
                fprintf(listing,"NUM, val = %s\n",token.Sem);
                break;

            case CHARC:
                fprintf(listing,"INCHAR, char = %c\n",token.Sem);
                break;

            case ID:
                fprintf(listing,"ID, name = %s\n",token.Sem);
                break;

            case ERROR:
                fprintf(listing,"Error: %s\n",token.Sem);
                break;

            default:
                fprintf(listing,"Unknown token: %d\n",token.Lex);
                break;
        }
        fseek(fp,i*TOKENLEN,0);
    }
    fprintf(listing,"\n");
    fclose(fp);
}


/*函数名：ChainToFile(ChainNodeType *p)
 * 功能：将链表中的Token以此写入文件中
 * 说明：参数Chainhead是Token链表的表头 */
void ChainToFile(ChainNodeType *Chainhead){
    int num = 1;
    ChainNodeType  *cur = Chainhead;

    fp = fopen((path+filename).c_str(),"wb+");
    if(!fp)
    {
        std::cout<<"Cannot create file "+filename+"!\n";
        Error = TRUE;
    }
    fp = fopen((path+filename).c_str(),"ab");
    if(!fp)
    {
        std::cout<<"Cannot open file "+filename+"!\n";
        Error = TRUE;
    }
    /*遍历链表，将所有的token写入文件*/
    while (cur){
        fwrite(cur,TOKENLEN,1,fp);
        cur = cur->nextToken;
        num++;
    }
    fclose(fp);
}
/*功能：将文件tokenlist中的信息作为返回值，listing指向标准输出
 * 说明：返回值为TokenType类型，用于语法分析
 * */
void ReadNextToken(TokenType *p){
    FILE *fp2;
    fp2=fopen((path+filename).c_str(),"rb");
    if(!fp2){
        printf("cannot craate file Tokenlist!\n");
        Error =TRUE;
    }
    fseek(fp2,fp_num*sizeof(TokenType),0);
    fread(p,sizeof(TokenType),1,fp2);
    fp_num++;
    fclose(fp2);
}
/*功能：创建语法树根节点
 * 说明：为语法树创建一个新节点并将语法树结点成员初始化
 * */
TreeNode * newRootNode(){
    TreeNode *t = new TreeNode();
    /*语法树结点未能成功分配，将错误信息写入文件*/
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        /*指定新语法树结点t成员：结点类型为nodekind的语句类型ProK，标记为根节点*/
        t->nodeKind = ProK;
        //更新源代码行号
        t->lineno = lineno;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}

/*函数名：newPheadNode
 * 功能：创建程序头语法树结点函数
 * 说明：如上，但是标记nodekind为PheadK，程序头结点
 * */
TreeNode * newPheadNode(){
    TreeNode *t = new TreeNode();
    /*语法树结点未能成功分配，将错误信息写入文件*/
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;
        /*指定新语法树结点t成员：结点类型为nodekind的语句类型ProK，标记为程序头节点*/
        t->nodeKind = PheadK;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}
/* 功能：创建声明语法树结点的函数，没有指明具体的结点声明类型，
 *      在语法树第二层
 * 说明：该函数为语法树创建一个新的结点，并将语法树结点成员初始化
 */
TreeNode * newDecANode(NodeKind kind){
    TreeNode * t = new TreeNode();
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }
    else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;

        /*指定新语法树结点t成员：结点类型为nodekind的参数kind*/
        t->nodeKind = kind;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}
/*创建声明类型语法树节点函数
 * 说明：为语法树创建一个新的声明类型节点
 *      并将语法树节点成员初始化
 * */
TreeNode * newDecNode(){
    TreeNode * t = new TreeNode();
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }
    else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;

        /*指定新语法树结点t成员：结点类型为nodekind的参数DecK*/
        t->nodeKind = DecK;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}
/*功能：创建函数类型语法树结点
 * 说明：为语法树创建一个新的函数类型节点
 * */
TreeNode * newProcNode(){
    TreeNode * t = new TreeNode();
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }
    else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;

        /*指定新语法树结点t成员：结点类型为nodekind的参数DecK*/
        t->nodeKind = ProcDecK;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}
/*功能：创建语句标志类型语法树结点，并将其成员初始化
 * */
TreeNode * newStmlNode(){
    TreeNode * t = new TreeNode();
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
        Error = TRUE;
    }
    else{
        /*初始化语法树结点各子节点、兄弟结点为NULL*/
        for (int i = 0; i < MAXCHILDREN; ++i) {
            t->child[i] = NULL;
        }
        t->sibling = NULL;

        /*指定新语法树结点t成员：结点类型为nodekind的参数DecK*/
        t->nodeKind = StmLK;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}































































