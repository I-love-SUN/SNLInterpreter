//
// Created by long on 2023/5/12.
//

#include "util.h"


//extern FILE *fp;

std::string path = "../outFile/";
std::string filename = "Tokenlist";
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

/*功能：创建语句类型节点，并将其成员初始化
 * */
TreeNode * newStmtNode(StmtKind kind){
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

        /*指定新语法树结点t成员：结点类型为nodekind的参数StmtK*/
        t->nodeKind = StmtK;
        t->kind.stmt = kind;
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

/*功能：创建表达式类型节点，并将语法树节点成员初始化
 * */
TreeNode * newExpNode(ExpKind kind){
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

        /*指定新语法树结点t成员：结点类型为nodekind的参数ExpK*/
        t->nodeKind = ExpK;
        /* 指定新语法树节点t成员: 表达式类型kind.exp为函数给定参数kind */
        t->kind.exp = kind;
        //更新源代码行号
        t->lineno = lineno;
        //初始化变量计数标志
        t->idnum = 0;
        /* 指定新语法树节点t成员: 表达式为变量类型时的变量类型varkind为IdV.*/
        t->attr.ExpAttr.varkind=IdV;
        /* 指定新语法树节点t成员: 类型检查类型type为Void */
        t->attr.ExpAttr.type=Void;
        for (int i = 0; i < 10; ++i) {
            t->name[i]="\0";
            t->table[i] = NULL;
        }
    }
    return t;
}


/* 函数名 printSpaces
/* 功  能 空格打印函数
/* 说  明 该函数打印指定数量空格,用于表示子树缩进
*/
static void printSpaces(){

    /*按给定缩进量indentno打印空格进行缩进其中缩进量indentno总能保持非负*/
    for (int i=0;i<indentno;i++)
        fprintf(listing," ");
}


/*
 * 函数名:printTab
 * 功能:打印空格
 */
void printTab(int tabnum)
{
    for (int i = 0; i < tabnum; i++)
        fprintf(listing," ");
}

