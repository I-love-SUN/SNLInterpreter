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

