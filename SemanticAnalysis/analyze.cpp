//
// Created by long on 2023/5/24.
//

#include "analyze.h"
#include "symbTable.h"


//scope栈的层数
int Level = -1;
//同层偏移量
int Off;
//主程序的off偏移
int mainOff;
//scope栈
SymbTable * scope[SCOPESIZE];
//该指针一直指向整数类型的内部表示
TypeIR *intPtr = NULL;
//指向字符类型的内部表示
TypeIR * charPtr = NULL;
//指向布尔类型的内部表示
TypeIR * boolPtr = NULL;
//处理数组型变量
static TypeIR * arrayVar(TreeNode* t);
//处理记录型类型变量
static TypeIR * recordVar(TreeNode* t);


/*功能：该函数初始化整数类型，字符类型，布尔类型的内部表示
 * 说明：由于这三种类型均为基本类型，内部表示固定。
 * */
void initialize(){
    intPtr = NewTy(intTy);
    charPtr = NewTy(charTy);
    boolPtr = NewTy(boolTy);

    for(int i = 0; i<SCOPESIZE;i++)
        scope[i] = NULL;
}

/*功能：完成类新分析的工作说明：处理语法树当前结点类型，构造出当前类型
 *      的内部表示，并将其地址返回给Ptr类型的内部表示地址
 * */
TypeIR *TypeProcess(TreeNode* t,DecKind decKind){
    TypeIR *ptr = NULL;
    switch(decKind)
    {
        case IdK:
            ptr = nameType(t);
            break;
        case ArrayK:
            ptr = arrayType(t);
            break;
        case RecordK:
            ptr = recordType(t);
            break;
        case IntegerK:
            ptr = intPtr;
            break;
        case CharK:
            ptr = charPtr;
            break;
    }
    return ptr;
}

/*功能：用来在符号表中查找已定义的类型名字
 * 说  明  调用寻找表项地址函数FindEntry，返回找到的表项地址
            指针entry。如果present为FALSE，则发生无声明错误。
            如果符号表中的该标识符的属性信息不是类型，则非类
            型标识符。该函数返回指针指向符号表中的该标识符的
	        类型内部表示。
*/
TypeIR * nameType(TreeNode* t){
    TypeIR * ptr = NULL;
    SymbTable * entry = NULL;
    int present;

    present = FindEntry(t->attr.typeName,&entry);
    if (present == TRUE)
    {
        /*检查该标识符是否为类型标识符*/
        if(entry->attrIR.kind != typeKind)
            ErrorPrompt(t->lineno,t->attr.typeName,"used before typed!\n");
        else
            ptr = entry->attrIR.idtype;
    }
    else{ //没有找到标识符
        ErrorPrompt(t->lineno,t->attr.typeName,"type name is not declared!\n");
    }
    return ptr;
}

/*功能：处理数组类型的内部表示
 * 说明：类型为数组时，检查下标是否合法
 * */
TypeIR *arrayType(TreeNode* t) {
    TypeIR *ptr0 = NULL;
    TypeIR *ptr1 = NULL;
    TypeIR *ptr = NULL;
    /*检查数组上下界合法性*/
    if(t->attr.ArrayAttr.low > t->attr.ArrayAttr.up){
        ErrorPrompt(t->lineno,"","array subscript error!\n");
        Error = TRUE;
    }
    else{
        /*处理下标类型*/
        ptr0 = TypeProcess(t,IntegerK);
        /*处理元素类型*/
        ptr1 = TypeProcess(t,t->attr.ArrayAttr.childtype);
        /*指向新建的类型信息表*/
        ptr = NewTy(arrayTy);
        /*计算类型长度*/
        ptr->size =(t->attr.ArrayAttr.up-t->attr.ArrayAttr.low +1)*(ptr1->size);
        ptr->More.ArrayAttr.indexTy=ptr0;
        ptr->More.ArrayAttr.elemTy=ptr1;
        ptr->More.ArrayAttr.low=t->attr.ArrayAttr.low;
        ptr->More.ArrayAttr.up=t->attr.ArrayAttr.up;
    }
    return ptr;
}

