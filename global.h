//=================================
/*
 * 编译器的全局类型和变量，应该在其他包含文件之前
 */

/*如已经联入头文件global.h则不再联入*/
#ifndef SNLINTERPRETER_GLOBAL_H
#define SNLINTERPRETER_GLOBAL_H

#include<stdio.h>
#include<stdlib.h>
#include <string>
#include <string.h>
using std::string;
/*
 * 定义常量FALSE为0，TRUE为1
 */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/*
 * 定义保留字最大长度常量 21
 */
#define MAXRESERVED 21

/*
 * 定义单词最大长度常量为40
 */
#define MAXTOKENLEN 40

/*
 * 初始化符号中变量的偏移
 */

#define INITOFF 7

/*
 * scope栈的大小
 * */
#define SCOPESIZE 1000

/*
 * 定义链表节点的长度
 */
#define CHAINNODELEN sizeof(ChainNodeType)

/*
 * 定义token的长度
 * */
#define TOKENLEN sizeof (TokenType)




/*
 * ------------------单词词法类型--------------------
 */
typedef enum {

    /*簿记单词符号*/
    ENDFILE,    ERROR,

    /*保留字*/
    PROGRAM,    PROCEDURE,      TYPE,     VAR,    IF,
    THEN,       ELSE,           FI,       WHILE,  DO,
    ENDWH,      BEGIN,          END,      WRITE,  READ,
    ARRAY,      OF,             RECORD,   RETURN,

    /*数据基类型*/
    INTEGER,    CHAR,

    /*多字符单词符号*/
    ID,     INTC,       CHARC,

    /*特殊符号：运算符等等*/
    ASSIGN,     EQ,     LT,         PLUS,       MINUS,
    TIMES,      OVER,   LPAREN,     RPAREN,     DOT,
    COLON,      SEMI,   COMMA,      LMIDPAREN,  RMIDPAREN,
    UNDERANGE,


}LexType;

/*
 * --------------SNL的token结构:包括词法信息和语义信息----------
 */
typedef struct tokenType
{
    int     Lineshow; //记录该单词在源程序中的行数
    LexType Lex;        //记录该单词的词法信息
    char    Sem[MAXTOKENLEN+1]; //记录该单词的语义信息
}TokenType;

/*toke序列链表*/
typedef struct node
{
    TokenType  Token;/*单词*/
    struct node *nextToken;
    node():nextToken(nullptr){};
}ChainNodeType;

/*
 * 语法分析树
 * */
typedef enum{
    /*标志节点：只表示结点的类型，没有具体的内容*/
    ProK, //语法树根标志节点
    PheadK, //程序头标志节点
    TypeK, //类型声明标志节点
    VarK, //变量声明标志结点
    ProcDecK, //过程声明标志结点
    FunkDeck,//函数声明标志结点
    StmLK, //语句序列标志结点
    /*声明结点：*/
    StmtK, //语句声明结点
    DecK, //声明类型节点
    ExpK //表达式结点
}NodeKind;


/*
 * 声明类型的枚举
 * */
typedef enum {
    ArrayK, //数组类型
    CharK, //字符类型
    IntegerK, //整数类型
    RecordK, //记录类型
    IdK //类型标识符作为类型
}DecKind;


/*
 * 语句类型枚举
 * */
typedef enum {
    IfK, //判断类型if
    WhileK, //循环类型While
    AssignK, //赋值类型=
    ReadK, //读类型（程序输入）
    WriteK, //写类型（程序输出）
    CallK, //函数调用类型
    ReturnK //函数返回类型
} StmtKind;


/*
 * 表达式类型枚举
 * */
typedef enum {
    OpK, //操作类型
    ConstK, //常数类型
    VariK //变量类型
} ExpKind;


/*
 * 变量类型枚举
 * */
typedef enum {
    IdV, //标识符
    ArrayMembV, //数组成员
    FieldMembV, //域成员
}VarKind;


/*
 * 类型检查类型枚举
 * */
typedef enum {
    Void,
    Integer,
    Boolean,
    Char
}ExpType;


/*
 * 参数类型枚举
 * */
typedef enum {
    valparamType, //值参
    varparamType //变参
}ParamType;

/*定义语法树结点的最大子节点数为3，
 * 过程声明部分子节点child[0]指向参数部分，
 * 子节点child[1]指向声明体部分，
 * 子节点child[2]指向函数的语句部分
 * */
#define MAXCHILDREN 3

//提前声明符号表
struct symbtable;

