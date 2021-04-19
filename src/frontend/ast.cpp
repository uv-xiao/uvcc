#include "ast.h"

NIdent::NIdent(const std::string &name) : name(name) {
#ifdef DEBUG_AST
    std::cout << "construct NIdent: " << name << std::endl;
#endif
}
NCond::NCond(NExpr &value) : value(value) {
#ifdef DEBUG_AST
    std::cout << "constuct NCond: " ;
    value.debug();
#endif
}
NBiExpr::NBiExpr(NExpr &lhs, int op, NExpr &rhs) : lhs(lhs), op(op), rhs(rhs) {
#ifdef DEBUG_AST
    std::cout << "construct NBiExpr: " << std::to_string(op) << std::endl;
#endif
}
NUExpr::NUExpr(int op, NExpr &rhs) : op(op), rhs(rhs) {
#ifdef DEBUG_AST
    std::cout << "construct NUExpr: " << std::to_string(op) << std::endl;
#endif
}
NFuncCall::NFuncCall(NIdent &name, NFuncRParams &args) : name(name), args(args) {}
NAssignStmt::NAssignStmt(NIdent &lhs, NExpr &rhs) : lhs(lhs), rhs(rhs) {
#ifdef DEBUG_AST
    std::cout << "construct NAssignStmt: " << lhs.name << std::endl;
#endif
}
NExprStmt::NExprStmt(NExpr &value) : value(value) {}
NIfStmt::NIfStmt(NCond &cond, NStmt &t, NStmt &e, bool has_else) : cond(cond), t(t), e(e), has_else(has_else) {
#ifdef DEBUG_AST  
    std::cout << "construct NIfStmt" << std::endl; 
#endif
}
NWhileStmt::NWhileStmt(NCond &cond, NStmt &d) : cond(cond), d(d) {}
NReturnStmt::NReturnStmt(NExpr *value) : value(value) {}
NDecl::NDecl(int type) : type(type) {}
NArrayIdent::NArrayIdent(NIdent &name) : name(name) {}
NArrayVal::NArrayVal(bool is_number, NExpr *value) : is_number(is_number), value(value) {}
NSingleDef::NSingleDef(NIdent &name, NExpr *value, bool is_const) : name(name), value(value), is_const(is_const) {}
NArrayDef::NArrayDef(NArrayIdent &name, NArrayVal *value, bool is_const)
: name(name), value(value), is_const(is_const) {}
NFuncFParam::NFuncFParam(int type, NIdent &name) : type(type), name(name) {}
NFuncDef::NFuncDef(int type, NIdent &name, NFuncFParams &args, NBlock &body)
: type(type), name(name), args(args), body(body){}

NNumber::NNumber(const std::string &value) {
    val = std::stoi(value, nullptr, 0);
}
NNumber::NNumber(int val) : val(val) {}