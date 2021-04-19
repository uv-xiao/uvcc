#ifndef UVCC_AST_H
#define UVCC_AST_H

#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include "symtable.h"

class Node {
public:
    virtual ~Node() = default;
    virtual void generate_global();
};

class NExpr : public Node {
public:
    virtual void debug();
    virtual int eval();
    virtual std::string generate(std::stringstream &buff);
    virtual std::string generate_lval(std::stringstream &buff);
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
};
class NStmt : public Node {
public:
    virtual void generate_global(std::stringstream &buff);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NDef : public Node {
public:
    virtual void generate_global(std::stringstream &buff);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};

/* expression class*/
class NNumber : public NExpr{
public:
    int val;
    NNumber(const std::string &value);
    NNumber(int val);
    virtual std::string generate(std::stringstream &buff);
    virtual int eval();
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
};
class NIdent : public NExpr {
public:
    std::string name;
    NIdent() = default;
    NIdent(const std::string &name);
    virtual std::string generate(std::stringstream &buff);
    virtual std::string generate_lval(std::stringstream &buff);
    virtual void generate_fparam(Symbol *func);
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
    virtual int eval();
};
class NCond : public NExpr {
public:
    NExpr &value;
    NCond(NExpr &value);
    void generate(std::stringstream &buff, std::string true_label, std::string false_label);
};
class NBiExpr : public NExpr {
public:
    int op;
    NExpr &lhs, &rhs;
    NBiExpr(NExpr &lhs, int op, NExpr &rhs);
    virtual std::string generate(std::stringstream &buff);
    virtual void debug();
    virtual int eval();
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
};

class NUExpr : public NExpr {
public:
    int op;
    NExpr &rhs;
    NUExpr(int op, NExpr &rhs);
    virtual std::string generate(std::stringstream &buff);
    virtual int eval();
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
};

class NFuncRParams : public Node {
public:
    std::vector<NExpr*> list;
    virtual std::vector<std::string> generate(std::stringstream &buff);
};

class NFuncCall : public NExpr {
public:
    NIdent &name;
    NFuncRParams &args;
    NFuncCall(NIdent &name, NFuncRParams &args);
    virtual std::string generate(std::stringstream &buff);
    virtual void generate_cond(std::stringstream &buff, std::string true_label, std::string false_label);
};

/* statement class*/
class NBlock : public NStmt {
public:
    std::vector<NStmt *> stmts;
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NAssignStmt : public NStmt {
public:
    NIdent &lhs;
    NExpr  &rhs;
    NAssignStmt(NIdent &lhs, NExpr &rhs);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NExprStmt : public NStmt {
public:
    NExpr &value;
    NExprStmt(NExpr &value);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NIfStmt : public NStmt {
public:
    NCond &cond;
    NStmt &t, &e;
    bool has_else;
    NIfStmt(NCond &cond, NStmt &t, NStmt &e, bool has_else);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NWhileStmt : public NStmt {
public:
    NCond &cond;
    NStmt &d;
    NWhileStmt(NCond &cond, NStmt &d);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NBreakStmt : public NStmt {
public:
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NContinueStmt : public NStmt {
public:
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NReturnStmt : public NStmt {
public:
    NExpr *value;
    NReturnStmt(NExpr *value = nullptr);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NVoidStmt : public NStmt {
public:
//    virtual void generate_global(std::stringstream &buff);
//    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
/*** declaration is also statement ***/
class NDecl : public NStmt {
public:
    std::vector<NDef *> list;
    int type;
    NDecl(int type);
    virtual void generate_global();
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};

/* array */
class NArrayIdent : public NIdent {
public:
    NIdent &name;
    std::vector<NExpr *> shape;
    NArrayIdent(NIdent &name);
    virtual void set_symbol(Symbol *symbol);
    virtual std::string generate(std::stringstream &buff);
    virtual std::string generate_lval(std::stringstream &buff);
    virtual void generate_fparam(Symbol *func);
};

class NArrayVal : public NExpr {
public:
    bool is_number;
    NExpr *value;
    std::vector<NArrayVal *> list;
    NArrayVal(bool is_number, NExpr *value);
    virtual void generate(std::stringstream &buff, Symbol *symbol, int begin, int length, int dim, int depth);
};

/* definition */
class NSingleDef : public NDef {
public:
    NIdent &name;
    NExpr *value;
    bool is_const;
    NSingleDef(NIdent &name, NExpr *value=nullptr, bool is_const=false);
    virtual void generate_global(std::stringstream &buff);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};
class NArrayDef : public NDef {
public:
    NArrayIdent &name;
    NArrayVal *value;
    bool is_const;
    NArrayDef(NArrayIdent &name, NArrayVal *value=nullptr, bool is_const=false);
    virtual void generate_global(std::stringstream &buff);
    virtual void generate_local(std::stringstream &decl_b, std::stringstream &stmt_b);
};

/* function */
class NFuncFParam : public Node {
public:
    int type;
    NIdent &name;
    NFuncFParam(int type, NIdent &name);
    virtual void generate(Symbol *func);
};
class NFuncFParams : public Node {
public:
    std::vector<NFuncFParam*> list;
    virtual void generate(Symbol *func);
};
class NFuncDef : public Node {
public:
    int type;
    NIdent &name;
    NFuncFParams &args;
    NBlock &body;
    NFuncDef(int type, NIdent &name, NFuncFParams &args, NBlock &body);
    virtual void generate_global();
};

/* root */
class NRoot : public Node {
public:
    std::vector<Node*> body;
    virtual void generate();
};

extern NRoot *root;

#endif //UVCC_AST_H