/*功能：处理记录类型的内部表示
 * 说明：类型为记录时，是有记录体组成的，其中内部结点有三个信息：
 *      1.空间大小size；2.类型种类标志recordTy；3.体部分的
 *      节点地址body。记录类型中的域名都是标识符的定义性出现，
 *      因此需要记录其属性。
 * */
TypeIR *recordType(TreeNode* t){
    TypeIR * ptr = NewTy(recordTy);
    t=t->child[0];         //从语法树的儿子节点读取域信息

    fieldchain * ptr1 = NULL;
    fieldchain * ptr2 = NULL;
    fieldchain * body = NULL;
    while(t!=NULL){
        for (int i = 0; i < t->idnum; ++i) {
            ptr2 = NewBody();
            if(body==NULL)
                body = ptr1 = ptr2;
            ptr2->id = t->name[i];
            ptr2->UnitType = TypeProcess(t,t->kind.dec);
            ptr2->Next = NULL;

            if(ptr2 != ptr1){
                ptr2->off = ptr1->off + ptr1->UnitType->size;
                ptr1->Next = ptr2;
                ptr1 = ptr2;
            }
        }
        t = t->sibling;
    }
    ptr->size = ptr2->off+ ptr2->UnitType->size;
    ptr->More.body = body;
    return ptr;
}

/******************************声明的语义分析****************************/
/*功能：处理类型声明的语义分析
 * 说明：遇到类型T时，构造其内部结点Tptr，对于“idname=T”构造
 *      符号表项，并检查本层类型声明中是否有重复定义 的错误
 * */
void TypeDecPart(TreeNode* t){
    int present = FALSE;
    AttributeIR attributeIr;
    SymbTable * entry = NULL;
    /*添加属性作为参数*/
    attributeIr.kind = typeKind;
    while(t!=NULL){
        present = Enter(t->name[0],&attributeIr,&entry);
        if(present!=FALSE){
            ErrorPrompt(t->lineno,t->name[0],"is repetation declared!\n");
            entry = NULL;
        }else{
            entry->attrIR.idtype = TypeProcess(t,t->kind.dec);
        }
        t = t->sibling;
    }
}

/*功能：处理变量声明的语义分析
 * 说明：调用变量处理分析函数
 * */
void VarDecPart(TreeNode* t){
    varDecList(t);
}

/*功能：处理变量声明的语义分析
 * 说明：遇到标识符id时，把id登记到符号表中，检查重复性定义
 *      遇到类型时，构造其内部表示
 * */
void varDecList(TreeNode* t){
    AttributeIR  attrIr;
    int present = FALSE;
    SymbTable  *entry = NULL;
    while(t!=NULL){
        attrIr.kind = varKind;
        for (int i = 0; i < t->idnum; ++i) {
            attrIr.idtype = TypeProcess(t,t->kind.dec);
            /*判断是值参还是变参*/
            if(t->attr.ProcAttr.paramt == varparamType){
                attrIr.More.VarAttr.access=indir;
                attrIr.More.VarAttr.level=Level;
                //计算形参的偏移量
                attrIr.More.VarAttr.off=Off;
                Off++;   //变参的偏移加一
            } else{
                attrIr.More.VarAttr.access=dir;
                attrIr.More.VarAttr.level=Level;
                //计算值参的偏移
                if(attrIr.idtype!=NULL){
                    attrIr.More.VarAttr.off=Off;
                    Off+=attrIr.idtype->size;
                }
            }
            present = Enter(t->name[i],&attrIr,&entry);
            if(present!=FALSE){
                ErrorPrompt(t->lineno,t->name[i]," is defined repetation!\n");
            }
            else
                t->table[i]=entry;
        }
        if(t!=NULL)
            t=t->sibling;
    }
    /*如果是主程序，则记录此时偏移，用于目标代码生成时的displayOff*/
    if(Level == 0){
        mainOff = Off;
        /*存储主程序AR的display表的偏移到全局变量*/
        StoreNoff = Off;
    }
        /*如果不是主程序，则记录此时偏移，用于下面填写过程信息表的noff信息*/
    else
        savedOff = Off;
}

/*功能：处理过程声明的语义分析
 * 说明：在当前符号表中填写过程标识符的属性，
 *      在新层符号表中填写形参标识符的属性
 * */
