//=================================
/*
 * 编译器的全局类型和变量，应该在其他包含文件之前
 */

/*如已经联入头文件global.h则不再联入*/
#ifndef SNLINTERPRETER_GLOBAL_H
#define SNLINTERPRETER_GLOBAL_H

#include<stdio.h>
#include<stdlib.h>

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






#endif //SNLINTERPRETER_GLOBAL_H


