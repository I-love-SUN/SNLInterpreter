//
// Created by Lenovo on 2023/5/24.
//

#include "../global.h"
#include "../util.h"
#include "../SemanticAnalysis/symbTable.h"
#include "midcode.h"

/*临时变量*/
int TempOffset = 0;

/*标号*/
int Label = 0;

/*指向第一条中间代码*/
CodeFile *firstCode = NULL;

/*指向当前最后一条中间代码*/
CodeFile *lastCode = NULL;

/*函数声明*/

/*中间代码生成主要函数*/
CodeFile *GenMidCode(TreeNode* t);
/*过程声明的中间代码生成*/
void GenProDec(TreeNode* t);
/*语句序列中间代码生成*/
void GenBody(TreeNode* t);
/*语句的中间代码生成*/
void GenStatement(TreeNode* t);
/*赋值语句的中间代码生成*/
void GenAssignS(TreeNode* t);
/*处理变量*/
ArgRecord *GenVar(TreeNode *t);
/*处理数组成员变量*/
ArgRecord *GenArray(ArgRecord *V1arg,TreeNode *t,int low ,int size);
/*处理域变量*/
ArgRecord *GenField(ArgRecord *V1arg,TreeNode *t ,fieldChain *head );
/*处理表达式的中间代码生成*/
ArgRecord *GenExpr(TreeNode *t);
/*过程调用的中间代码生成*/
void GenCallS(TreeNode *t );
/*输入语句的中间代码生成*/
void GenReadS (TreeNode *t );
/*输出语句的中间代码生成*/
void GenWriteS(TreeNode *t );
/*条件语句的中间代码生成*/
void GenIfS(TreeNode  *t );
/*循环语句的中间代码生成*/
void GenWhileS(TreeNode *t);


/*实现*/

CodeFile *GenMidCode(TreeNode* t)
{
    TreeNode *t1 = t->child[1];
    while(t1!=NULL)
    {
        if(t1->nodeKind==ProcDecK)
        {
            GenProDec(t1);
        }
        t1 = t1->sibling;
    }

    ArgRecord  *Noff = ARGValue(StoreNoff);
    CodeFile *code = GenCode(MENTRY,NULL,NULL,Noff);
    TempOffset = StoreNoff + 1;
    GenBody(t->child[2]);

    int size = TempOffset;
    ArgRecord  *sizeArg = ARGValue(size);
    code->codeR.arg2= sizeArg;
    return firstCode;
}

/*
 * 函数名：GenProDec
 * 功能：过程声明中间代码生成函数
 */
void GenProcDec(TreeNode *t)
{

    /*过程如果标号*/
    int ProcEntry = NewLabel( );

    /*过程名在符号表中的地址*/
    SymbTable  *Entry = t->table[0];

    /*过程入口标号，回填到节点中*/
    Entry->attrIR.More.ProcAttr.codeEntry = ProcEntry;

    /*过程的display表的偏移量*/
    int noff = Entry->attrIR.More.ProcAttr.nOff;

    /*得到过程的层数及其ARG结构*/
    int procLevel = Entry->attrIR.More.ProcAttr.level;
    ArgRecord *levelArg = ARGValue(procLevel);


    /*÷若过程内部仍有过程声明，调用相应的函数，产生过程声明的中间代码*/
    TreeNode *t1=t->child[1];
    while (t1!=NULL)
    { if (t1->nodeKind=ProcDecK)
            GenProcDec(t1);
        t1=t1->sibling;
    }

    /*产生过程入口中间代码*/
    ArgRecord  *arg1=ARGLabel(ProcEntry);
    CodeFile *code = GenCode(PENTRY,arg1,NULL,levelArg);

    /*初始化临时变量的开始编号，为过程临时变量区的第一个地址*/
    TempOffset =  noff + procLevel+1;

    /*调用语句序列的代码生成函数处理过程体*/
    GenBody(t->child[2]);

    /*得到过程的AR大小，回填到过程的入口中间代码*/
    int size = TempOffset;
    ArgRecord *sizeArg = ARGValue(size);
    code->codeR.arg2 = sizeArg;

    /*产生过程出口中间代码*/
    GenCode(ENDPROC,NULL,NULL,NULL);
}

/*
 * 函数名：GenBody
 * 功能：语句序列中间代码生成函数
 */
void GenBody(TreeNode *t)
{
    TreeNode *t1 = t;
    if(t1->nodeKind==StmLK)
    {
        t1 = t1->child[0];
    }
    while(t1!=NULL)
    {
        GenStatement(t1);
        t1 = t1->sibling;
    }
}

/*
 * GenStatement
 * 语句处理函数
 */

