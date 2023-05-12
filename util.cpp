//
// Created by long on 2023/5/12.
//

#include "util.h"


//extern FILE *fp;

int fp_num = 0; //输出时记录token的数量

/*
 * 函数名：printTokenlist()
 * 功能：将文件tokenlist中的信息作为返回值输出
 * 说明：用于显示词法分析的结果
 * */
void printTokenlist(){
    TokenType token;
    std::string filename = "Tokenlist";
    fp = fopen(filename.c_str(), "rb");
    if(fp == NULL)
    {
        std::cout<<"Can't open the file "+filename+"!\n";
        Error = TRUE;
    }
    for (int i = 1; i <= Tokennum; ++i) {
        fread(&token,TOKENLEN,1,fp);
        fprintf(listing,"\t%d: ",token.Lineshow);//打印行号

        /*对函数参数Lex给定单词进行分类处理*/
        switch (token.Lex)
        {
            /* 单词为保留字,将保留字词元写入列表文件listing */
            case PROGRAM:
            case PROCEDURE:
            case TYPE:
            case VAR:
            case IF:
            case THEN:
            case ELSE:
            case FI:
            case INTEGER:
            case CHAR:
            case WHILE:
            case DO:
            case ENDWH:
            case BEGIN:
            case END:
            case READ:
            case WRITE:
            case ARRAY:
            case OF:
            case RECORD:
            case RETURN:
                fprintf(listing,"reserved word: %s\n",token.Sem);
                break;

            case ASSIGN: fprintf(listing,":=\n"); break;

            case LT: fprintf(listing,"<\n"); break;

            case EQ: fprintf(listing,"=\n"); break;

            case LPAREN: fprintf(listing,"(\n"); break;

            case RPAREN: fprintf(listing,")\n"); break;

            case SEMI: fprintf(listing,";\n"); break;

            case PLUS: fprintf(listing,"+\n"); break;

            case MINUS: fprintf(listing,"-\n"); break;

            case TIMES: fprintf(listing,"*\n"); break;

            case OVER: fprintf(listing,"/\n");  break;

            case DOT:  fprintf(listing,".\n"); break;

            case COMMA: fprintf(listing,",\n"); break;

            case LMIDPAREN: fprintf(listing,"[\n"); break;

            case RMIDPAREN:
                fprintf(listing,"]\n");
                break;

            case UNDERANGE:
                fprintf(listing,"..\n");
                break;

            case ENDFILE:
                fprintf(listing,"EOF\n");
                break;

            case INTC:
                fprintf(listing,"NUM, val = %s\n",token.Sem);
                break;

            case CHARC:
                fprintf(listing,"INCHAR, char = %c\n",token.Sem);
                break;

            case ID:
                fprintf(listing,"ID, name = %s\n",token.Sem);
                break;

            case ERROR:
                fprintf(listing,"Error: %s\n",token.Sem);
                break;
            default:
                fprintf(listing,"Unknown token: %d\n",token.Lex);
                break;
        }
        fseek(fp,i*TOKENLEN,0);
    }
    fprintf(listing,"\n");
    fclose(fp);
}


/*函数名：ChainToFile(ChainNodeType *p)
 * 功能：将链表中的Token以此写入文件中
 * 说明：参数Chainhead是Token链表的表头 */
void ChainToFile(ChainNodeType *Chainhead){
    int num = 1;
    ChainNodeType  *cur = Chainhead;
    std::string filename = "Tokenlist";
    fp = fopen(filename.c_str(),"wb+");
    if(!fp)
    {
        std::cout<<"Cannot create file Tokenlist!\n";
        Error = TRUE;
    }
    fp = fopen(filename.c_str(),"ab");
    if(!fp)
    {
        std::cout<<"Cannot open file Tokenlist!\n";
        Error = TRUE;
    }
    do{
        fwrite(cur,TOKENLEN,1,fp);
        cur = cur->nextToken;
        num++;
    }while (cur = nullptr);
    fclose(fp);
}