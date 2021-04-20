#ifndef UVCC_GENERATE_H
#define UVCC_GENERATE_H

#include <sstream>
#include <iostream>
#include <cstring>
#include <string>
#include "symtable.h"
#include "parser.tab.hpp"



std::string next_temp_var() ;
std::string current_temp_var() ;

extern SymTable ST;
extern SymTable localST;
extern std::string ir_global;

extern int tmp_var;
extern std::string return_var;
extern std::string return_label;
extern std::vector<std::string> while_start_label;
extern std::vector<std::string> while_exit_label;
extern std::string FALL;
extern std::string get_opposite_op(std::string oper);
extern std::string get_op(int op);
#endif //UVCC_GENERATE_H