void GenStatement(TreeNode *t)
{
    switch (t->kind.stmt) {
        case AssignK: GenAssignS(t);break;
        case CallK:GenCallS(t);break;
        case ReadK:GenReadS(t);break;
        case WriteK:GenWriteS(t);break;
        case IfK:GenIfS(t);break;
        case WhileK:GenWhileS(t);break;
        case ReturnK:
            GenCode(RETURNC,NULL,NULL,NULL);
            break;
        default:break;
    }
}

/*
 * GenAssignS
 * 赋值语句处理函数
 */
void GenAssignS(TreeNode*t)
{
    ArgRecord *Larg = GenVar(t->child[0]);
    ArgRecord *Rarg = GenExpr(t->child[1]);
    GenCode(ASSIGN,Rarg,Larg,NULL);
}

/*
 * GenVar
 * 变量处理函数
 */
ArgRecord *GenVar(TreeNode *t)
{
    int low,size;
    fieldChain *head = NULL;

    /*生成变量名的arg结构，entry是标识符在符号表中的地址*/
    SymbTable  *Entry = t->table[0];
    ArgRecord  *V1arg = ARGAddr(t->name[0],
                                Entry->attrIR.More.VarAttr.level,
                                Entry->attrIR.More.VarAttr.off,
                                Entry->attrIR.More.VarAttr.access);
    ArgRecord *Varg = NULL;
    switch (t->attr.ExpAttr.varkind) {
        case IdV:Varg = V1arg;break;
        case ArrayMembV:
            low = Entry->attrIR.idtype->More.ArrayAttr.low;
            size = Entry->attrIR.idtype->More.ArrayAttr.elemTy->size;
            Varg = GenArray(V1arg,t,low,size);
            break;
        case FieldMembV:
            head = Entry->attrIR.idtype->More.body;
            Varg = GenField(V1arg,t ,head);
            break;
        default : break;
    }
    return Varg;
}

ArgRecord *GenArray(ArgRecord *V1arg,TreeNode *t,int low,int size)
{
    /*处理下标表达式*/
    ArgRecord *Earg= GenExpr(t->child[0]);

    ArgRecord  *lowArg = ARGValue(low);
    ArgRecord  *sizeArg= ARGValue(size);

    /*产生三个临时变量*/
    ArgRecord  *temp1= NewTemp(dir);
    ArgRecord  *temp2= NewTemp(dir);
    /*复杂变量的临时变量属于间接访问*/
    ArgRecord  *temp3= NewTemp(indir);

    /*生成中间代码*/
    GenCode(SUB, Earg, lowArg ,temp1);
    GenCode(MULT,temp1,sizeArg,temp2);
    GenCode(AADD,V1arg,temp2, temp3);

    return temp3;
}

/*
 * GenArray
 * 域变量处理函数
 */
ArgRecord *GenField(ArgRecord *V1arg,TreeNode *t,fieldChain *head)
{
    ArgRecord  *FieldV = NULL;
    /*t1指向当前域成员*/
    TreeNode  *t1 = t->child[0];
    fieldChain  *Entry2 = NULL ;

    FindField(t1->name[0],head ,&Entry2);
    /*域名在域表中的偏移*/
    int  off = Entry2->off;

    ArgRecord  *temp1 = NewTemp( indir);
    GenCode(AADD , V1arg, offArg , temp1);
    /*域是数组变量*/
    if (t1->attr.ExpAttr.varkind==ArrayMembV)
    {  int  low = Entry2->UnitType->More.ArrayAttr.low;
        int  size= Entry2->UnitType->More.ArrayAttr.elemTy->size;
        FieldV = GenArray(temp1, t1,low,size );
    }
    else
        FieldV = temp1;

    return FieldV;
}

/*
 * GenExpr(TreeNode *t)
 * 表达式处理函数
 */
ArgRecord *GenExpr(TreeNode *t)
{
    ArgRecord  *arg = NULL;
    ArgRecord  *Larg = NULL;
    ArgRecord  *Rarg = NULL;
    ArgRecord  *temp = NULL;

    switch(t->kind.exp)
    {
        case VariK:
            arg = GenVar(t);
            break ;
        case ConstK:
            /*得到值得ARG结构*/
            arg = ARGValue(t->attr.ExpAttr.val);
            break;
        case OpK:
            /*处理左部和右部*/
            Larg = GenExpr(t->child[0]);
            Rarg = GenExpr(t->child[1]);

            /*根据操作符类别，得到中间代码的类别*/
            CodeKind  op;
            switch(t->attr.ExpAttr.op)
            {
                case LT:   	op = LTC ; break;
                case EQ:		op = EQC ; break;
                case PLUS:		op = ADD ; break;
                case MINUS:    op = SUB ; break;
                case TIMES:    op = MULT; break;
                case OVER :    op = DIV ; break;
                default : break;
            }
            /*产生新的临时变量*/
            temp = NewTemp(dir);

            GenCode(op, Larg ,Rarg , temp);
            arg = temp ;
            break;
        default : break;
    }
    return arg;
}

