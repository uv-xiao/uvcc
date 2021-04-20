#include "ast.h"
#include "generate.h"

SymTable ST;
SymTable localST;
std::string ir_global;

int tmp_var = 0;
int label_count = 0;

std::string next_temp_var() {
    std::cout << "var t" << ++ tmp_var << std::endl;
    return std::string("t") + std::to_string(tmp_var);
}

std::string current_temp_var() {
    return std::string("t") + std::to_string(tmp_var);
}

std::string next_label() {
    return std::string("l") + std::to_string(++ label_count);
}

Symbol *get_symbol_from_name(std::string name) {
    Symbol *symbol = localST.get(name);
    if (symbol == NULL) {
        symbol = ST.get(name);
    }
    return symbol;
}

void gen_control_flow(std::stringstream &buff, std::string laddr, std::string oper, std::string raddr,
                      std::string true_label, std::string false_label) {
    if (true_label != FALL && false_label != FALL) {
        buff << "if " << laddr << " " << oper << " " << raddr << " goto " << true_label << std::endl;
        buff << "goto " << false_label << std::endl;
    }
    else
    if (true_label != FALL) {
        buff << "if " << laddr << " " << oper << " " << raddr << " goto " << true_label << std::endl;
    }
    else {
        buff << "if " << laddr << " " << get_opposite_op(oper) << " " << raddr <<
             " goto " << false_label << std::endl;
    }
}

void Node::generate_global() {}
void NRoot::generate() {
    #ifdef DEBUG
    std::cout << "generate NRoot" << std::endl;
    #endif
    Symbol *symbol;
    ST.add(std::string("getint"), FUNC_INT);
    symbol = ST.add(std::string("putint"), FUNC_VOID);
    symbol->arg_list.push_back(ARG_INT);
    symbol = ST.add(std::string("putch"), FUNC_VOID);
    symbol->arg_list.push_back(ARG_INT);

    for (auto bd: body) {
        bd->generate_global();
    }
}

void NStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {}
void NStmt::generate_global(std::stringstream &buff) {}

int NExpr::eval() { return 0;}
std::string NExpr::generate(std::stringstream &buff) { return "";}
std::string NExpr::generate_lval(std::stringstream &buff) { return "";}
void NExpr::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {}
void NExpr::debug() { std::cout << "NExpr" << std::endl; }

void NDecl::generate_global() {
    #ifdef DEBUG
    std::cout << "generate NDecl globally" << std::endl;
    #endif
    for (auto def: list) {
        std::stringstream line;
        def->generate_global(line);
        ir_global = ir_global + line.str() + "\n";
    }
}
void NDecl::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate NDecl locally" << std::endl;
    #endif
    for (auto x : list) {
        x->generate_local(decl_b, stmt_b);
    }
}

void NDef::generate_global(std::stringstream &buff) {}
void NDef::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {}

void NSingleDef::generate_global(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NSingleDef globally" << std::endl;
    #endif
    Symbol *symbol = ST.add(name.name, VAR_INT);
    std::cout << "var " << symbol->__str__() << std::endl;
    if (value != nullptr) {
        int const_val = value->eval();
        symbol->value = const_val;
        buff << symbol->__str__() << " = " << const_val << std::endl;
    }
}
void NSingleDef::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate NSingleDef locally" << std::endl;
    #endif
    Symbol *symbol = localST.add(name.name, VAR_INT);
    decl_b << "var " << symbol->__str__() << std::endl;
    if (value != nullptr) {
        std::string addr = value->generate(stmt_b);
        stmt_b << symbol->__str__() << " = " << addr << std::endl;
    }
}

void NArrayDef::generate_global(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NArrayDef globally" << std::endl;
    #endif
    Symbol *symbol = ST.add(name.name.name, VAR_ARRAY);
    name.set_symbol(symbol);
    std::cout << "var " << 4 * symbol->length << " " << symbol->__str__() << std::endl;
    if (value != nullptr) {
        value->generate(buff, symbol, 0, symbol->length / symbol->shape[0], symbol->shape[0], 0 );
    }
}
void NArrayDef::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate NArrayDef locally" << std::endl;
    #endif

    Symbol *symbol = localST.add(name.name.name, VAR_ARRAY);
    name.set_symbol(symbol);
    decl_b << "var " << symbol->length * 4 << " " << symbol->__str__() << std::endl;
    if (value != nullptr) {
        value->generate(stmt_b, symbol, 0, symbol->length / symbol->shape[0], symbol->shape[0], 0);
    }
}