//语法树结点
typedef struct treeNode
{
    struct treeNode *child[MAXCHILDREN];    //子节点指针
    struct treeNode *sibling;               //兄弟结点指针
    int lineno;                             //源程序代码行号
    NodeKind nodeKind;                      //结点类型

    union {                                 //声明的结点类型（具体类型）
        DecKind dec;
        StmtKind stmt;
        ExpKind exp;
    }kind;

    int idnum;                      //相同类型的变量个数
    string name[10];                //标识符的名称

    struct symbtable *table[10];    //与标识符对应的符号表地址，在语义分析阶段填入

    struct {

        struct{
            int low;                //数组上界
            int up;                 //数组下界
            DecKind childtype;      //数组的子类型
        }ArrayAttr;                 //数组类型

        struct {
            ParamType paramt;       //过程的参数属性
        }ProcAttr;                  //过程属性

        struct {
            LexType op;             //表达式的操作符
            int val;                //表达式的值
            VarKind varkind;        //变量的类别
            ExpType type;           //表达式属性
        }ExpAttr;

        string typeName;            //类型名是标识符
    }attr;

}TreeNode;

//非终极符的个数
#define NTMLNUM 68

//终极符个数
#define TMLNUM 42

/*所有非终极符*/
typedef enum
{
    Program,	      ProgramHead,	    ProgramName,	DeclarePart,
    TypeDec,          TypeDeclaration,	TypeDecList,	TypeDecMore,
    TypeId,	          TypeName,			BaseType,	    StructureType,
    ArrayType,        Low,	            Top,            RecType,
    FieldDecList,     FieldDecMore,	    IdList,	        IdMore,
    VarDec,	          VarDeclaration,	VarDecList,		VarDecMore,
    VarIdList,	      VarIdMore,		ProcDec,		ProcDeclaration,
    ProcDecMore,      ProcName,		    ParamList,		ParamDecList,
    ParamMore,        Param,		    FormList,		FidMore,
    ProcDecPart,      ProcBody,	    	ProgramBody,	StmList,
    StmMore,          Stm,				AssCall,		AssignmentRest,
    ConditionalStm,   StmL,			    LoopStm,		InputStm,
    InVar,            OutputStm,		ReturnStm,		CallStmRest,
    ActParamList,     ActParamMore,		RelExp,			OtherRelE,
    Exp,			  OtherTerm,		Term,           OtherFactor,
    Factor,           Variable,			VariMore,		FieldVar,
    FieldVarMore,     CmpOp,			AddOp,          MultOp
}NontmlType;

/*所有的终极符都取自单词词法类型的枚举定义*/
typedef LexType TmlType;


/*LL1分析栈，存放终极符和非终极符*/
typedef struct Node
{
    /*flag=1位非终极符，flag=2位终极符*/
    int flag;

    union{
        NontmlType Ntmlvar; /*非终极符部分*/
        TmlType tmlvar; /*终极符部分 */
    };

    /*指向下一个结点的指针*/
    struct Node *underNode;
}StackNode;

/*
 * 创建语法树所需的类型和变量****************************
 */
/*
 * 建立声明和语句部分的语法树所设的指针栈
 * 栈中存放的是指针地址
 * */
typedef struct NodePA
{
    TreeNode **pointer;
    struct NodePA *underNode;
} StackNodePA;

/*为生成表达式部分的语法树所设的指针栈
 * 栈中存放的是指向树节点的指针
 * */
typedef struct NodeP
{
    TreeNode **pointer;
    struct NodeP *underNode;
}StackNodeP;



/*
 * 全局共享
 */
/*源代码文本文件指针*/
extern FILE *source;

/*列表输出指针*/
extern FILE *listing;

/*词法分析结果tokenlist存储文件指针*/
extern FILE *fp;

/*源文件的行号*/
extern int lineno;

/*Token序列中单词总数,初始为0*/
extern int Tokennum;


/*符号栈、操作符栈的指针及标志*/

/*符号栈顶指针*/
extern StackNode *StackTop;

/*栈空标志*/
extern int STACKEMPTY;

/*语法树栈顶指针*/
extern StackNodePA *StackTopPA;

/*栈空标志*/
extern int paSTACKEMPTY;

/*操作符栈顶指针*/
extern StackNodeP * OpStackTop;

/*栈空标志*/
extern int OpSTACKEMPTY;

/*操作数栈顶标志*/
extern StackNodeP *NumStackTop;

/*操作数栈空标志*/
extern int NumSTACKEMPTY;

/*
 * 追踪标志：本次指定的输出设备是文件listing
 */