/*
 * 函数名 GenCall
 * 过程调用处理函数
 * 分别调用表达式处理函数处理各个实参，
 *      并生成相应的形参实参结合中间代码
 *      从符号表中过程标识符属性中，查到入口标号
 *      产生过程调用中间代码
 */
void GenCallS(TreeNode *t)
{
    /*取得过程标识符在符号表中得地址*/
    SymbTable  *Entry = t->child[0]->table[0];
    ParamTable *param = Entry->attrIR.More.ProcAttr.param;
    /*调用表达式处理函数处理每个参数*/
    TreeNode *t1 = t->child[1];
    ArgRecord  *Earg = NULL;
    while (t1!=NULL)
    {
        Earg = GenExpr(t1);
        /*记录参数偏移*/
        int paramOff = param->entry->attrIR.More.VarAttr.off;
        ArgRecord  *OffArg = ARGValue(paramOff);
        /*形实参结合中间代码*/
        if (param->entry->attrIR.More.VarAttr.access==dir)
            /*值参ë*/
            GenCode(VALACT, Earg ,OffArg,NULL);
        else  /*变参*/
            GenCode(VARACT, Earg ,OffArg,NULL);
        t1 = t1->sibling;
        param = param->next;
    }

    /*¹过程入口标号*/
    int label = Entry->attrIR.More.ProcAttr.codeEntry;
    ArgRecord *labelarg = ARGLabel(label);

    /*过程的display表得偏移量*/
    int Noff = Entry->attrIR.More.ProcAttr.nOff;
    ArgRecord *Noffarg = ARGValue(Noff);
    GenCode (CALL,labelarg, NULL,Noffarg);
}

/*
 * 读语句处理函数
 */
void GenReadS(TreeNode *t )
{
    SymbTable *Entry = t->table[0];
    ArgRecord  *Varg = ARGAddr(t->name[0],
                               Entry->attrIR.More.VarAttr.level,
                               Entry->attrIR.More.VarAttr.off,
                               Entry->attrIR.More.VarAttr.access);
    GenCode(READC,Varg,NULL,NULL);
}

/*
 * 写语句处理函数
 */
void GenWriteS(TreeNode *t)
{
    /*调用表达式处理*/
    ArgRecord *Earg = GenExpr(t->child[0]);
    GenCode(WRITEC,Earg ,NULL ,NULL);
}

/*
 * 条件语句处理函数
 */
void GenIfS(TreeNode *t)
{
    /*生成else部分入口标号*/
    int elseL = NewLabel( );
    ArgRecord *ElseLarg =ARGLabel(elseL);

    /*生成if语句出口标号*/
    int outL = NewLabel( );
    ArgRecord *OutLarg = ARGLabel(outL);

    /*条件表达式中间代码生成*/
    ArgRecord *Earg = GenExpr(t->child[0]);

    /*表达示为假，跳转到else入口*/
    GenCode(JUMP0, Earg, ElseLarg ,NULL);

    /*then部分*/
    GenBody(t->child[1]);

    /*if出口*/
    GenCode(JUMP, OutLarg ,NULL ,NULL );

    /*else部分入口标号声明*/
    GenCode(LABEL,ElseLarg,NULL,NULL);

    /*else部分*/
    GenBody(t->child[2]);

    /*if语句出口标号声明*/
    GenCode(LABEL,OutLarg,NULL,NULL);

}

/*
 * 循环语句处理函数
 */
void GenWhileS(TreeNode *t )
{
    /*生成while语句入口标号*/
    int  inL = NewLabel() ;
    ArgRecord *InLarg = ARGLabel(inL);

    /*生成while语句出口标号*/
    int outL = NewLabel( );
    ArgRecord *OutLarg = ARGLabel(outL);

    /*while语句入口标号声明*/
    GenCode(WHILESTART ,InLarg,NULL,NULL);

    /*条件表达式*/
    ArgRecord *Earg = GenExpr(t->child[0]);

    /*为假跳转到出口*/
    GenCode(JUMP0, Earg, OutLarg ,NULL);

    /*循环体*/
    GenBody(t->child[1]);

    /*跳到while入口*/
    GenCode(JUMP, InLarg ,NULL ,NULL );

    /*while出口标号声明*/
    GenCode(ENDWHILE,OutLarg, NULL,NULL);
}




