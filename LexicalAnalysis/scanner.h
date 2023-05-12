/*
 *编译器的词法扫描
 */

/* 判断联入标志，如该头文件已经联入则不再联入*/
#ifndef SNLINTERPRETER_SCANNER_H
#define SNLINTERPRETER_SCANNER_H


/*
 * 词法分析器确定性有限自动机DFA状态类型
 */
typedef enum{
    START,      //开始状态
    INASSIGN,   //赋值状态
    INRANGE,    //下标范围状态
    INCOMMENT,  //注释状态
    INNUM,      //数字状态
    INID,       //标识符状态
    INCHAR,     //字符标志状态
    DONE        //完成状态
}StateType;

void getTokenlist();

#endif //SNLINTERPRETER_SCANNER_H
