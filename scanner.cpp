/*
 * 编译器的词法扫描器实现
 */

/*
 * 包含头文件
 */
#include "global.h"
#include<string.h>
#include<ctype.h>
#include "util.h"
#include<math.h>

/*====================================================|
 * static函数只能声明后的本文中调用,static函数在内存中只有一份 |
 * static全局变量只能声明后的本文中调用                     |
 *====================================================|*/

/*
 * 词法分析器确定性有限自动机DFA状态类型
 */
typedef enum{
    START,      //开始状态
    INASSIGN,   //赋值装填
    INRANGE,    //下标范围状态
    INCOMMENT,  //注释状态
    INNUM,      //数字状态
    INID,       //标识符状态
    INCHAR,     //字符状态
    DONE        //完成状态
}StateType;

/*tokenString*同于保存标识符和保留字单词的词元*/
char tokenString[MAXTOKENLEN+1];

/*源码行的输入缓冲区长度为256*/
#define BUFLEN 256

/*lineBuf为当前输入代码行缓冲区*/
static char lineBuf[BUFLEN];

/*linepos为代码缓冲区lineBuf当前字符位置，初始0*/
static int linepos = 0;

/*buf_strSize为当前缓冲区中所存字符串大小*/
static int buf_strSize = 0;

/*文件读完标志,初始为false*/
static int EOF_flag = FALSE;


/*----------------------------------------------------------------------------
 * 函数：getNextChar()
 * 功能：读取到下一个非空字符
 * 说明：该函数从lineBuf里面读取下一个非空字符，如果lineBuf中的读完，则读取源程序中新的一行
 *----------------------------------------------------------------------------*/

static int getNextChar(){
    if(linepos<buf_strSize){
        return lineBuf[linepos++];
    }
    else{

        /*从源文件的source中读入BURLEN-1个字符到行缓冲区中
         * 若该行（包括最后一个换行符）的字符数超过n-1，则fgets只返回一个不完整的行，
         * 缓冲区总是以NULL字符结尾
         */
        if(fgets(lineBuf,BUFLEN-1,source))
        {
            if(EchoSource)fprintf(listing,"%4d: %s",lineno,lineBuf);
            /*开始扫描新的一行*/
            buf_strSize = strlen(lineBuf);
            /*linepo置零*/
            linepos = 0;
            return lineBuf[linepos++];
        }
        else
        {
            /*fgets返回NULL，已经读到文件尾部*/
            EOF_flag = TRUE;
            /*declared in:stdio.h   #define EOF (-1)*/
            return EOF;
        }
    }
}

/*---------------------------------------------------
 * 函数：ungetNextChar()
 * 功能：字符回退函数
 * 说明：lineBuf中的回退一个
 *---------------------------------------------------*/
static void ungetNextChar()
{
    /*还没读完的时候可以回退*/
    if(!EOF_flag) linepos--;
}

/*----------------------------------保留字查找表:21个---------------------------------*/
static struct{
    char *str;
    LexType tok;
}reservedWords[MAXRESERVED] ={
         {"program",PROGRAM},{"type",TYPE},{"var",VAR},{"procedure",PROCEDURE}
        ,{"begin",BEGIN},{"end",END},{"array",ARRAY},{"of",OF},{"record",RECORD}
        ,{"if",IF},{"then",THEN},{"else",ELSE},{"fi",FI},{"while",WHILE}
        ,{"do",DO},{"endwh",ENDWH},{"read",READ},{"write",WRITE},{"return",RETURN}
        ,{"integer",INTEGER},{"char",CHAR}
};

/*
 * 函数：reservedLookup()
 * 功能：查找保留字
 * 说明：将识别出来的字符串传入，查找保留字表
 */

static LexType reservedLookup(char *str){
    /*21个*/
    for(int i = 0;i<MAXRESERVED;i++){
        if(strcmp(str,reservedWords[i].str)==0){
            return reservedWords[i].tok;
        }
    }
    /*不是保留字就是标识符*/
    return ID;
}