void procDecPart(TreeNode *t){
    TreeNode * p= t;
    SymbTable * entry = HeadProcess(t);
    t=t->child[1];
    //如果过程内存在声明，则处理声明
    while(t!=NULL){
        switch (t->nodeKind)
        {
            case TypeK: TypeDecPart(t->child[0]); break;
            case VarK: VarDecPart(t->child[0]); break;
            //如果有函数声明，则跳出循环处理
            case ProcDecK:
                break;
            default:
                ErrorPrompt(t->lineno,"","no this kind in syntax tree!");
                break;
        }
        if(t->nodeKind == ProcDecK)
            break;
        else
            t=t->sibling;
    }
    entry->attrIR.More.ProcAttr.nOff=savedOff;
    /*过程活动记录的长度等于nOff加上display表的长度diplay表的长度等于过程所在层数加一*/
    entry->attrIR.More.ProcAttr.mOff=entry->attrIR.More.ProcAttr.nOff+entry->attrIR.More.ProcAttr.level+1;

    //处理程序的声明部分
    while(t!=NULL){
        procDecPart(t);
        t=t->sibling;
    }
    t=p;
    Body(t->child[2]);
    if(Level!=-1)
        DestroyTable();  //结束当前的scope
}

/*功能：处理函数头的语义分析
 * 说明：在当前符号表中填写函数标识符的属性，在新层符号表中
 *      填写形参标识符的属性，其中过程的大小和代码都需要回填
 * */
SymbTable * HeadProcess(TreeNode* t){
    AttributeIR attrIr;
    int present = FALSE;
    SymbTable * entry = NULL;
    attrIr.kind=procKind;
    attrIr.idtype=NULL;
    attrIr.More.ProcAttr.level=Level+1;
    if(t!=NULL){
        /*等级函数的符号表项*/
        present=Enter(t->name[0],&attrIr,&entry);
        t->table[0]=entry;
    }
    entry->attrIR.More.ProcAttr.param=ParaDecList(t);
    return entry;
}

/*功能：处理函数头中的参数声明的语义分析
 * 说明：在新的符号表中登记所以形参的表现，构造形参表项的地址表，
 *      并有para指向其。
 * */
ParamTable * ParaDecList(TreeNode* t){
    TreeNode * p =NULL;
    ParamTable * ptr1 =NULL;
    ParamTable * ptr2 =NULL;
    ParamTable * head =NULL;
    if(t!=NULL){
        if(t->child[0]!=NULL)
            p=t->child[0];
        CreateTable();        //进入新的局部化区域
        Off = 7;               /*子程序中的变量初始偏移设为8*/
        VarDecPart(p);      	/*变量声明部分*/
        SymbTable * ptr0 = scope[Level];
        while(ptr0!=NULL){       //只要非空就访问其兄弟节点
            //构造形参符号表，并使其连接至符号表的param项
            ptr2 = NewParams();
            if(head==NULL)
                head = ptr1 = ptr2;
            ptr2->entry = ptr0;
            ptr2->next=NULL;
            if(ptr2!=ptr1){
                ptr1->next=ptr2;
                ptr1=ptr2;
            }
            ptr0=ptr0->next;
        }
    }
    return head;
}

/*功能：该函数处理执行体部分的语义分析
 * 说明：编译系统的执行体部分即为语句序列，故只需处理语句序列部分
 * */
void Body(TreeNode* t){
    if(t->nodeKind==StmLK){
        TreeNode *p = t->child[0];
        while(p!=NULL){
            statement(p);
            p=p->sibling;
        }
    }
}

/*功能：处理语句状态的函数
 * 说明：根据语法树节点的kind项判断应该转向哪个语句处理函数
 * */
void statement (TreeNode * t)
{
    switch(t->kind.stmt)
    {
        case IfK:			ifstatement(t); break;
        case WhileK:		whilestatement(t); break;
        case AssignK:		assignstatement(t); break;
        case ReadK:		    readstatement(t); break;
        case WriteK:		writestatement(t);break;
        case CallK:		    callstatement(t); break;
        case ReturnK:		returnstatement(t); break;
        default :
            ErrorPrompt(t->lineno,"","statement type error!\n");
            break;
    }
}

/*功能：处理表达式的分析
 * 说明：主要检查运算分量的相容性，求表达式的类型，kind记录了参数类型
 * */