/*源代码跟踪标志，判断是否要将源程序列表输出到指定设备*/
extern int EchoSource;
/*词法分析追踪标志，初始为FALSE。为TRUE,将词法分析信息输出到指定设备*/
extern int TraceScan;
/*语法分析追踪标志，初始为FALSE,为TRUE，将语法分析信息输出到指定设备*/
extern int TraceParse;
/*符号表追踪标志，初始为TRUE,为TRUE，将语义分析产生的符号表信息输出到指定设备*/
extern int TraceTable;
/*目标代码追踪*/
extern int TraceCode;
/*错误追踪标志，防止错误进一步传递*/
extern int Error;

/**  语义分析需要用到的类型和变量定义  **/

/*标识符类型*/
typedef enum{
    typeKind,varKind,procKind
} IdKind;

/*变量的类型*/
typedef enum{
    dir,//直接变量（值参）
    indir//间接变量（变参）
} AccessKind;

/*形参表的结构定义*/
typedef struct  paramTable
{
    struct symbtable   *  entry;/*指向该形参所在符号表中的地址入口*/
    struct paramTable  *  next;
}ParamTable;

struct typeIR;

/*标识符的属性结构定义*/
typedef struct {
    struct typeIR *idtype;      /*指向标识符的类型内部表示*/
    IdKind kind;                /*标识符的类型*/
    union{
        struct{
            AccessKind access;  /*判断是变参还是值参*/
            int     level;
            int     off;
            bool    isParam;    /*判断是参数还是普通变量*/
        }VarAttr;   /*变量标识符的属性*/
        struct{
            int level;  /*该过程的层数*/
            ParamTable *param;   /*参数表*/
            int mOff;   /*过程活动记录的大小*/
            int nOff;   /*sp到display表的偏移量*/
            int procEntry;  /*过程的入口地址*/
            int codeEntry;  /*过程入口标号,用于中间代码生成*/
        }ProcAttr;  /*过程名标识符的属性*/
    }More;  /*标识符的不同类型有不同的属性*/
} AttributeIR;
/*符号表的结构定义*/
typedef struct symbtable{
    string idName;
    AttributeIR attrIR;
    struct symbtable * next;
}SymbTable;

/*使用scope栈的局部符号表方法中所用到的scope栈*/
extern SymbTable * scope[1000];
/*scope栈的层数*/
extern int Level;
/*在同层的变量偏移*/
extern int Off;
/*记录主程序的displayOff*/
extern int mainOff;
/*记录当前层的displayOff*/
extern int savedOff;
/*保存主程序的display表的偏移*/
extern  int StoreNoff ;

/*****************************************/
/************类型内部表示*******************/
/*****************************************/

typedef enum{
    intTy,
    charTy,
    arrayTy,
    boolTy,
    recordTy
}TypeKind;

struct typeIR;

/**域结构单元结构定义**/
typedef struct fieldchain
{
    string id;   //变量名
    int off;        //所在记录中的偏移
    struct typeIR *UnitType; //域中成员的类型
    struct fieldchain *Next;
}fieldChain;

/**类型的内部结构定义**/

typedef struct typeIR
{
    int size;   /*类型所占空间大小*/
    TypeKind kind;
    union
    {   struct
        {	struct typeIR	* indexTy;
            struct typeIR	* elemTy;
            int    low;     /*记录数组类型的下界*/
            int    up;      /*记录数组类型的上界*/
        }ArrayAttr;
        fieldChain * body;  /*记录类型中的域链*/
    } More;
}TypeIR;







/*中间代码相关*/

typedef  enum { LabelForm ,
                ValueForm ,
                AddrForm
} ArgForm;

typedef struct
{
    ArgForm form;
    union {
        int value;
        int label;
        struct {
            char name[10];
            int dataLevel;
            int dataOff;
            AccessKind access;
        }addr;
    }Attr;
}ArgRecord;

/*中间代码的类别*/
typedef  enum
{
    ADD , SUB , MULT ,DIV , EQC ,LTC,
    READC, WRITEC ,RETURNC ,ASSIG,AADD ,LABEL,
    JUMP0, JUMP ,CALL, VARACT ,VALACT,
    PENTRY , ENDPROC, MENTRY,WHILESTART,ENDWHILE
}CodeKind ;

/*中间代码的结构*/
typedef struct {
    CodeKind codekind;
    ArgRecord *arg1;
    ArgRecord *arg2;
    ArgRecord *arg3;
}CodeR;

typedef  struct  codeFile
{
    CodeR  codeR;
    struct codeFile  *former;
    struct codeFile  *next ;
} CodeFile ;



/*********************生成中间代码部分时用到的变量**********************/
/*临时变量编号*/
extern  int  TempOffset;
/*标号值*/
extern  int  Label ;

/*第一条中间代码*/
extern  CodeFile  *firstCode ;
/*最后一条中间代码*/
extern  CodeFile  *lastCode ;








#endif //SNLINTERPRETER_GLOBAL_H