/* 函数名 printTree
* 功  能 把语法树输出，显示在listing文件中
* 说  明 该函数运用了宏来定义增量减量的缩进
*/
void  printTree(TreeNode  *tree)
{
    int i;
    /* 增量缩进宏,每次进入语法树节点都进行增量缩进 */
    INDENT;
    /* 函数参数给定语法树节点指针tree非NULL(空) */
    while (tree != NULL)
    {
        /*打印行号*/
        if(tree->lineno==0)
            printTab(9);
        else
            switch((int)(tree->lineno / 10))
            {
                case 0:
                    fprintf(listing,"line:%d",tree->lineno);
                    printTab(3);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    fprintf(listing,"line:%d",tree->lineno);
                    printTab(2);
                    break;
                default:
                    fprintf(listing,"line:%d",tree->lineno);
                    printTab(1);
            }
        /* 调用函数printSpaces,打印相应的空格,进行缩进 */
        printSpaces();
        switch ( tree->nodeKind)
        {
            case ProK :
                fprintf(listing,"%s  ","ProK"); break;
            case PheadK:
            {
                fprintf(listing,"%s  ","PheadK");
                fprintf(listing,"%s  ",tree->name[0].c_str());
            }
                break;
            case DecK:
            {
                fprintf(listing,"%s  ","DecK");
                if (tree->attr.ProcAttr.paramt==varparamType )
                    fprintf(listing,"%s  ","var param:");
                if (tree->attr.ProcAttr.paramt==valparamType)
                    fprintf(listing,"%s  ","value param:");
                switch(tree->kind.dec){
                    case  ArrayK:
                        {
                        fprintf(listing,"%s  ","ArrayK");
                        fprintf(listing,"%d  ",tree->attr.ArrayAttr.up);
                        fprintf(listing,"%d  ",tree->attr.ArrayAttr.low);
                        if (tree->attr.ArrayAttr.childtype == CharK)
                            fprintf(listing,"%s  ","CharK");
                        else if( tree->attr.ArrayAttr.childtype == IntegerK)
                            fprintf(listing,"%s  ","IntegerK");
                        };
                        break;
                    case  CharK:
                        fprintf(listing,"%s  ","CharK");break;
                    case  IntegerK:
                        fprintf(listing,"%s  ","IntegerK");break;
                    case  RecordK:
                        fprintf(listing,"%s  ","RecordK");break;
                    case  IdK:
                        fprintf(listing,"%s  ","IdK");
                        fprintf(listing,"%s  ",tree->attr.typeName.c_str());
                        break;
                    default:
                        fprintf(listing,"error1!");
                        Error = TRUE;
                };
                if (tree->idnum !=0)
                    for (int i=0 ; i <= (tree->idnum);i++){
                        fprintf(listing,"%s  ",tree->name[i].c_str());

                    }
                else{
                    fprintf(listing,"wrong!no var!\n");
                    Error = TRUE;
                }
            }
            break;
            case TypeK:
                fprintf(listing,"%s  ","TypeK");break;
            case VarK:
                fprintf(listing,"%s  ","VarK");
                if(tree->table[0]!=NULL)
                    fprintf(listing,"%d  %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);
                break;
            case ProcDecK:
                fprintf(listing,"%s  ","ProcDecK");
                fprintf(listing,"%s  ",tree->name[0].c_str());
                if(tree->table[0]!=NULL)
                    fprintf(listing,"%d %d %d  ",tree->table[0]->attrIR.More.ProcAttr.mOff,tree->table[0]->attrIR.More.ProcAttr.nOff,tree->table[0]->attrIR.More.ProcAttr.level);
                break;
            case StmLK:
                fprintf(listing,"%s  ","StmLk");break;
            case StmtK:
            {
                fprintf(listing,"%s  ","StmtK");
                switch (tree->kind.stmt)
                {
                    case IfK:
                        fprintf(listing,"%s  ","If");break;
                    case WhileK:
                        std::cout << "11111111";
                        fprintf(listing,"%s  ","While");break;
                    case AssignK:
                        fprintf(listing,"%s  ","Assign");break;
                    case ReadK:
                        fprintf(listing,"%s  ","Read");
                        fprintf(listing,"%s  " , tree->name[0].c_str());
                        if(tree->table[0]!=NULL)
                            fprintf(listing,"%d   %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);
                        break;
                    case WriteK:
                        fprintf(listing,"%s  ","Write");break;
                    case CallK:
                        fprintf(listing,"%s  ","Call");
                        fprintf(listing,"%s  ", tree->name[0].c_str());
                        break;
                    case ReturnK:
                        fprintf(listing,"%s  ","Return");break;

                    default:
                        fprintf(listing,"error2!");
                        Error = TRUE;
                }
            };
                break;
            case ExpK:
            {
                fprintf(listing,"%s  ","ExpK");
                switch (tree->kind.exp)
                {
                    case OpK:
                    {
                        fprintf(listing,"%s  ","Op");
                        switch(tree->attr.ExpAttr.op)
                        {
                            case EQ:   fprintf(listing,"%s  " , "="); break;
                            case LT:   fprintf(listing,"%s  " , "<"); break;
                            case PLUS: fprintf(listing,"%s  " , "+"); break;
                            case MINUS:fprintf(listing,"%s  " , "-"); break;
                            case TIMES:fprintf(listing,"%s  " , "*"); break;
                            case OVER: fprintf(listing,"%s  " , "/"); break;
                            default:
                                fprintf(listing,"error3!");
                                Error = TRUE;
                        }
                        if(tree->attr.ExpAttr.varkind==ArrayMembV)
                        {
                            fprintf(listing,"ArrayMember  ");
                            fprintf(listing,"%s  ",tree->name[0].c_str());
                        }
                    };
                    break;
                    case ConstK:
                        fprintf(listing,"%s  ","Const");
                        switch(tree->attr.ExpAttr.varkind)
                        {
                            case IdV:
                                fprintf(listing,"Id  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            case FieldMembV:
                                fprintf(listing,"FieldMember  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            case ArrayMembV:
                                fprintf(listing,"ArrayMember  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            default:
                                fprintf(listing,"var type error!");
                                Error = TRUE;
                        }

                        fprintf(listing,"%d  ",tree->attr.ExpAttr.val);
                        break;
                    case VariK:
                        fprintf(listing,"%s  ","Vari");
                        switch(tree->attr.ExpAttr.varkind)
                        {
                            case IdV:
                                fprintf(listing,"Id  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            case FieldMembV:
                                fprintf(listing,"FieldMember  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            case ArrayMembV:
                                fprintf(listing,"ArrayMember  ");
                                fprintf(listing,"%s  ",tree->name[0].c_str());
                                break;
                            default:
                                fprintf(listing,"var type error!");
                                Error = TRUE;
                        }
                        if(tree->table[0]!=NULL)
                            fprintf(listing,"%d   %d  ",tree->table[0]->attrIR.More.VarAttr.off,tree->table[0]->attrIR.More.VarAttr.level);
                        break;
                    default:
                        fprintf(listing,"error4!");
                        Error = TRUE;
                }
            };
            break;
            default:
                fprintf(listing,"error5!");
                Error = TRUE;
        }
        fprintf(listing,"\n");
        /* 对语法树结点tree的各子结点递归调用printTree过程 *
         * 缩进写入列表文件listing						   */
        for (i=0;i<MAXCHILDREN;i++)
            printTree(tree->child[i]);
        /* 对语法树结点tree的兄弟节点递归调用printTree过程 *
         * 缩进写入列表文件listing						   */
        tree = tree->sibling;
    }

    /* 减量缩进宏,每次退出语法树节点时减量缩进 */
    UNINDENT;
}
