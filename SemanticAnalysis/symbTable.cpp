/**********符号处理程序**********/
#include "../global.h"

#include "stdio.h"

#include "string.h"

#include "symbTable.h"


static void printTy(TypeIR *ty);

static void printVar(SymbTable *entry);

static void printProc(SymbTable *entry);


/**   符号表相关操作    **/

/*
 * 函数名  PrintFieldTable
 * 功  能  打印纪录类型的域表
 */
void   PrintFieldChain(fieldChain  *currentP)
{
    fprintf(listing,"\n--------------Field  chain--------------------\n");
    fieldChain  *t=currentP;
    while (t!=NULL)
    {
        /*输出标识符名字*/
        fprintf(listing ,"%s:  ",t->id.c_str());
        /*输出标识符的类型信息*/

        switch(t->UnitType->kind)
        {
            case  intTy :  fprintf(listing ,"intTy     ");   break;
            case  charTy:	fprintf(listing ,"charTy    ");  break;
            case  arrayTy: fprintf(listing ,"arrayTy   "); break;
            case  recordTy:fprintf(listing ,"recordTy  ");break;
            default : fprintf(listing ,"error  type!  "); break;
        }
        fprintf(listing ,"off = %d\n",t->off);

        t = t->Next;
    }
}
/*
 * 函数名  PrintOneLayer
 * 功  能  打印符号表的一层
 * 说  明  有符号表打印函数PrintSymbTabl
 */
void  PrintOneLayer(int level)
{
    SymbTable  *t= scope[level];
    fprintf(listing,"\n-------SymbTable  in level %d ---------\n",level);

    while (t!=NULL)
    { /*输出标识符名字*/
        fprintf(listing ,"%s:   ",t->idName.c_str());
        AttributeIR  *Attrib = &(t->attrIR );
        /*输出标识符的类型信息，过程标识符除外*/
        if (Attrib->idtype!=NULL)  /*过程标识符*/
            switch(Attrib->idtype->kind)
            {
                case  intTy :  fprintf(listing ,"intTy  ");   break;
                case  charTy:	fprintf(listing ,"charTy  ");  break;
                case  arrayTy: fprintf(listing ,"arrayTy  "); break;
                case  recordTy:fprintf(listing ,"recordTy  ");break;
                default : fprintf(listing ,"error  type!  "); break;
            }
        /*输出标识符的类别，并根据不同类型输出不同其它属性*/
        switch(Attrib->kind)
        {
            case  typeKind :
                fprintf(listing, "typekind  "); break;
            case  varKind :
                fprintf(listing, "varkind  ");
                fprintf(listing ,"Level = %d  ", Attrib->More.VarAttr.level);
                fprintf(listing ,"Offset= %d  ", Attrib->More.VarAttr.off);

                switch(Attrib->More.VarAttr.access)
                {
                    case  dir :  fprintf(listing ,"dir  "); break;
                    case  indir: fprintf(listing ,"indir  ");break;
                    default :fprintf(listing ,"errorkind  ");  break;
                }
                break;
            case  procKind:
                fprintf(listing ,"funckind   ");
                fprintf(listing ,"Level= %d  ",Attrib->More.ProcAttr.level);
                fprintf(listing ,"Noff= %d  ",Attrib->More.ProcAttr.nOff);
                break;
            default :fprintf(listing ,"error  ");
        }
        fprintf(listing,"\n");
        t = t->next;
    }
}



/*
 * 函数名:printSymbTable
 * 功能:打印生成的符号表
 */
void PrintSymbTable() {
   int level = 0;
   while(scope[level]!= NULL){
       PrintOneLayer(level);
       level++;
   }
}

/*
 * 函数名:NewTable
 * 功能:创建当前空符号表
 */
SymbTable *NewTable(void)
{
    /*内存中动态申请分配单元，返回指向该单元的符号表类型指针*/
    SymbTable *table = new SymbTable();
    if (table == NULL)
    {
        fprintf(listing, "Out of memory!\n");
        Error = TRUE;
    }
    table->next = NULL;
    table->attrIR.kind = typeKind;
    table->attrIR.idtype = NULL;
    table->next = NULL;
    table->attrIR.More.VarAttr.isParam = false;
    return table;
}

/*
 * 函数名:CreateTable
 * 功能:创建空符号表\
 * 当进入一个新的局部化单位的时候，调用本程序
 */
void CreateTable(void)
{
    Level = Level + 1;
    scope[Level] = NULL;
    Off = INITOFF;
}

/*
 * 函数名:DstroyTable
 * 功能:撤销当前符号表
 * 退出一个局部化区的时候，层数减一，并撤销当前符号表
 */
void DestroyTable(void)
{
    //free(scope[Level]);
    Level = Level - 1;
    //scope[Level] = NULL;
}

/*
 * 函数名:Enter
 * 功能:登记标识符和属性
 * 说明:将标识符名，标识符属性登记在符号表中，
 *      登记成功，返回值为1，Entry指向登记的符号表中的位置；
 *      登记不成功，返回0，Entry为空
 */
