#ifndef UVCC_SYMTABLE_H
#define UVCC_SYMTABLE_H
#include <iostream>
#include <cstring>
#include <string>
#include <vector>


enum SymType { VAR_INT, VAR_ARRAY, FUNC_INT, FUNC_VOID };
enum ArgType { ARG_INT, ARG_ARRAY };

class Symbol {
public:
    static int count;
    std::string name;
    std::string prefix;
    int id, type, value, length;
    std::vector<int> shape;
    std::vector<int> arg_list;
    std::string __str__() {
        return prefix + std::to_string(id);
    }

    Symbol() = default;
    Symbol(std::string name, int type);
};

class SymTable {
private:
    std::vector<Symbol *> symbols;
    SymTable *prev;
    int label;

public:
    SymTable() = default;
    SymTable(const SymTable &b);
    Symbol *get(std::string name) ;
    Symbol *add(std::string name, int type) ;
    void push();
    void pop();
};


#endif //UVCC_SYMTABLE_H