#include "symtable.h"

int Symbol::count = 0;

Symbol::Symbol(std::string name, int type) : name(name), type(type) {
    if (type == VAR_INT || type == VAR_ARRAY) {
        Symbol::count += 1;
    }
    id = Symbol::count;
    length = 0;
    prefix = "T";
}

SymTable::SymTable(const SymTable &b) : symbols(b.symbols), label(b.label), prev(b.prev) {}

Symbol *SymTable::get(std::string name) {
    for (SymTable *table = this; table != nullptr; table = table->prev) {
        for (auto s_ptr : table->symbols) {
            if (s_ptr->name == name) {
                return s_ptr;
            }
        }
    }
    return nullptr;
}

Symbol *SymTable::add(std::string name, int type) {
    SymTable *table = this;
    Symbol *symbol = new Symbol(name, type);
    table->symbols.push_back(symbol);
    return symbol;
}

void SymTable::push() {
    // std::cout << "push table" << std::endl;
    SymTable *oldTable = new SymTable(*this);
    prev = oldTable;
    label = oldTable->label + 1;
    symbols.clear();
}

void SymTable::pop() {
    // std::cout << "pop table" << std::endl;
    SymTable *lastTable = prev;
    prev = lastTable->prev;
    symbols = lastTable->symbols;
    label = lastTable->label;
    delete lastTable;
}