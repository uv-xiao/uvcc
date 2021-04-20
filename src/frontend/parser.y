%{
#include "ast.h"
#include <cstdio>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

NRoot *root;

extern int yydebug;
extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();
extern void yyerror(const char *s);

#define YYERROR_VERBOSE true
#define YYDEBUG 1


%}

%union {
    int token;
    std::string* string;
    NIdent* ident;
    NCond* cond;
    NExpr* expr;
    NStmt* stmt;
    NDecl* decl;
    NDef* def;
    NFuncDef* funcdef;
    NFuncFParams* fparams;
    NFuncFParam* fparam;
    NFuncRParams* rparams;
    NBlock* block;
    NArrayVal* array_val;
    NArrayIdent* array_id;
    NRoot* root;
}

/* tokens */

/*** basics ***/
%token <string>         INTVAL IDENT
/*** keyword ***/
%token <token>          IF ELSE WHILE BREAK CONTINUE RETURN CONST INT VOID
/*** operator ***/
%token <token>          ASSIGN EQ NE GT LT GE LE AND OR NOT
%token <token>          ADD MINUS MUL DIV MOD
%token <token>          LP RP LS RS LB RB
%token <token>          COMMA SEMI

/* types */

%type <token>           AddOp MulOp RelOp UnaryOp EqOp BType 
%type <ident>           ident LVal
%type <cond>            Cond 
%type <expr>            Exp Number AddExp MulExp RelExp LOrExp LAndExp EqExp PrimaryExp UnaryExp FuncCall InitVal
%type <stmt>            Stmt AssignStmt ExprStmt IfStmt WhileStmt BreakStmt ContinueStmt ReturnStmt VoidStmt BlockItem
%type <decl>            Decl ConstDecl VarDecl ConstDeclStmt VarDeclStmt
%type <def>             Def DefSingle DefArray ConstDef ConstDefSingle ConstDefArray
%type <funcdef>         FuncDef
%type <fparams>         FuncFParams
%type <fparam>          FuncFParam FuncFParamArray FuncFParamSingle
%type <rparams>         FuncRParams
%type <block>           Block BlockItems
%type <array_val>       InitValArray InitValArrayInner
%type <array_id>      	ArrayName ArrayItem
%type <root>            CompUnit


/* Start */
%start CompUnit

%%

CompUnit:   CompUnit Decl   { $$->body.push_back($<decl>2); }
        |   CompUnit FuncDef    { $$->body.push_back($<funcdef>2);  }
        |   Decl    { root = new NRoot(); $$ = root; $$->body.push_back($<decl>1);   }
        |   FuncDef { root = new NRoot(); $$ = root; $$->body.push_back($<funcdef>1);  }
        ;

BType:  INT;

Decl:   ConstDeclStmt
    |   VarDeclStmt
    ;

/* const declaration statement */
ConstDeclStmt:  ConstDecl SEMI  { $$ = $1; };

ConstDecl:  CONST BType ConstDef    { $$ = new NDecl($2); $$->list.push_back($3); } 
         |  ConstDecl COMMA ConstDef    { $$->list.push_back($3); }
         ;

ConstDef:   ConstDefSingle
        |   ConstDefArray
        ;

ConstDefSingle:  ident ASSIGN InitVal    { $$ = new NSingleDef(*$1, $3, true); };

ConstDefArray:  ArrayName ASSIGN InitValArray   { $$ = new NArrayDef(*$1, $3, true); };

/* general declaration statement */
VarDeclStmt:    VarDecl SEMI    { $$ = $1;  };

VarDecl:    BType Def   { $$ = new NDecl($1); $$->list.push_back($2); }
       |    VarDecl COMMA Def   { $$->list.push_back($3); }
       ;

Def:    DefSingle
   |    DefArray
   ;

DefSingle:  ident ASSIGN InitVal    { $$ = new NSingleDef(*$1, $3); }
         |  ident   { $$ = new NSingleDef(*$1); }
         ;
DefArray:   ArrayName ASSIGN InitValArray   { $$ = new NArrayDef(*$1, $3); }
        |   ArrayName   { $$ = new NArrayDef(*$1); }
        ;

/* array item and val */
ArrayName:  ident LS Exp RS    { $$ = new NArrayIdent(*$1); $$->shape.push_back($3); }
         |  ArrayName LS Exp RS   { $$ = $1; $$->shape.push_back($3); } 
         ;

InitValArray:   LB InitValArrayInner RB { $$ = $2; }
            |   LB RB   { $$ = new NArrayVal(false, nullptr); }
            ;

InitValArrayInner:  InitVal { $$ = new NArrayVal(false, nullptr); $$->list.push_back(new NArrayVal(true, $1)); }
                 |  InitValArray    { $$ = new NArrayVal(false, nullptr); $$->list.push_back($1); }
                 |  InitValArrayInner COMMA InitVal { $$ = $1; $$->list.push_back(new NArrayVal(true, $3)); }
                 |  InitValArrayInner COMMA InitValArray    { $$ = $1; $$->list.push_back($3); }
                 ;

/* number, item */
Number: INTVAL  { $$ = new NNumber(*$1); };

ident:  IDENT   { $$ = new NIdent(*$1); }

LVal:   ArrayItem
    |   ident
    ;

ArrayItem:  ident LS Exp RS { $$ = new NArrayIdent(*$1); $$->shape.push_back($3); }
         |  ArrayItem LS Exp RS { $$ = $1; $$->shape.push_back($3); }
         ;

/* symbols */ 
AddOp:  ADD
     |  MINUS
     ;
MulOp:  MUL
     |  DIV
     |  MOD
     ;
UnaryOp:    ADD
       |    MINUS
       |    NOT
       ;
