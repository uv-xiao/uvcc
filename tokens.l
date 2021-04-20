%{
#include <cstring>
#include <string>
#include "ast.h"
#include "parser.tab.hpp"

#include <cstdio>


#define TOKEN(tok)  (yylval.token = tok, tok)
#define SAVE(str)   yylval.string = new std::string(str)
#define SAVE_YY     yylval.string = new std::string(yytext, yyleng)
#define NEWLINE     yylineno += 1
#define PASS        for (auto i : std::string(yytext)) { if (i == '\n') NEWLINE; }
#define COMMENT     for (int c = yyinput(); c; c = yyinput()) {\
                        if (c == '\n') { \
                            yylineno += 1; \
                        } else { \
                            if (c == '*') { \
                                if ((c = yyinput()) == '/') break; \
                                else unput(c); \
                            } \
                        } \
                    }
#define BRACKET     *yy_cp = yy_hold_char; yy_hold_char = '('; yy_cp --; yyleng --; yy_c_buf_p --;
#define PUTCHAR(c)  *yy_c_buf_p = c; yy_c_buf_p --;
#define PUTLINE     PUTCHAR('_'); PUTCHAR('_'); PUTCHAR('E'); PUTCHAR('N'); PUTCHAR('I'); PUTCHAR('L'); PUTCHAR('_'); PUTCHAR('_');

%}


%%
  /*** comment, newline, blank ***/
\/\/.*\n?       {PASS;}
\/\*            {COMMENT;}
[ \t]             {}
\n              {NEWLINE;}

  /*** type ***/
"int"           {return TOKEN(INT);}
"void"          {return TOKEN(VOID);}

  /*** keyword ***/
"if"            {return TOKEN(IF);}
"else"          {return TOKEN(ELSE);}
"while"         {return TOKEN(WHILE);}
"break"         {return TOKEN(BREAK);}
"continue"      {return TOKEN(CONTINUE);}
"return"        {return TOKEN(RETURN);}

  /*** qualifier ***/
"const"         {return TOKEN(CONST);}

  /*** system call ***/
"putf"[ \t\n]*"(" {
                PASS;
                SAVE("printf");
                BRACKET;
                return IDENT;
            }
"starttime"[ \t\n]*"(" {
                PASS;
                SAVE("_sysy_startime"); 
                BRACKET;
                PUTLINE;
                return IDENT;
            }
"stoptime"[ \t\n]*"(" {
                PASS;
                SAVE("_sysy_startime"); 
                BRACKET;
                PUTLINE;
                return IDENT;
            }

  /*** number ***/
[0-9]+          {
                SAVE_YY;
                return INTVAL;
            } 
"0x"[0-9a-fA-F]+ {
                SAVE_YY;
                return INTVAL;
            }
"__LINE__"      {
                SAVE(std::to_string(yyget_lineno()));
                return INTVAL;
            }
"_SYSY_N"       {
                SAVE("1024");
                return INTVAL;
            }

  /*** identifier ***/
[a-zA-Z_][a-zA-Z0-9_]*  {
                SAVE_YY; 
                return IDENT;
            }

  /*** operators ***/
    /*** assign ***/
"="             {return TOKEN(ASSIGN);}
    /*** relaton ops ***/
"=="            {return TOKEN(EQ);}
"!="            {return TOKEN(NE);}
">"             {return TOKEN(GT);}
"<"             {return TOKEN(LT);}
">="            {return TOKEN(GE);}
"<="            {return TOKEN(LE);}

    /*** logic ops ***/
"&&"            {return TOKEN(AND);}
"||"            {return TOKEN(OR);}
"!"             {return TOKEN(NOT);}

    /*** arithmetic ops ***/
"+"             {return TOKEN(ADD);}
"-"             {return TOKEN(MINUS);}
"*"             {return TOKEN(MUL);}
"%"             {return TOKEN(MOD);}
"/"             {return TOKEN(DIV);}


  /*** symbols ***/
","             {return TOKEN(COMMA);}
";"             {return TOKEN(SEMI);}
"("             {return TOKEN(LP);}
")"             {return TOKEN(RP);}
"["             {return TOKEN(LS);}
"]"             {return TOKEN(RS);}
"{"             {return TOKEN(LB);}
"}"             {return TOKEN(RB);}

.               {
                std::string str = yytext;
                str = str + " ";
                for (auto x : std::string(yytext)) {
                    str = str + std::to_string((int)x) + " ";
                }
                std::cerr << "Unmatched text: " << str << std::endl;
                yyterminate();
            }            

%%

int yywrap() {
    return 1;
}