/*---------------------------------------------------
 * 函数：getTokenlist()
 * 功能：得到单词
 * 说明：识别所有的token序列，使用确定性有限自动机DFA的直接转向法
 *      产生词法错误时，是否修改？？？？？？？？？
 *----------------------------------------------------*/

void getTokenlist()
{

    ChainNodeType *chainHead; //头节点
    ChainNodeType *cur;         //当前处理的节点
    ChainNodeType *cur_pre;     //当前节点的前驱
    ChainNodeType *temp;        //临时指针

    TokenType curToken;

    /*第一个节点*/
    chainHead = cur_pre = cur = (ChainNodeType *)malloc(sizeof(ChainNodeType));

    cur->nextToken = nullptr;
    do{
        /*tokenString中的当前正在识别字符的位置*/
        int tokenStringIndex = 0;

        /*都是以START作为开始状态*/
        StateType state = START;

        /*决定是否将此字符存入tokenString*/
        int saveFlag;

        while(state!=DONE){
            int ch = getNextChar();

            /*默认保存*/
            saveFlag = TRUE;
            switch(state)
            {
                case START: {
                    if (isdigit(ch)) state = INNUM;/*遇到数字转换到数字状态*/
                    else if (isalpha(ch))state = INID;/*遇到字符是标识符状态*/
                    else if (ch == ':') state = INASSIGN;/*赋值状态*/
                    else if (ch == '.') state = INRANGE;/*下标范围状态*/
                    else if (ch == '{') {
                        saveFlag = FALSE;
                        state = INCOMMENT;/*注释状态*/
                    } else if (ch == '\'') {
                        saveFlag = FALSE;
                        state = INCHAR;
                    } else if (ch == ' ' || ch == '\t' || ch == '\n') {
                        saveFlag = FALSE;
                    } else {
                        state = DONE;
                        switch (ch) {
                            case EOF: {
                                saveFlag = FALSE;
                                curToken.Lex = ENDFILE;
                                break;
                            }
                            case '+': {
                                curToken.Lex = PLUS;
                                break;
                            }
                            case '-': {
                                curToken.Lex = MINUS;
                                break;
                            }
                            case '*': {
                                curToken.Lex = TIMES;
                                break;
                            }
                            case '/': {
                                curToken.Lex = OVER;
                                break;
                            }
                            case '=': {
                                curToken.Lex = EQ;
                                break;
                            }
                            case '<': {
                                curToken.Lex = LT;
                            }
                            case '(': {
                                curToken.Lex = LPAREN;
                                break;
                            }
                            case ')': {
                                curToken.Lex = RPAREN;
                                break;
                            }
                            case '[': {
                                curToken.Lex = LMIDPAREN;
                                break;
                            }
                            case ']': {
                                curToken.Lex = RMIDPAREN;
                                break;
                            }
                            case ';': {
                                curToken.Lex = SEMI;
                                break;
                            }
                            case ',': {
                                curToken.Lex = COMMA;
                                break;
                            }
                            default: {
                                curToken.Lex = ERROR;
                                Error = TRUE;
                                break;
                            }
                        }

                    }
                    break;
                }
                case INID:{
                    break;

                }
                case INNUM:{
                    break;
                }
                case INASSIGN:{
                    break;
                }
                case INCOMMENT:{
                    saveFlag = FALSE;
                    if(ch==EOF){
                        state = DONE;
                        curToken.Lex = ENDFILE;
                    }
                    else if(ch=='}') state = START;  /*注释状态结束，设置为开始状态*/
                    break;
                }
                case INCHAR:{
                    break;
                }
                case INRANGE:{
                    break;

                }
                case DONE:break;/*确定性有限自动机处于单词结束位置*/
            }


        }

    }while((curToken.Lex)!=ENDFILE));
}