TypeIR * Expr(TreeNode * t,AccessKind  *  Ekind)
{
    int present = FALSE;
    SymbTable * entry = NULL;
    TypeIR * Eptr0 = NULL;
    TypeIR * Eptr1 = NULL;
    TypeIR * Eptr = NULL;
    if(t!=NULL)
        switch(t->kind.exp)
        {
            case ConstK:
                Eptr = TypeProcess (t,IntegerK);
                Eptr->kind = intTy;
                if(Ekind!=NULL)
                    *Ekind = dir;   /*直接变量*/
                break;
            case VariK:
                /*Var = id的情形*/
                if(t->child[0]==NULL)
                {
                    /*在符号表中查找此标识符*/
                    present = FindEntry(t->name[0],&entry);
                    t->table[0] = entry;
                    if(present!=FALSE)
                    {   /*id不是变量*/
                        if (FindAttr(entry).kind!=varKind)
                        {
                            ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
                            Eptr = NULL;
                        }
                        else{
                            Eptr = entry->attrIR.idtype;
                            if (Ekind!=NULL)
                                *Ekind = indir;  /*间接变量*/
                        }
                    }
                    else /*标识符无声明*/
                    {
                        ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
                    }
                }
                else/*Var = Var0[E]的情形*/
                {
                    if(t->attr.ExpAttr.varkind==ArrayMembV)
                        Eptr = arrayVar(t);
                    else /*Var = Var0.id的情形*/
                    if(t->attr.ExpAttr.varkind==FieldMembV)
                        Eptr = recordVar(t);
                }
                break;
            case OpK:
                /*递归调用儿子节点*/
                Eptr0 = Expr(t->child[0],NULL);
                if(Eptr0==NULL)
                    return NULL;
                Eptr1 = Expr(t->child[1],NULL);
                if(Eptr1==NULL)
                    return NULL;
                /*类型判别*/
                present = Compat(Eptr0,Eptr1);
                if (present != FALSE)
                {
                    switch(t->attr.ExpAttr.op)
                    {
                        case LT:
                        case EQ:
                            Eptr = boolPtr;
                            break;  /*条件表达式*/
                        case PLUS:
                        case MINUS:
                        case TIMES:
                        case OVER:
                            Eptr = intPtr;
                            break;  /*算数表达式*/
                    }
                    if(Ekind!=NULL)
                        *Ekind = dir; /*直接变量*/
                }
                else
                    ErrorPrompt(t->lineno,"","operator is not compat!\n");
                break;
        }
    return Eptr;
}

static TypeIR * arrayVar(TreeNode* t){
    int present = FALSE;
    SymbTable * entry = NULL;
    TypeIR * Eptr0 = NULL;
    TypeIR * Eptr1 = NULL;
    TypeIR * Eptr = NULL;
    /*在符号表中查找此标识符*/
    present = FindEntry(t->name[0],&entry);
    t->table[0] = entry;
    if(present!=FALSE){  //找到了
        if(FindAttr(entry).kind!=varKind){ //var不是变量
            ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
            Eptr = NULL;
        }
        else
            if(FindAttr(entry).idtype!=NULL) //var不是数组变量
                if(FindAttr(entry).idtype->kind!=arrayTy){
                    ErrorPrompt(t->lineno,t->name[0],"is not an array variable error!\n");
                    Eptr = NULL;
                }
                else{
                    Eptr0 = entry->attrIR.idtype->More.ArrayAttr.indexTy;
                    if(Eptr0==NULL)
                        return NULL;
                    Eptr1 = Expr(t->child[0],NULL);
                    if(Eptr1==NULL)
                        return NULL;
                    present = Compat(Eptr0,Eptr1);
                    if(present!=TRUE){
                        ErrorPrompt(t->lineno,"","type is not matched with the array member error!\n");
                        Eptr= NULL;
                    }
                    else
                        Eptr = entry->attrIR.idtype->More.ArrayAttr.elemTy;
                }
    }
    else
        ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
    return Eptr;
}

/*处理记录变量中的域的分析
 * 说明：检查var:=var0.id中的var0是不是记录类型变量，id
 *      是不是该记录类型中的域成员
 * */