std::string NIdent::generate(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NIdent" << std::endl;
    #endif
    return get_symbol_from_name(name)->__str__();
}
std::string NIdent::generate_lval(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NIdent lval" << std::endl;
    #endif
    return get_symbol_from_name(name)->__str__();
}
void NIdent::generate_fparam(Symbol *func) {
    func->arg_list.push_back(ARG_INT);
    Symbol *symbol = localST.add(name, VAR_INT);
    symbol->prefix = "p";
    symbol->id = func->arg_list.size() - 1;
}
void NIdent::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {
    std::string oper = "!=";
    std::string laddr = generate(buff);
    std::string raddr = "0";
    gen_control_flow(buff, laddr, oper, raddr, true_label, false_label);
}
int NIdent::eval() {
    return get_symbol_from_name(name)->value;
}

void NArrayIdent::set_symbol(Symbol *symbol) {
    symbol->length = 1;
    for (auto x : shape) {
        int val = x->eval();
        symbol->shape.push_back(val);
        symbol->length *= val;
    }
}
std::string NArrayIdent::generate(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NIdent" << std::endl;
    #endif
    Symbol *symbol = localST.get(name.name);
    if (symbol == nullptr) {
        symbol = ST.get(name.name);
    }

    std::string sum = "0";
    int offset = 1, dim = shape.size();
    for (int i = 0; i < dim; i++) {
        std::string addr1 = shape[dim - i - 1]->generate(buff);
        std::string addr2 = next_temp_var();
        buff << addr2 << " = " << addr1 << " * " << offset << std::endl;
        offset *= symbol->shape[dim - i - 1];
        buff << next_temp_var() << " = " << addr2 << " + " << sum << std::endl;
        sum = current_temp_var();
    }

    buff << next_temp_var() << " = " << sum << " * 4" << std::endl;
    std::string raddr = get_symbol_from_name(name.name)->__str__() + "[" + current_temp_var() + "]";
    std::string laddr = next_temp_var();
    buff << laddr << " = " << raddr << std::endl;
    return laddr;
}
std::string NArrayIdent::generate_lval(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NArrayIdent Lval" << std::endl;
    #endif
    Symbol *symbol = localST.get(name.name);
    if (symbol == nullptr) {
        symbol = ST.get(name.name);
    }

    std::string sum = "0";
    int offset = 1, dim = shape.size();
    for (int i = 0; i < dim; i++) {
        std::string addr1 = shape[dim - i - 1]->generate(buff);
        std::string addr2 = next_temp_var();
        buff << addr2 << " = " << addr1 << " * " << offset << std::endl;
        offset *= symbol->shape[dim - i - 1];
        buff << next_temp_var() << " = " << addr2 << " + " << sum << std::endl;
        sum = current_temp_var();
    }

    buff << next_temp_var() << " = " << sum << " * 4" << std::endl;
    std::string addr = get_symbol_from_name(name.name)->__str__() + "[" + current_temp_var() + "]";
    return addr;
}
void NArrayIdent::generate_fparam(Symbol *func) {
    func->arg_list.push_back(ARG_ARRAY);
    Symbol *symbol = localST.add(name.name, VAR_ARRAY);
    symbol->prefix = "p";
    symbol->id = func->arg_list.size() - 1;
    symbol->shape.push_back(-1);
    for (auto x : shape) {
        symbol->shape.push_back(x->eval());
    }
}

void NArrayVal::generate(std::stringstream &buff, Symbol *symbol, int offset, int length, int dim, int depth) {
    int count = 0;
    std::string name = symbol->__str__();

    for (NArrayVal *item : list) {
        if (item->is_number) {
            std::string addr = item->value->generate(buff);
            buff << name << "[" << 4 * (offset + count) << "] = " << addr << std::endl;
            count += 1;
        } else {
            while (count % length != 0) {
                buff << name << "[" << 4 * (offset + count) << "] = 0\n";
                count += 1;
            }
            item->generate(buff, symbol, offset+count, length / symbol->shape[depth + 1],
                                 symbol->shape[depth + 1], depth + 1);
            count += length;
        }
    }
    while (count < dim * length) {
        buff << name << "[" << 4 * (offset + count) << "] = 0\n";
        count += 1;
    }
}

