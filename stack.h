#ifndef _STACK_H_
#define _STACK_H_


#include "types.h"
#include "vars.h"
#include <memory>

namespace obj {
  
class StackFrame {
  /* layout:
  * | regs: 26 * 4 | params | vars |
  */
private:
  struct VarPos {
    evar_sptr vptr;
    int offset;
    VarPos(){}
    VarPos(evar_sptr vptr, int offset)
      :vptr(vptr), offset(offset) {}
    bool operator < (const VarPos &b) const {
      return *vptr < *(b.vptr); 
    }
  };
  
  static const int REGS = 26;
  int count = 0; 
  int used = 0;
  std::map<EVar, VarPos> varTable;

  int _allocate(int slotNum) {
    used += slotNum;
    return used - slotNum + 1;
  }
public:
  StackFrame(): count(REGS), used(REGS) {}
  void expand(size_t size) { count += size / 4; }
  int size() { return count; }
  int allocateVar(evar_sptr var); 
  int allocateArr(evar_sptr var, int size);
  int addressOfVar(evar_sptr var);
  int addressOfReg(RegID_t rid);

  void codegen_spillin(FILE *f, int offset, RegID_t rid, RegID_t offreg = 0);
  void codegen_spillout(FILE *f, int offset, RegID_t rid);
  void codegen_lea(FILE *f, int offset, RegID_t rid);
};
}

#endif