int Enter(string id, AttributeIR *attribP,SymbTable **entry)
{
    int present = FALSE;
    SymbTable *curentry = scope[Level];
    SymbTable *prentry = scope[Level];
    if(scope[Level] == NULL)
    {
        curentry = NewTable();
        scope[Level] = curentry;
    }
    else {
        while (curentry != NULL) {
            prentry = curentry;
            if (id==curentry->idName) {
                fprintf(listing, "repetition declaration error!\n");
                Error = TRUE;
                present = TRUE;
            }
            else
                curentry = prentry->next;
        }
        if (present == FALSE) {
            curentry = NewTable();
            prentry->next = curentry;
        }
    }
    curentry->idName = id;
    curentry->attrIR.idtype = attribP->idtype;
    curentry->attrIR.kind = attribP->kind;
    switch(attribP->kind)
    {
        case typeKind:break;
        case varKind:
            curentry->attrIR.More.VarAttr.level =attribP->More.VarAttr.level;
            curentry->attrIR.More.VarAttr.off=attribP->More.VarAttr.off;
            curentry->attrIR.More.VarAttr.access=attribP->More.VarAttr.access;
            break;
        case procKind:
            curentry->attrIR.More.ProcAttr.param = attribP->More.ProcAttr.param;
            curentry->attrIR.More.ProcAttr.level = attribP->More.ProcAttr.level;
            break;
        default:break;
    }
    (*entry) = curentry;
    return present;
}

/*
 * 函数名:FindEntry
 * 功能:寻找表项地址
 * 说明:查找符号表中是否存在标识符id,求出其表项地址
 *      如果存在,返回值为1,
 *      否则返回0
 */
int FindEntry(string id, SymbTable **entry)
{
    int present = FALSE;
    int lev = Level; /*临时记录层数的变量*/
    SymbTable * findentry=  scope[lev];
    while ((lev!=-1)&&present!=TRUE)
    {
        while((findentry!=NULL)&&(present!=TRUE))
        {
            if(id==findentry->idName)
                present = TRUE;
            else
                findentry = findentry->next;
        }
        if(present!=TRUE)
        {
            lev = lev - 1;
            findentry= scope[lev];
        }
    }
    /*如果在本层中没有找到，去上一个局部化区域中查找*/
    if(present==TRUE)
    {
        (*entry) = findentry;
    } else{
        (*entry) = NULL;
    }
    return present;
}

/*
 * 函数名:FindAtrr
 * 功能:属性查询
 * 说明:对给定表项地址，求出其属性值，并将其返回给Atrrib的实参单元中
 */
AttributeIR FindAttr(SymbTable *entry)
{
    AttributeIR attrIr = entry->attrIR;
    return attrIr;
}

/*
 * 函数名:Compat
 * 功能 判断类型是否相容
 * 只有整型类型、字符类型、数组类型和记录类型，只需要判断类型内部表示产生的指针是否相同
 */
int Compat(TypeIR *ty1, TypeIR *ty2)
{
    if (ty1!= ty2)
    {
        return FALSE;
    }
    else return TRUE;
}

/*
 * 函数名：NewTy
 * 功能:创建当前空类型内部表示
 * 参数为类型，函数返回该类型的内部表示的地址
 */
TypeIR *NewTy(TypeKind kind)
{
    TypeIR *table = new TypeIR();
    if (table == NULL)
    {
        fprintf(listing, "Out of memory!\n");
        Error = TRUE;
    }
    else{
        switch(kind)
        {
            case intTy:
            case charTy:
            case boolTy:
                table->kind = kind;
                table->size = 1;
                break;
            case arrayTy:
                table->kind = arrayTy;
                table->More.ArrayAttr.indexTy = NULL;
                table->More.ArrayAttr.elemTy = NULL;
                break;
            case recordTy:
                table->kind = recordTy;
                table->More.body = NULL;
                break;
            default:break;
        }
    }
    return table;
}

/*
 * 函数名:NewBody
 * 功能:创建当前空记录类型中域的链表
 */
fieldChain* NewBody(void)
{
    fieldChain * Ptr = new fieldChain();
    if (Ptr==NULL)
    {
        fprintf(listing,"Out of memory error !");
        Error = TRUE;
    }
    else
    {
        Ptr->Next = NULL;
        Ptr->off = 0;
        Ptr->UnitType = NULL;
    }
    return Ptr;
}

/*
 * 函数名:NewParam
 * 功能:创建当前空形参链表
 */
ParamTable * NewParams(void)
{
    ParamTable * Ptr = new ParamTable();
    if (Ptr==NULL)
    {
        fprintf(listing,"Out of memory error !");
        Error = TRUE;
    }
    else
    {
        Ptr->entry = NULL;
        Ptr->next = NULL;
    }
    return Ptr;
}

/*
 * 函数名:ErrorPrompt
 * 错误提示
 */
void ErrorPrompt(int line,string name, string message)
{
    fprintf(listing,">>>Line: %d, %s %s",line,name.c_str(),message.c_str());    Error = TRUE;
    Error = TRUE;
    exit(0);
}


/*
 *函数名：FindField
 *功能：查找记录的域名
 */
bool FindField(string Id,fieldChain *head,fieldChain **Entry)
{
    bool  present =false;
    /*记录当前节点*/
    fieldChain *currentItem = head;
    /*从表头开始查找这个标识符，直到找到或到达表尾*/
    while ((currentItem!=NULL)&&( present==false))
    {
        if  (currentItem->id==Id)
        {
            present= true;
            if (Entry!=NULL)
                (*Entry)=currentItem;
        }
        else  currentItem=currentItem->Next;
    }

    return present;
}