std::string NBiExpr::generate(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NBiExpr: " << get_op(op) << std::endl;
    #endif
    std::string var = next_temp_var();
    std::string laddr = lhs.generate(buff);
    std::string raddr = rhs.generate(buff);
    buff << var << " = " << laddr << " " << get_op(op) << " " << raddr << "\n";
    return var;
}
int NBiExpr::eval() {
    switch (op) {
        case ADD:   return lhs.eval() + rhs.eval();
        case MINUS: return lhs.eval() - rhs.eval();
        case MUL:   return lhs.eval() * rhs.eval();
        case DIV:   return lhs.eval() / rhs.eval();
        case MOD:   return lhs.eval() % rhs.eval();
        case AND:   return lhs.eval() && rhs.eval();
        case OR:    return lhs.eval() || rhs.eval();
        case GT:    return lhs.eval() > rhs.eval();
        case LT:    return lhs.eval() < rhs.eval();
        case GE:    return lhs.eval() >= rhs.eval();
        case LE:    return lhs.eval() <= rhs.eval();
        case EQ:    return lhs.eval() == rhs.eval();
        case NE:    return lhs.eval() != rhs.eval();
        default:    return 0;
    }
}
void NBiExpr::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {
    if (op == OR) {
        std::string l_true = (true_label != FALL) ? true_label : next_label();
        std::string l_false = FALL;

        lhs.generate_cond(buff, l_true, l_false);
        rhs.generate_cond(buff, true_label, false_label);

        if (true_label == FALL) {
            buff << l_true << ":" << std::endl;
        }
    }
    else
    if (op == AND) {
        std::string l_true = FALL;
        std::string l_false = (false_label != FALL) ? false_label : next_label();
        lhs.generate_cond(buff, l_true, l_false);
        rhs.generate_cond(buff, true_label, l_false);
        if (false_label == FALL) {
            buff << l_false << ":" << std::endl;
        }
    }
    else {
        std::string oper, laddr, raddr;
        if (op == EQ || op == NE) {
            oper = get_op(op);
            laddr = lhs.generate(buff);
            raddr = rhs.generate(buff);
        }
        else {
            oper = "!=";
            laddr = generate(buff);
            raddr = "0";
        }
        gen_control_flow(buff, laddr, oper, raddr, true_label, false_label);
    }
}
void NBiExpr::debug() {
    std::cout << "NBiExpr " << get_op(op) << std::endl;
}

std::string NUExpr::generate(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NUExpr: " << get_op(op) << std::endl;
    #endif
    std::string var = next_temp_var();
    std::string addr = rhs.generate(buff);
    std::string oper = op == ADD ? "" : get_op(op);
    buff << var << " = " << oper << " " << addr << std::endl;
    return var;
}
int NUExpr::eval() {
    switch (op) {
        case ADD:   return rhs.eval();
        case MINUS: return -rhs.eval();
        case NOT:   return !rhs.eval();
        default:    return rhs.eval();
    }
}
void NUExpr::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {
    if (op == NOT) {
        rhs.generate_cond(buff, false_label, true_label);
    }
    else {
        std::string oper, laddr, raddr;
        oper = "!=";
        laddr = generate(buff);
        raddr = "0";
        gen_control_flow(buff, laddr, oper, raddr, true_label, false_label);
    }
}

std::string NNumber::generate(std::stringstream &buff) {
    std::string var = next_temp_var();
    buff << var << " = " << val << std::endl;
    return var;
}
int NNumber::eval() {
    return val;
}
void NNumber::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {
    std::string oper = "!=";
    std::string laddr = generate(buff);
    std::string raddr = "0";
    gen_control_flow(buff, laddr, oper, raddr, true_label, false_label);
}

std::string NFuncCall::generate(std::stringstream &buff) {
    #ifdef DEBUG
    std::cout << "generate NFuncCall: " << name.name << std::endl;
    #endif
    std::vector<std::string> args_str = args.generate(buff);
    Symbol *func = ST.get(name.name);
    for (auto x : args_str) {
        buff << "param " << x << std::endl;
    }
    if (func->type == FUNC_INT) {
        buff << next_temp_var() << " = ";
    }
    buff << "call f_" << name.name << "\n";
    return current_temp_var();
}
void NFuncCall::generate_cond(std::stringstream  &buff, std::string true_label, std::string false_label) {
    std::string oper = "!=";
    std::string laddr = generate(buff);
    std::string raddr = "0";
    gen_control_flow(buff, laddr, oper, raddr, true_label, false_label);
}

