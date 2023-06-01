/*================================
 * 主题：实现SNL语言的编译程序
 */
#include <iostream>
#include <bits/stdc++.h>
using std::string;

/*
 * 按照要求，对文件的引入加以限制
 */
#include "global.h"
#include "LexicalAnalysis/scanner.h"
#include "SyntaxAnalysis/parse.h"
#include "SemanticAnalysis/analyze.h"
#include "SemanticAnalysis/symbTable.h"
#include "MidCodeGeneration/midcode.h"
#include "util.h"
/*
 * 全局变量区
 */
 FILE *fp;
 FILE *source;
 FILE *listing;

int Tokennum = 0;
int lineno = 0;
int EchoSource = TRUE;
int Error;
int StoreNoff;
int savedOff=0;
CodeFile  *midcode = NULL;

int main() {

    std::cout << "Hello, SNL!" << std::endl;
//      ../testFiles/test1.txt
//      ../testFiles/test2.txt
//      ../testFiles/test3.txt
//      ../testFiles/test4.txt

    string programName = "..\\testFiles\\test2.txt";
    std::cout << "Please input source file: \n";
    std::cin>>programName;

    source = fopen(programName.c_str(),"r");
    if(source == NULL)
    {
        fprintf(stderr,"File %s not found.\n",programName.c_str());
        exit(1);
    }
    listing = stdout;
    fprintf(listing,"\nSource program: %s\n",programName.c_str());

    //获取程序的token链表
    getTokenlist();

    fprintf(listing,"\nLexical analysizing:\n");
    fprintf(listing,"\ntoken list:\n");
    //打印词法分析的结果
    printTokenlist();
    getchar();

    //保存语法树的根节点
    TreeNode * syntaxTree;
    //进行语法分析
    fprintf(listing,"\nRecursive descent Syntax analysizing:\n");
    syntaxTree = parse();

    /* 如果没有语法错误,则将生成的语法树输出到屏幕 */
    if (!Error)
    {
        fprintf(listing,"\nSyntax tree:\n\n");
        printTree(syntaxTree);
        getchar();
    }

    fprintf(listing ,"\nSematic analysizing:\n");
    fprintf(listing ,"Checking  sematic  error......\n");
    /*语义分析*/
    analyze(syntaxTree);
    if (!Error)
        fprintf(listing ," \nNo  error !\n");

    /*输出符号表*/
    if (!Error)
    {
        fprintf(listing ,"\nNow printing  symb table.....\n");
        PrintSymbTable();
        getchar();
    }
    /*输出含符号表信息的语法树*/
    if (!Error)
    {
        fprintf(listing,"after  analysis ..");
        fprintf(listing,"\nSyntax tree:\n\n");
        printTree(syntaxTree);
        getchar( );
    }
    //输出生成的中间代码
    if (!Error)
    {
        std::cout << "midcode:\n";
        midcode = GenMidCode(syntaxTree);
        fprintf(listing,"\n>>Generated  Midcode:\n");
        PrintMidCode(midcode);
        getchar();
    }
    return 0;
}
