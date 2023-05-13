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
    return 0;
}
