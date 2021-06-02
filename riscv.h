#ifndef _RISCV_H
#define _RISCV_H


#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

namespace riscv {

using stringset = std::set<std::string>;
using stringmap = std::map<std::string, std::string>;
using stringvec = std::vector<std::string>;

extern int DEBUG_FLAG;
extern stringset reg_names;
extern stringmap expr_ops, cond_ops, goto_ops, rimm_ops;
extern std::string emptyReg;

class RISCVGen {
private:
  int stackSize;
  int labelCount;
  stringvec lines;
  const std::string LabelPref = ".Lbl";
  stringvec _split(const std::string &str, char sep=' ');
  
  inline bool _substr(const std::string &str, const std::string &sub) {
    return str.find(sub) != std::string::npos;
  }
  inline bool _inSet(const stringset &Set, const std::string &str) {
      return Set.count(str) > 0;
  }
  inline bool _inMap(const stringmap &Map, const std::string &str) {
      return Map.count(str) > 0;
  }

  void Emit(const std::string &str) { Emit(stdout, str);}
  void Emit(FILE *f, const std::string &str) { fprintf(f, "%s\n", str.c_str());}
  inline std::string _newLabel() {
    labelCount += 1;
    return LabelPref + std::to_string(labelCount);
  }
  inline bool _isNum(const std::string &s) {
    try {
      auto num = std::stoll(s);
    } catch(...) { 
      return false; 
    }
    return true;
  }
  void _codegen(FILE *f, stringvec line);

  enum class TType {
    Empty,            /* empty line*/
    TFunc,            /* f_name [pcnt] [size] */
    TEndFunc,         /* end f_name */
    TGlobalArr,       /* v1 = malloc 12345 */
    TGlobalVar,       /* v0 = 0 */
    TAssignInt,       /* reg = integer */  
    TBiExpr,          /* reg1 = reg2 op reg3 */
    TBiseqz,          /* reg1 = reg2 >=/<=/== reg3 */
    TBisnez,          /* reg1 = reg2 ||/!= reg3*/
    TAnd,             /* reg1 = reg2 && reg3 */
    TBiImm,           /* reg1 = reg2 op imm*/
    TAssignReg,       /* reg1 = reg2 */
    TUExpr3,          /* reg1 = -reg2 / reg1 = !reg2 */
    TUExpr4,          /* reg1 = - reg2 / reg1 = ! reg2 */
    TAssignToArr,     /* reg [1] = reg */
    TAssignFromArr,   /* reg = reg [2] */
    TCJump,           /* if reg1 op reg2 goto l3 */
    TLabel,           /* label */
    TJump,            /* goto Label */
    TCall,            /* call function */
    TStore,           /* store r0 4 */
    TLoad,            /* load int reg */
    TLoadAddr,        /* loadaddr int/gvar reg */
    TReturn           /* return */
  };
  TType _which(const stringvec &line);

public:
  RISCVGen() {}
  void addLine(const std::string &line) {
    lines.push_back(line);
  }
  void codegen(FILE *f) {
     for (auto line : lines) {
       _codegen(f, _split(line));
     }
  }
};


} // namespace riscv

#endif
