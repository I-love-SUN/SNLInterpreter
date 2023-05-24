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
#include "util.h"
#include "SyntaxAnalysis/parse.h"

/*
 * 全局变量区
 */
 FILE *fp;
 FILE *source;
 FILE *listing;

int Tokennum = 0;
int lineno = 0;
int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int Error;



int main() {
    std::cout << "Hello, SNL!" << std::endl;

    string programName = "..\\testFiles\\test1.txt";
//    std::cout << "请输入程序的文件名：\n";
//    std::cin>>programName;

    source = fopen(programName.c_str(),"r");
    if(source == NULL)
    {
        fprintf(stderr,"File %s not found.\n",programName.c_str());
        exit(1);
    }
    listing = stdout;
    fprintf(listing,"\nTINY COMPILATION: %s\n",programName.c_str());

    getTokenlist();
    if(EchoSource)
    {
        getchar();
    }
    if (TraceScan)
    {
        fprintf(listing,"\nLexical analysizing:\n");
        fprintf(listing,"\ntoken list:\n");
        printTokenlist();
        getchar();
    }
    TreeNode * syntaxTree;

    fprintf(listing,"\nRecursive descent Syntax analysizing:\n");
    syntaxTree = parse();

    /* 如果语法分析追踪标志为TRUE且没有语法错误,
       则将生成的语法树输出到屏幕 */
    if ((TraceParse)&&(!Error))
    {
        fprintf(listing,"\nSyntax tree:\n\n");
        printTree(syntaxTree);
        getchar();
        getchar();
    }

    fprintf(listing ,"\nSematic analysizing:\n");
    fprintf(listing ,"Checking  sematic  error......\n");
    /*语义分析*/
    analyze(syntaxTree);
    if (!Error)
        fprintf(listing ," \nNo  error !\n");

    /*输出符号表*/
    if ((TraceTable)&&(!Error))
    {
        fprintf(listing ,"\nNow printing  symb table.....\n");
        PrintSymbTable();
        getchar();
        getchar();
    }
    /*输出含符号表信息的语法树*/
    if ((TraceParse)&&(!Error))
    {
        fprintf(listing,"after  analysis ..");
        fprintf(listing,"\nSyntax tree:\n\n");
        printTree(syntaxTree);
        getchar( );
    }
    return 0;
}