static TypeIR * recordVar(TreeNode * t)
{
    int present = FALSE;
    int result = TRUE;
    SymbTable * entry = NULL;
    TypeIR * Eptr0 = NULL;
    TypeIR * Eptr1 = NULL;
    TypeIR * Eptr = NULL;
    fieldchain * currentP = NULL;
    /*在符号表中查找此标识符*/
    present = FindEntry(t->name[0],&entry);
    t->table[0] = entry;
    if(present!=FALSE)      /*找到*/
    {
        /*Var0不是变量*/
        if (FindAttr(entry).kind!=varKind)
        {
            ErrorPrompt(t->lineno,t->name[0],"is not variable error!\n");
            Eptr = NULL;
        }
        else    /*Var0不是记录类型变量*/
            if(FindAttr(entry).idtype->kind!=recordTy)
            {
                ErrorPrompt(t->lineno,t->name[0],"is not record variable error !\n");
                Eptr = NULL;
            }
            else/*检查id是否是合法域名*/
            {
                Eptr0 = entry->attrIR.idtype;
                currentP = Eptr0->More.body;
                while((currentP!=NULL) && (result!=FALSE)){
                    result = strcmp(t->child[0]->name[0].c_str(),currentP->id.c_str());
                    /*如果相等*/
                    if(result==FALSE)
                        Eptr = currentP->UnitType;
                    else
                        currentP = currentP->Next;
                }
                if(currentP==NULL)
                    if(result!=FALSE){
                        ErrorPrompt(t->child[0]->lineno,t->child[0]->name[0],"is not field type!\n");
                        Eptr = NULL;
                    }
                    /*如果id是数组变量*/
                    else if(t->child[0]->child[0]!=NULL)
                        Eptr = arrayVar(t->child[0]);
            }
    }
    else/*标识符无声明*/
        ErrorPrompt(t->lineno,t->name[0],"is not declarations!\n");
    return Eptr;
}

/*功能：处理赋值语句分析
 * 说明：主要检测赋值号两端分类类型相容性
 * */
void assignstatement(TreeNode* t){
    SymbTable * entry = NULL;
    int present = FALSE;
    TypeIR *ptr = NULL;
    TypeIR * Eptr = NULL;
    TreeNode * child1 = NULL;
    TreeNode * child2 = NULL;
    child1 = t->child[0];
    child2 = t->child[1];
    if(child1->child[0]==NULL){
        //在符号表中查找标识符
        present = FindEntry(child1->name[0],&entry);
        if(present!=FALSE){
            //id不是变量
            if(FindAttr(entry).kind!=varKind){
                ErrorPrompt(child1->lineno,child1->name[0],"is not variable error!\n");
                Eptr = NULL;
            }
            else{
                Eptr = entry->attrIR.idtype;
                child1->table[0]=entry;
            }
        }
        else   //标识符没有声明
            ErrorPrompt(child1->lineno,child1->name[0],"is not declaration error!\n");
    }
    else{
        if(child1->attr.ExpAttr.varkind==ArrayMembV)
            Eptr = arrayVar(child1);
        else if(child1->attr.ExpAttr.varkind==FieldMembV)
                Eptr = recordVar(child1);
    }
    if(Eptr!=NULL){
        if(t->nodeKind==StmtK && t->kind.stmt==AssignK){
            ptr = Expr(child2,NULL);
            //检测赋值号两侧类型是否等价
            if(!Compat(ptr,Eptr))
                ErrorPrompt(t->lineno,"","assign expression is error!\n");
        }
    }
}

/*功能：处理函数调用语句分析
 * 说明：函数调用语句的语义分析首先检查符号表求出其属性中的param部分，
 *      并用它检查形参和实参之间的对应关系是否正确
 * */