std::vector<std::string> NFuncRParams::generate(std::stringstream &buff) {
    std::vector<std::string> args;
    for (auto x : list) {
        std::string addr = x->generate(buff);
        args.push_back(addr);
    }
    return args;
}

std::string return_var;
std::string return_label;

void NFuncDef::generate_global() {
    #ifdef DEBUG
    std::cout << "generate NFuncDef: " << name.name << std::endl;
    #endif
    return_label = next_label();
    Symbol *func;
    if (type == INT) {
        func = ST.add(name.name, FUNC_INT);
        return_var = next_temp_var();
    }
    else {
        func = ST.add(name.name, FUNC_VOID);
        return_var = "";
    }
    localST.push();
    args.generate(func);
    if (name.name == "main") {
        std::cout << ir_global;
    }

    std::stringstream decl;
    std::stringstream stmt;
    for (auto x : body.stmts) {
        x->generate_local(decl, stmt);
    }
    stmt << return_label << ":" << std::endl;
    stmt << "return " << return_var << std::endl;
    std::cout << decl.str() << std::endl;
    std::cout << stmt.str() << std::endl;
    std::cout << "end f_" << func->name << std::endl;

    localST.pop();
}

void NFuncFParams::generate(Symbol *func) {
    std::cout << "f_" << func->name << " [" << list.size() << "]" << std::endl;
    for (auto param : list) {
        param->generate(func);
    }
}

void NFuncFParam::generate(Symbol *func) {
    name.generate_fparam(func);
}

void NBlock::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate Block" << std::endl;
    #endif
    localST.push();
    for (auto x : stmts) {
        x->generate_local(decl_b, stmt_b);
    }
    localST.pop();
}

void NAssignStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {

    std::string laddr = lhs.generate_lval(stmt_b);
    std::string raddr = rhs.generate(stmt_b);
    stmt_b << laddr << " = " << raddr << std::endl;
    #ifdef DEBUG
    std::cout << "generate NAssignStmt: " << laddr << " = " << raddr << std::endl;
    #endif
}

void NExprStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    value.generate(stmt_b);
}

void NCond::generate(std::stringstream &buff, std::string true_label, std::string false_label) {
    value.generate_cond(buff, true_label, false_label);
}
void NCond::generate_cond(std::stringstream &buff, std::string true_label, std::string false_label) {
    value.generate_cond(buff, true_label, false_label);
}
std::string NCond::generate(std::stringstream &buff) {
    return value.generate(buff);
}
std::string NCond::generate_lval(std::stringstream &buff) {
    return value.generate(buff);
}


std::string FALL = ".fall";

void NIfStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate NIfStmt" << std::endl;
    #endif
    std::string then_label = next_label();
    std::string else_label = next_label();
    std::string exit_label = next_label();

    if (has_else) {
        cond.generate(stmt_b, FALL, else_label);
        t.generate_local(decl_b, stmt_b);
        stmt_b << "goto " << exit_label << std::endl;
        stmt_b << else_label << ":" << std::endl;
        e.generate_local(decl_b, stmt_b);
    }
    else {
        cond.generate(stmt_b, FALL, exit_label);
        t.generate_local(decl_b, stmt_b);
    }

    stmt_b << exit_label << ":" << std::endl;
}

std::vector<std::string> while_start_label;
std::vector<std::string> while_exit_label;

void NWhileStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    #ifdef DEBUG
    std::cout << "generate NWhileStmt" << std::endl;
    #endif
    std::string start_label = next_label();
    std::string do_label = next_label();
    std::string exit_label = next_label();
    while_start_label.push_back(start_label);
    while_exit_label.push_back(exit_label);

    stmt_b << start_label << ":" << std::endl;
    cond.generate(stmt_b, FALL,  exit_label);
    d.generate_local(decl_b, stmt_b);
    stmt_b << "goto " << start_label << std::endl;
    stmt_b << exit_label << ":" << std::endl;

    while_start_label.pop_back();
    while_exit_label.pop_back();
}

void NBreakStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    stmt_b << "goto " << while_exit_label.back() << std::endl;
}

void NContinueStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    stmt_b << "goto " << while_start_label.back() << std::endl;
}

void NReturnStmt::generate_local(std::stringstream &decl_b, std::stringstream &stmt_b) {
    if (value != nullptr) {
        std::string addr = value->generate(stmt_b);
        stmt_b << return_var << " = " << addr << std::endl;
    }
    stmt_b << "goto " << return_label << std::endl;
}