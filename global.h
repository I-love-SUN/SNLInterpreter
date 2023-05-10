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
 * ------------------单词词法类型--------------------
 */
typedef enum {

    /*簿记单词符号*/
    ENDFILE,    ERROR,

    /*保留字*/
    PROGRAM,    PROCEDURE,      TYPE,   VAR,    TF,
    THEN,       ELSE,           FI,     WHILE,  DO,
    ENDWH,      BEGIN,          END,    WRITE,  READ,
    ARRAY,      OF,             RECORD, RETURN,

    /*数据基类型*/
    INTEGER,    CHAR,

    /*多字符单词符号*/
    ID,     INTC,       CHARC,

    /*特殊符号：运算符等等*/
    ASSIGN,     EQ,     LI,         PLUS,       MINUS,
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





#endif //SNLINTERPRETER_GLOBAL_H