void callstatement(TreeNode* t){
    AccessKind Ekind;
    int  present = FALSE;
    SymbTable * entry = NULL;
    TreeNode * p = NULL;
    //用id检查整个符号表
    present = FindEntry(t->child[0]->name[0],&entry);
    t->child[0]->table[0]=entry;
    if(present==FALSE){
        ErrorPrompt(t->lineno,t->child[0]->name[0],"function is not declared!\n");
    }
    else
        //id不是函数名
        if(FindAttr(entry).kind!=procKind)
            ErrorPrompt(t->lineno,t->name[0],"is not function name!\n");
        else{//形参实参匹配
            p=t->child[1];
            ParamTable * paramP = FindAttr(entry).More.ProcAttr.param;
            while (p!=NULL && paramP!=NULL){
                SymbTable * paraEntry = paramP->entry;
                TypeIR * Etp = Expr(p,&Ekind);//实参
                //参数类型不匹配
                if(FindAttr(paraEntry).More.VarAttr.access==indir && Ekind==dir)
                    ErrorPrompt(p->lineno,"","param kind mismatch!\n");
                else
                    if(FindAttr(paraEntry).idtype!=Etp)
                        ErrorPrompt(p->lineno,"","param type mismatch!\n");
                p=p->sibling;
                paramP=paramP->next;
            }
            if(p!=NULL || paramP!=NULL)
                ErrorPrompt(t->child[1]->lineno,"","param num is not match!\n");
        }
}

/*功能：处理条件语句分析
 *说明：分析语法树的三个儿子节点
 * */
void ifstatement(TreeNode* t){
    AccessKind * Ekind = NULL;
    TypeIR * Etp = Expr(t->child[0],Ekind);
    if(Etp!=NULL){
        if(Etp->kind!=boolTy)
            ErrorPrompt(t->lineno,"","condition expression error!\n");
        else{
            TreeNode * p = t->child[1];
            while(p!=NULL){
                statement(p);
                p=p->sibling;
            }
            t=t->child[2];
            while(t!=NULL){
                statement(t);
                t=t->sibling;
            }
        }
    }
}

/*功能：处理循环语句分析
 * 说明：分析语法树的两个儿子节点
 * */
void whilestatement(TreeNode* t){
    TypeIR * Etp = Expr(t->child[0],NULL);
    if(Etp!=NULL){
        if(Etp->kind!=boolTy)
            ErrorPrompt(t->lineno,"","condition expression error!\n");
        else{
            t=t->child[1];
            while(t!=NULL){
                statement(t);
                t=t->sibling;
            }
        }
    }
}

/*功能：处理输入语句分析
 * 说明：分析语法树节点，检查变量有无声明和是否为变量错误
 * */
void readstatement(TreeNode* t){
    SymbTable * entry = NULL;
    int present = FALSE;
    present = FindEntry(t->name[0],&entry);
    t->table[0] = entry;
    //标识符未声明
    if(present==FALSE)
        ErrorPrompt(t->lineno,t->name[0]," is not declared!\n");
    else
        //不是变量标识符
        if(entry->attrIR.kind!=varKind)
            ErrorPrompt(t->lineno,t->name[0]," is not var name!\n");
}

/*功能：处理语句分析
 * 说明：分析输出语句中的表达式是否合法
 * */
void writestatement(TreeNode* t){
    TypeIR * Etp = Expr(t->child[0],NULL);
    if(Etp!=NULL)
        if(Etp->kind==boolTy)
            ErrorPrompt(t->lineno,"","expression type error!\n");
}

/*功能：处理返回语句分析
 * 分析函数返回语句是否在主程序中出现
 * */
void returnstatement(TreeNode* t){
    if(Level==0)
        ErrorPrompt(t->lineno,"","return statement error!\n");
}

/*功能：处理总的语义分析
 * 说明：对语法树进行分析
 * */
void analyze(TreeNode* t){
    SymbTable * entry = NULL;
    TreeNode * p = NULL;
    TreeNode * pp = t;
    CreateTable();   //创建符号表
    initialize();   //类型内部初始化函数
    p=t->child[1];  //语法树声明节点
    while(p!=NULL){
        switch (p->nodeKind) {
            case TypeK:
                TypeDecPart(p->child[0]);
                break;
            case VarK:
                VarDecPart(p->child[0]);
                break;
            case ProcDecK:
                procDecPart(p);
                break;
            default:
                ErrorPrompt(p->lineno,"","not a node such this in syntax tree!\n");
                break;
        }
        p=p->sibling;
    }
    /*程序体*/
    t=t->child[2];
    if(t->nodeKind==StmLK)
        Body(t);
    //撤销符号表
    if(Level!=-1)
        DestroyTable();
    //输出语义错误
    if(Error==TRUE)
        fprintf(listing,"\nanalyze error:\n");
    else
        fprintf(listing,"\n.........no error!\n");
}


































