RelOp:  GT
     |  LT
     |  GE
     |  LE
     ;
EqOp:   EQ
    |   NE
    ;

/* expression */
InitVal:    AddExp;

Exp:    AddExp;

PrimaryExp: LP Exp RP  { $$ = $2; }
          | LVal
          | Number
          ;

UnaryExp:   UnaryOp UnaryExp    { $$ = new NUExpr($1, *$2); }
        |   PrimaryExp
        |   FuncCall
        ;

MulExp: MulExp MulOp UnaryExp   { $$ = new NBiExpr(*$1, $2, *$3); }
      | UnaryExp
      ;

AddExp: AddExp AddOp MulExp { $$ = new NBiExpr(*$1, $2, *$3); }
      | MulExp
      ;

RelExp: RelExp RelOp AddExp { $$ = new NBiExpr(*$1, $2, *$3); }
      | AddExp
      ;

EqExp:  RelExp EqOp RelExp  { $$ = new NBiExpr(*$1, $2, *$3); }
     |  RelExp
     ;

LAndExp:    LAndExp AND EqExp  { $$ = new NBiExpr(*$1, $2, *$3); }
       |    EqExp
       ;

LOrExp: LOrExp OR LAndExp   { $$ = new NBiExpr(*$1, $2, *$3); }
      | LAndExp OR LAndExp  { $$ = new NBiExpr(*$1, $2, *$3); }
      | LAndExp
      ;

Cond:   LOrExp  { $$ = new NCond(*$1); }
    ;

/* function */
/*** function declaration ***/

FuncDef:    BType ident LP FuncFParams RP Block    { $$ = new NFuncDef($1, *$2, *$4, *$6); }
       |    BType ident LP RP Block { $$ = new NFuncDef($1, *$2, *(new NFuncFParams()), *$5); } 
       |    VOID ident LP FuncFParams RP Block    { $$ = new NFuncDef($1, *$2, *$4, *$6); }
       |    VOID ident LP RP Block { $$ = new NFuncDef($1, *$2, *(new NFuncFParams()), *$5); } 
       ;

FuncFParams:    FuncFParams COMMA FuncFParam    { $$->list.push_back($3); }
           |    FuncFParam  { $$ = new NFuncFParams(); $$->list.push_back($1); }
           ;

FuncFParam: FuncFParamSingle
          | FuncFParamArray
          ;

FuncFParamSingle:   BType ident { $$ = new NFuncFParam($1, *$2); };

FuncFParamArray:    FuncFParamArray LS Exp RS { $$ = $1; ((NArrayIdent *)&($$->name))->shape.push_back($3); }
               |    FuncFParamSingle LS RS {
                        $$ = new NFuncFParam($1->type, *new NArrayIdent($1->name));
                        ((NArrayIdent *)&($$->name))->shape.push_back(new NNumber(1));
                        delete $1;
                   }
               ;

/*** function call ***/

FuncCall:   ident LP FuncRParams RP { $$ = new NFuncCall(*$1, *$3); }
            |   ident LP RP { $$ = new NFuncCall(*$1, *(new NFuncRParams())); }
            ;

FuncRParams:    FuncRParams COMMA AddExp { $$ = $1; $$->list.push_back($3); }
           |    AddExp  { $$ = new NFuncRParams(); $$->list.push_back($1); }
           ;

/* statement */
Stmt:   AssignStmt
    |   ExprStmt
    |   Block
    |   IfStmt
    |   WhileStmt
    |   BreakStmt
    |   ContinueStmt
    |   ReturnStmt
    |   VoidStmt
    ;

AssignStmt: LVal ASSIGN AddExp SEMI { $$ = new NAssignStmt(*$1, *$3); };
ExprStmt:    Exp SEMI    { $$ = new NExprStmt(*$1); }

IfStmt: IF LP Cond RP Stmt  { $$ = new NIfStmt(*$3, *$5, *new NVoidStmt(), 0); }
      | IF LP Cond RP Stmt ELSE Stmt    { $$ = new NIfStmt(*$3, *$5, *$7, 1); }
      ;

WhileStmt:  WHILE LP Cond RP Stmt   { $$ = new NWhileStmt(*$3, *$5); };
BreakStmt:  BREAK SEMI  { $$ = new NBreakStmt(); };
ContinueStmt:   CONTINUE SEMI   { $$ = new NContinueStmt(); };

ReturnStmt: RETURN Exp SEMI { $$ = new NReturnStmt($2); } 
          | RETURN SEMI { $$ = new NReturnStmt(); }
          ;

VoidStmt:   SEMI    { $$ = new NVoidStmt(); }

/* block */

Block:  LB RB   { $$ = new NBlock(); }
     |  LB BlockItems RB    { $$ = $2; }
     ;

BlockItems: BlockItems BlockItem    { $$ = $1; $$->stmts.push_back($2); }
          | BlockItem   { $$ = new NBlock(); $$->stmts.push_back($1); }
          ;

BlockItem:  Decl
         |  Stmt
         ;

%%

void yyerror(const char *s) {
    printf("Error(line %d): %s\n", yyget_lineno(), s);
    yylex_destroy();
    if (!yydebug) exit(1);
}


std::string get_op(int op) {
    switch (op) {
        case ADD: return "+";
        case MUL: return "*";
        case MINUS: return "-";
        case DIV: return "/";
        case MOD: return "%";
        case GT: return ">";
        case LT: return "<";
        case GE: return ">=";
        case LE: return "<=";
        case EQ: return "==";
        case NE: return "!=";
        case AND: return "&&";
        case OR: return "||";
        case NOT: return "!";
        default: return "F**K";
    }
}
std::string get_opposite_op(std::string oper) {
    if (oper == "==") return "!=";
    else return "==";
}