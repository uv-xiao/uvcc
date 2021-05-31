#include "stack.h"
#include "regs.h"


namespace obj {
int StackFrame::allocateVar(evar_sptr var) {
  int pos{-1};
  if (!varTable.count(*var)) {
    pos = _allocate(1);
    varTable[*var] = VarPos(var, pos);
  }
  else {
    pos = varTable[*var].offset;
    std::cerr << var->getName() + " already exits in stack\n" << std::endl;
  }
  return pos;
}

int StackFrame::allocateArr(evar_sptr var, int size) {
  int pos{-1};
  if (!varTable.count(*var)) {
    pos = _allocate(size / 4);
    varTable[*var] = VarPos(var, pos);
  }
  else {
    pos = varTable[*var].offset;
    std::cerr << var->getName() + " already exits in stack" << std::endl;
  }
  return pos;
}

int StackFrame::addressOfReg(RegID_t rid) {
  if (rid > 0) return rid - 1;
  else
    throw std::runtime_error("Invalid reg on stack: " + globalRegs[rid]); 
}

int StackFrame::addressOfVar(evar_sptr var) {
  if (varTable.count(*var) == 0) {
    std::cerr << var->getName() + " isn't in stack" << std::endl;
    return -1;
  }
  else
    return varTable[*var].offset;
}

void StackFrame::codegen_lea(FILE *f, int offset, RegID_t rid) {
  fprintf(f, "loadaddr %s %s\n", std::to_string(offset).c_str()
                                , globalRegs[rid].c_str());
}

void StackFrame::codegen_spillin(FILE *f, int offset, RegID_t rid, 
                                  RegID_t offreg) {
  if (offreg > 0) {
    fprintf(f, "loadaddr %s t6\nt6 = %s\nt6 [0] = %s\n",
                std::to_string(offset).c_str(),
                globalRegs[offreg].c_str(), globalRegs[rid].c_str());
  } else {
    fprintf(f, "store %s %s\n", globalRegs[rid].c_str(), 
                                std::to_string(offset).c_str());
  }
}
void StackFrame::codegen_spillout(FILE *f, int offset, RegID_t rid) {
  fprintf(f, "load %s %s\n", std::to_string(offset).c_str(), 
                             globalRegs[rid].c_str());
}


} // namespace obj